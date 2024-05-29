// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_CALLBACKS_H
#define MUDUO_NET_CALLBACKS_H

#include <base/Timestamp.h>
#include <net/InetAddress.h>


#include <functional>
#include <memory>

namespace muduo
{

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

// should really belong to base/Types.h, but <memory> is not included there.

template<typename T>
inline T* get_pointer(const std::shared_ptr<T>& ptr)
{
  return ptr.get();
}

template<typename T>
inline T* get_pointer(const std::unique_ptr<T>& ptr)
{
  return ptr.get();
}

// Adapted from google-protobuf stubs/common.h
// see License in base/Types.h
template<typename To, typename From>
inline ::std::shared_ptr<To> down_pointer_cast(const ::std::shared_ptr<From>& f) {
  if (false)
  {
    implicit_cast<From*, To*>(0);
  }

#ifndef NDEBUG
  assert(f == NULL || dynamic_cast<To*>(get_pointer(f)) != NULL);
#endif
  return ::std::static_pointer_cast<To>(f);
}

namespace net
{

// All client visible callbacks go here.

class Buffer;
class TcpSocket;
typedef std::shared_ptr<TcpSocket> TcpSocketPtr;
typedef std::function<void()> TimerCallback;
typedef std::function<void (const TcpSocketPtr&)> ConnectionCallback;
typedef std::function<void (const TcpSocketPtr&)> CloseCallback;
typedef std::function<void (const TcpSocketPtr&)> WriteCompleteCallback;
typedef std::function<void (const TcpSocketPtr&, size_t)> HighWaterMarkCallback;
typedef std::function<void (const void*,size_t, InetAddress clientAddr_)> UdpMessageCallback;

// the data has been read to (buf, len)
typedef std::function<void (const TcpSocketPtr&,
                            Buffer*,
                            Timestamp)> MessageCallback;

void defaultConnectionCallback(const TcpSocketPtr& conn);
void defaultMessageCallback(const TcpSocketPtr& conn,
                            Buffer* buffer,
                            Timestamp receiveTime);


}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_CALLBACKS_H
