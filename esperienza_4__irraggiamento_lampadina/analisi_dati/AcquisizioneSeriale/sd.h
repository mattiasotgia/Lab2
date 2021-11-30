#include <sys/time.h>
#include <fcntl.h>

//#define O_RDWR     02
//#define O_RDONLY   00
//#define O_WRONLY   01
//#define PPWDATA   134


class Timer{

 public:

  Timer();
  void   Start();
  double Time();

 private:

  timeval m_t0;

};


namespace sd{

  int open(const char *pathname, int flags);

  int close(int fd);

  int write(int fd, const void *buf,  int count);

  int read(int fd, void *buf,  int count);

}
