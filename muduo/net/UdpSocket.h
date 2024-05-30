#ifndef MUDUO_NET_UDPSOCKET_H
#define MUDUO_NET_UDPSOCKET_H



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


class UdpSocket :noncopyable,
                  public std::enable_shared_from_this<UdpSocket>
{

public:
    UdpSocket(EventLoop *loop,
                const InetAddress &serverAddr,UdpMessageCallback cb = nullptr);
    UdpSocket(EventLoop *loop, bool ipv6 = false);
    ~UdpSocket();

    int Write(const void* buffer,int len,const InetAddress &remoteAddr);
    int Start();
private:
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

private:
    enum Modes { mServer, mClient };
    Modes mode_;
    EventLoop* loop_;
    InetAddress serverAddr_; 
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> data_channel_;
    UdpMessageCallback dataCallback_;
    std::array<char,1500> recvBuffer_;
};










}
}

#endif