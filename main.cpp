
#include <chrono>
#include <thread>

#include "connector/producer/RtspProducer.h"

#include <muduo/net/EventLoop.h>
using namespace muduo::net;

int main()
{
    EventLoop loop;
    RtspProducer camera(loop);

    //loop.loop();
    std::thread t2(std::bind(&EventLoop::loop,&loop)); // pass by value
    t2.detach();
    camera.Init();
    camera.StartStream();
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}









