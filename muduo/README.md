# redpig

One C++ network library based on the reactor pattern.
# Overview

Tcp Acceptor  
Tcp Socket  
Udp Socket  
# todo

Tls  
Dtls  


## muduo使用指南

### 启用事件循环

```c++
muduo::ThreadPool pool(1);
EventLoop loop;
loop.loop();
```
### tcp客户端

```c++
//初始化客户端
muduo::net::InetAddress peerAddr("10.0.21.42", 554);
m_socket = std::make_shared<TcpSocket>(&loop,peerAddr,"test");
//设置读数据回调
m_socket->SetReadCallback([this](const TcpSocketPtr& ptr,
                            Buffer* buffer,
                            muduo::Timestamp time){
		int len = buffer->readAll(m_recvBuf.data()+m_recvLen);
		m_recvLen += len;
		log_debug("recv len:%d,sum len:%d",len,m_recvLen);
		ReceiveTcpMessage();
});
//连接对端
m_socket->AsyncConnect([this](const TcpSocketPtr&,int err){
    if (err == 0)
    {
    SendOptionsRequest();
    }
    else
    {
    log_error("rtsp options send:%d",err);
    }
});
//发送数据
m_socket->Send(SendBuf,strlen(SendBuf));

```



### tcp服务器





### udp

```C++
//客户端
UdpSocket client(&loop);
client.Start();
muduo::net::InetAddress clientAddr_("10.0.15.81",7866);
client.Write(buffer,100,clientAddr_);
//服务端
```

