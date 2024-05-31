
#include <base/Logging.h>
#include <net/TcpSocket.h>
#include <net/SocketsOps.h>

using namespace muduo;
using namespace muduo::net;

TcpSocket::TcpSocket(EventLoop *loop,
                     const InetAddress &peerAddr,
                     const string &nameArg) : loop_(loop),
                                              serverAddr_(peerAddr),
                                              connect_(false),
                                              state_(kDisconnected),
                                              name_(nameArg)
{
  LOG_DBG("TcpSocket ServerAddr: %s", serverAddr_.toIpPort().c_str());
}

TcpSocket::~TcpSocket()
{
}

void TcpSocket::AsyncConnect(ConnectionCallback callback)
{
  LOG_DBG("TcpClient::connect[%s] - connecting to %s connect status:%d", name_.c_str(), serverAddr_.toIpPort().c_str(),state_);
  connect_ = true;
  loop_->runInLoop(std::bind(&TcpSocket::connectInLoop, this)); // FIXME: unsafe
  connectedCallback_ = callback;
}

void TcpSocket::Disconnect()
{
}

void TcpSocket::Send(const void* message, int len)
{
    send(StringPiece(static_cast<const char*>(message), len));
}

void TcpSocket::SetReadCallback(MessageCallback callback)
{
  messageCallback_ = callback;
}

void TcpSocket::send(const StringPiece& message)
{
   if (state_ == kConnected)
  {
    if (loop_->isInLoopThread())
    {
      sendInLoop(message);
    }
    else
    {
       std::function<void()> funcWithoutParam = [this,message]() {
        sendInLoop(message); 
      };
      loop_->runInLoop(funcWithoutParam);
    }
  }
}

void TcpSocket::sendInLoop(const StringPiece& message)
{
     sendInLoop(message.data(), message.size());
}

void TcpSocket::sendInLoop(const void* message, size_t len)
{
  loop_->assertInLoopThread();
  ssize_t nwrote = 0;
  size_t remaining = len;
  bool faultError = false;
  if (state_ == kDisconnected)
  {
    LOG_WARN("disconnected, give up writing");
    return;
  }
  // if no thing in output queue, try writing directly
  if (!data_channel_->isWriting() && outputBuffer_.readableBytes() == 0)
  {
    nwrote = sockets::write(data_channel_->fd(), message, len);
    if (nwrote >= 0)
    {
      remaining = len - nwrote;
      if (remaining == 0 && writeCompleteCallback_)
      {
        loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
      }
    }
    else // nwrote < 0
    {
      nwrote = 0;
      if (errno != EWOULDBLOCK)
      {
        LOG_ERR("TcpConnection::sendInLoop");
        if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
        {
          faultError = true;
        }
      }
    }
  }

  assert(remaining <= len);
  if (!faultError && remaining > 0)
  {
    size_t oldLen = outputBuffer_.readableBytes();
    if (oldLen + remaining >= highWaterMark_
        && oldLen < highWaterMark_
        && highWaterMarkCallback_)
    {
      loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
    }
    outputBuffer_.append(static_cast<const char*>(message)+nwrote, remaining);
    if (!data_channel_->isWriting())
    {
      data_channel_->enableWriting();
    }
  }
}

void TcpSocket::shutdownInLoop()
{
    loop_->assertInLoopThread();
  if (!data_channel_->isWriting())
  {
    // we are not writing
    socket_->shutdownWrite();
  }
}

const char* TcpSocket::stateToString() const
{
  switch (state_)
  {
    case kDisconnected:
      return "kDisconnected";
    case kConnecting:
      return "kConnecting";
    case kConnected:
      return "kConnected";
    case kDisconnecting:
      return "kDisconnecting";
    default:
      return "unknown state";
  }
} 


void TcpSocket::connectInLoop()
{
  loop_->assertInLoopThread();
  assert(state_ == kDisconnected);
  if (connect_)
  {
    asyncConnect();
  }
  else
  {
    LOG_DBG("%s: do not connect", name_.c_str());
  }
}

void TcpSocket::asyncConnect()
{
  int sockfd = sockets::createNonblockingOrDie(serverAddr_.family());
  int ret = sockets::connect(sockfd, serverAddr_.getSockAddr());
  int savedErrno = (ret == 0) ? 0 : errno;
  switch (savedErrno)
  {
  case 0:
  case EINPROGRESS:
  case EINTR:
  case EISCONN:
    connecting(sockfd);
    break;

  case EAGAIN:
  case EADDRINUSE:
  case EADDRNOTAVAIL:
  case ECONNREFUSED:
  case ENETUNREACH:
    //   retry(sockfd);
    //   break;

  case EACCES:
  case EPERM:
  case EAFNOSUPPORT:
  case EALREADY:
  case EBADF:
  case EFAULT:
  case ENOTSOCK:
    LOG_ERR("connect error in Connector::startInLoop %d", savedErrno);
    sockets::close(sockfd);
    break;

  default:
    LOG_ERR("Unexpected error in Connector::startInLoop %d", savedErrno);
    sockets::close(sockfd);
    // connectErrorCallback_();
    break;
  }
}

