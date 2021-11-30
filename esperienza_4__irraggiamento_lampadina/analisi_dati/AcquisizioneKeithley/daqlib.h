#include <termios.h>
#include <string>
#include <sys/time.h>

using namespace std;

ssize_t readserial(int fd, void *buf, size_t count);
void    setserial(int fd, unsigned char speed, unsigned char bits, unsigned char stopb=0,string instrument="KEITHLEY");

class Timer{
 public:
  Timer();
  void   Start();
  double Time();
 private:
  timeval m_t0;
};

