#include "net/UdpSocket.h"
#include <base/Logging.h>
#include <net/SocketsOps.h>


using namespace muduo;
using namespace muduo::net;

UdpSocket::UdpSocket(EventLoop *loop,
                const InetAddress &localAddr):
                                              loop_(loop),
                                              localAddr_(localAddr)
{
    int sockfd = sockets::createNonblockingUdpOrDie(localAddr_.family());

    socket_.reset(new Socket(sockfd));

    socket_->setReuseAddr(true);
    socket_->setReusePort(localAddr.toPort());
    socket_->bindAddress(localAddr);
    data_channel_.reset(new Channel(loop_, sockfd));
    data_channel_->setReadCallback(
    std::bind(&UdpSocket::handleRead, this, _1));
    data_channel_->setWriteCallback(
    std::bind(&UdpSocket::handleWrite, this));
    data_channel_->setCloseCallback(
    std::bind(&UdpSocket::handleClose, this));
    data_channel_->setErrorCallback(
    std::bind(&UdpSocket::handleError, this));

}

UdpSocket::UdpSocket(EventLoop *loop, bool ipv6 ) :loop_(loop)
{
    int sockfd = 0;
    if(ipv6)
    {
       sockfd =  sockets::createNonblockingUdpOrDie(AF_INET6);
    }
    else
    {
        sockfd = sockets::createNonblockingUdpOrDie(AF_INET);
    }

    socket_.reset(new Socket(sockfd));

}


UdpSocket::~UdpSocket()
{
}



int UdpSocket::Write(const void* buffer,int len,const InetAddress &remoteAddr)
{
    return sockets::writeto(socket_->fd(),buffer,len,remoteAddr.getSockAddr());

}

int UdpSocket::SetMessageCallback(UdpMessageCallback callback)
{
    dataCallback_ = callback;
    data_channel_->tie(shared_from_this());
    data_channel_->enableReading();
    return 0;
}



void UdpSocket::handleRead(Timestamp receiveTime)
{
    LOG_DBG("handleRead");
    struct sockaddr_in addr_;
    recvBuffer_.fill(0);
    int dataLen = sockets::readfrom(socket_->fd(),&recvBuffer_,1500,reinterpret_cast<sockaddr*>(&addr_));
    if(dataCallback_&&(dataLen >0 ))
    {
        InetAddress clientAddr(addr_); 
        dataCallback_(&recvBuffer_[0],dataLen,clientAddr);
    }
}

void UdpSocket::handleWrite()
{
    LOG_DBG("handleWrite");
}

void UdpSocket::handleClose()
{
    LOG_DBG("handleClose");
}

void UdpSocket::handleError()
{
    LOG_DBG("handleError");
}
