

#include <iostream>
#include <vector>
#include <chrono>

#include "base/ThreadPool.h"
#include "net/EventLoop.h"
#include "net/TcpSocket.h"
#include "net/UdpSocket.h"
#include <base/Logging.h>

using namespace muduo::net;
#include "stdio.h"

int TimerRunCount = 0;

void printOne()
{
    printf("Timer Run ：%d\r\n", ++TimerRunCount);
}
int main()
{
    muduo::ThreadPool pool(1);
    EventLoop loop;

    std::thread t2(std::bind(&EventLoop::loop,&loop)); // pass by value
    t2.detach();

    // loop.runEvery(3, [&pool]()
    //               { pool.enqueue(printOne); });
    // loop.runEvery(2, [&pool]()
    //               { pool.enqueue(printOne); });

    //muduo::net::InetAddress listenAddr(8899);
    // UdpSocket aa(&loop,listenAddr,[&aa](const void*buffer,size_t len, InetAddress clientAddr_){
    //     LOG_DBG("data: %s, ip:%s ,port: %d",(char*)buffer,clientAddr_.toIp().c_str(),clientAddr_.toPort());
    //     aa.Write(buffer,len,clientAddr_);
    // });
    muduo::net::InetAddress listenAddr();
    UdpSocket client(&loop);
    std::shared_ptr<UdpSocket> ptr;
    ptr.reset(&client);
    ptr->Start();

    while (true)
    {
        char buffer[100] = "my name is redpig;i am one network lib";
        muduo::net::InetAddress clientAddr_("10.0.15.81",7866);
        ptr->Write(buffer,100,clientAddr_);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}