void TcpSocket::connecting(int sockfd)
{
  setState(kConnecting);
  assert(!link_channel_);
  link_channel_.reset(new Channel(loop_, sockfd));
  link_channel_->setWriteCallback(
      std::bind(&TcpSocket::handleLinkWrite, this)); // FIXME: unsafe
  link_channel_->setErrorCallback(
      std::bind(&TcpSocket::handleLinkError, this)); // FIXME: unsafe

  // link_channel_->tie(shared_from_this()); is not working,
  // as link_channel_ is not managed by shared_ptr
  link_channel_->enableWriting();
}

void TcpSocket::handleLinkWrite()
{
  LOG_DBG("Connector::handleWrite: %d ", state_);

  if (state_ == kConnecting)
  {
    int sockfd = removeAndResetChannel();
    int err = sockets::getSocketError(sockfd);
    if (err)
    {

      LOG_ERR("Connector::handleWrite - SO_ERROR = %d %s ", err, strerror_tl(err));
    }
    else if (sockets::isSelfConnect(sockfd))
    {
      LOG_WARN("Connector::handleWrite - Self connect");
    }
    else
    {
      LOG_DBG("connect_: %d",connect_);
      if (connect_)
      {
        connectEstablished(sockfd);
        if(connectedCallback_)
        {
          connectedCallback_(shared_from_this(),0);
        }
      }
      else
      {
        sockets::close(sockfd);
      }
    }
  }
  else
  {
    // what happened?
    assert(state_ == kDisconnected);
  }
}

void TcpSocket::handleLinkError()
{
  LOG_ERR("Connector::handleError state= %d", state_);
  if (state_ == kConnecting)
  {
    int sockfd = removeAndResetChannel();
    int err = sockets::getSocketError(sockfd);
    LOG_ERR("SO_ERROR = %d %s ", err, strerror_tl(err));
    if(connectedCallback_)
    {
      connectedCallback_(shared_from_this(),-1);
    }
  }
  setState(kDisconnected);
}

int TcpSocket::removeAndResetChannel()
{
  link_channel_->disableAll();
  link_channel_->remove();
  int sockfd = link_channel_->fd();
  // Can't reset link_channel_ here, because we are inside Channel::handleEvent
  loop_->queueInLoop(std::bind(&TcpSocket::resetChannel, this)); // FIXME: unsafe
  return sockfd;
}

void TcpSocket::resetChannel()
{
  link_channel_.reset();
}

void TcpSocket::connectEstablished(int sockfd)
{
  loop_->assertInLoopThread();
  assert(state_ == kConnecting);
  socket_.reset(new Socket(sockfd)),
      data_channel_.reset(new Channel(loop_, sockfd));
  data_channel_->setReadCallback(
      std::bind(&TcpSocket::handleRead, this, _1));
  data_channel_->setWriteCallback(
      std::bind(&TcpSocket::handleWrite, this));
  data_channel_->setCloseCallback(
      std::bind(&TcpSocket::handleClose, this));
  data_channel_->setErrorCallback(
      std::bind(&TcpSocket::handleError, this));
  LOG_DBG("TcpConnection::ctor[%s] at  fd= %d" ,name_.c_str(),sockfd) ;
  socket_->setKeepAlive(true);


  setState(kConnected);
  data_channel_->tie(shared_from_this());
  data_channel_->enableReading();

}

void TcpSocket::connectDestroyed()
{
}

void TcpSocket::handleRead(Timestamp receiveTime)
{
   loop_->assertInLoopThread();
  int savedErrno = 0;
  ssize_t n = inputBuffer_.readFd(data_channel_->fd(), &savedErrno);
  if (n > 0)
  {
    messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
  }
  else if (n == 0)
  {
    handleClose();
  }
  else
  {
    errno = savedErrno;
    LOG_ERR("TcpConnection::handleRead") 
    handleError();
  }
}

void TcpSocket::handleWrite()
{
  loop_->assertInLoopThread();
  if (data_channel_->isWriting())
  {
    ssize_t n = sockets::write(data_channel_->fd(),
                               outputBuffer_.peek(),
                               outputBuffer_.readableBytes());
    if (n > 0)
    {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.readableBytes() == 0)
      {
        data_channel_->disableWriting();
        if (writeCompleteCallback_)
        {
          loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
        }
        if (state_ == kDisconnecting)
        {
          shutdownInLoop();
        }
      }
    }
    else
    {
      LOG_ERR("TcpConnection::handleWrite");
      // if (state_ == kDisconnecting)
      // {
      //   shutdownInLoop();
      // }
    }
  }
  else
  {
    LOG_WARN("Connection fd = %d is down, no more writing",data_channel_->fd());
  }
}

void TcpSocket::handleClose()
{
  loop_->assertInLoopThread();
  LOG_PRI("fd = %d state = %s",data_channel_->fd() ,stateToString());
  assert(state_ == kConnected || state_ == kDisconnecting);
  // we don't close fd, leave it to dtor, so we can find leaks easily.
  setState(kDisconnected);
  data_channel_->disableAll();

  // TcpConnectionPtr guardThis(shared_from_this());
  // connectionCallback_(guardThis);
  // must be the last line
  // closeCallback_(guardThis);
}

void TcpSocket::handleError()
{
  int err = sockets::getSocketError(data_channel_->fd());
  LOG_ERR( "TcpConnection::handleError [%s] - SO_ERROR = %d",name_.c_str() ,strerror_tl(err));
}