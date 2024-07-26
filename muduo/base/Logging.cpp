
#include <base/Logging.h>
#include <thread>

__thread char t_errnobuf[512];

const char* strerror_tl(int savedErrno)
{
  return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}