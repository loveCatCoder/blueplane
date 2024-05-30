#include "connector/producer/RtspProducer.h"

#include <muduo/net/EventLoop.h>
using namespace muduo::net;

int main()
{
    EventLoop loop;
    RtspProducer camera(loop);

    loop.loop();
}









