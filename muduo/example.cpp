

#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <map>

#include "base/ThreadPool.h"
#include "net/EventLoop.h"
#include "net/TcpSocket.h"
#include "net/UdpSocket.h"
#include "net/Acceptor.h"
#include <base/Logging.h>
#include "base/Timestamp.h"

using namespace muduo::net;
#include "stdio.h"

int TimerRunCount = 0;

void printOne()
{
    printf("Timer Run ：%d\r\n", ++TimerRunCount);
}







class EchoServer
{
private:
    /* data */
public:
    EchoServer(EventLoop* loop){ m_loop = loop;}
    ~EchoServer(){}
    int InIt();
    void NewTcpConnection(int err, std::shared_ptr<TcpSocket> socket);
    void CloseTcpConnection(const std::shared_ptr<TcpSocket>& socket);
    void MessageCallback(const TcpSocketPtr&,Buffer*,muduo::Timestamp);
    void PrintClientInfo();
private:
    std::unique_ptr<Acceptor> m_acceptor;
    EventLoop* m_loop = nullptr;
    std::map<int,std::shared_ptr<TcpSocket>> m_clientMap;
};

int EchoServer::InIt()
{
    muduo::net::InetAddress listenAddr(6668);
    m_acceptor = std::make_unique<Acceptor>(m_loop,listenAddr,true);
    m_acceptor->setNewConnectionCallback(std::bind(&EchoServer::NewTcpConnection,this,std::placeholders::_1,std::placeholders::_2));
    m_acceptor->listen();
    return 0;
};

void EchoServer::NewTcpConnection(int err, std::shared_ptr<TcpSocket> socket)
{
    m_clientMap.emplace(socket->GetSocketFd(),socket);
    socket->SetReadCallback(std::bind(&EchoServer::MessageCallback,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    socket->SetCloseCallback(std::bind(&EchoServer::CloseTcpConnection,this,std::placeholders::_1));
}

void EchoServer::CloseTcpConnection(const std::shared_ptr<TcpSocket>& socket)
{
    m_clientMap.erase(socket->GetSocketFd());
}

void EchoServer::MessageCallback(const TcpSocketPtr& ptr,Buffer* data,muduo::Timestamp time)
{
    std::string dataStr = data->retrieveAllAsString();
    ptr->Send(dataStr.c_str(),dataStr.length());
}

void EchoServer::PrintClientInfo()
{
    printf("AllNode:\r\n");
    for(auto& node:m_clientMap)
    {
        printf("%d:%s\r\n",node.first);
    }
}




void StartUdpSocket(EventLoop* loop)
{
    UdpSocket client(loop);
    std::shared_ptr<UdpSocket> ptr;
    ptr.reset(&client);

    while (true)
    {
        char buffer[100] = "my name is redpig;i am one network lib";
        muduo::net::InetAddress clientAddr_("10.0.15.81",7866);
        ptr->Write(buffer,100,clientAddr_);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    //muduo::net::InetAddress listenAddr(8899);
    // UdpSocket aa(&loop,listenAddr,[&aa](const void*buffer,size_t len, InetAddress clientAddr_){
    //     LOG_DBG("data: %s, ip:%s ,port: %d",(char*)buffer,clientAddr_.toIp().c_str(),clientAddr_.toPort());
    //     aa.Write(buffer,len,clientAddr_);
    // });
}

int main()
{
    muduo::ThreadPool pool(1);
    EventLoop loop;



    std::thread workthread(std::bind(&EventLoop::loop,&loop));
    workthread.detach();

    EchoServer echo(&loop);
    echo.InIt();



    loop.runEvery(3, [&echo]()
                  { echo.PrintClientInfo(); });



    StartUdpSocket(&loop);
  
    while (true)
    {
         std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    
    return 0;
}