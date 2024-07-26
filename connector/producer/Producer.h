
#include <map>


class Producer
{
    virtual int Init() = 0; 
	virtual int StartStream() = 0;   // 虚析构函数
	virtual int StopStream() = 0;   // 虚析构函数
    virtual int Destroy() = 0;   // 虚析构函数
};