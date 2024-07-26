

#ifndef MUDUO_NET_TCPSOCKET_H
#define MUDUO_NET_TCPSOCKET_H



#include <base/noncopyable.h>
#include <base/StringPiece.h>
#include <base/Types.h>
#include <net/Callbacks.h>
#include <net/Buffer.h>
#include <net/InetAddress.h>
#include <net/Channel.h>
#include <net/EventLoop.h>
#include <net/Socket.h>

namespace muduo
{
namespace net
{

class Channel;
class EventLoop;




class TcpSocket: noncopyable,
                  public std::enable_shared_from_this<TcpSocket>
{

public:
    friend class Acceptor;

public:
    TcpSocket(EventLoop *loop,
                const InetAddress &peerAddr,
                const string &nameArg);
    TcpSocket(EventLoop *loop,
            const InetAddress &peerAddr);
    ~TcpSocket();

    void AsyncConnect(ConnectionCallback callback);
    void Disconnect();

    void Send(const void* message, int len);
    void SetReadCallback(MessageCallback callback);
    void SetCloseCallback( CloseCallback callback);

    int GetSocketFd(){return socket_->fd();}

private:
    enum States { kDisconnected, kConnecting, kConnected, kDisconnecting };
    void connectInLoop();
    void asyncConnect();
    void connecting(int sockfd);
    void handleLinkWrite();
    void handleLinkError();

    void setState(States s) { state_ = s; }
    int removeAndResetChannel();
    void resetChannel();

    // called when TcpServer accepts a new connection
    void connectEstablished(int sockfd);   // should be called only once
    // called when TcpServer has removed me from its map
    void connectDestroyed();  // should be called only once

    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    void send(const StringPiece& message);
    // void sendInLoop(string&& message);
    void sendInLoop(const StringPiece& message);
    void sendInLoop(const void* message, size_t len);
    void shutdownInLoop();
    const char* stateToString() const;

private:
    EventLoop* loop_;
    InetAddress serverAddr_; 
    const InetAddress localAddr_;
    bool connect_; // atomic
    States state_;  // FIXME: use atomic variable
    std::unique_ptr<Channel> link_channel_;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> data_channel_;
    const string name_;
    size_t highWaterMark_;
    Buffer inputBuffer_;
    Buffer outputBuffer_; // FIXME: use list<Buffer> as output buffer.
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;
    ConnectionCallback connectedCallback_;
};




    } // namespace net
} // namespace muduo

#endif