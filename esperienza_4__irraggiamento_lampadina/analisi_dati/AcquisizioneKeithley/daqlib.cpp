#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "daqlib.h"

ssize_t readserial(int fd, void *buf, size_t count){

  char *cbuf = (char*)buf;

  //Elimino tutti i caratteri spuri
  do{
    read(fd,cbuf,1); 
  } while ((cbuf[0] < 'A' || cbuf[0] > 'z') && (int)cbuf[0]!=' ' && (int)cbuf[0]!='$');

  int offset = 1;
  int nread  = 1;

  //Continuo a leggere se il numero di bit non e' quello aspettato
  while (nread<count){
    int rread = read(fd,cbuf+offset,count-offset);
    offset   += rread;
    nread    += rread;
  }

  return nread;

}

void setserial(int fd, unsigned char speed, unsigned char bits, unsigned char stopb, string instrument){

  struct termios term;             // crea variabile di tipo termios
  
  term.c_oflag  = 0;               // ...ne assegna i membri  
  term.c_lflag  = 0;
  term.c_iflag  = IGNBRK | IGNPAR;

  if (stopb!=0){
    term.c_cflag  = CREAD | CLOCAL | bits | stopb;
  } else {
    term.c_cflag  = CREAD | CLOCAL | bits;
  }

  cfsetispeed(&term,speed);        // setta la velocita' in input
  cfsetospeed(&term,speed);        // setta la velocita' in output

  tcsetattr(fd,TCSANOW,&term);     // assegna la struttura alla seriale

  write(fd,"*RST\n",5);
  usleep(400000);
  write(fd,"*CLS\n",5);
  usleep(400000);

}


Timer::Timer(){
  gettimeofday(&m_t0,NULL);
}

void Timer::Start(){
  gettimeofday(&m_t0,NULL);
}

double Timer::Time(){
  timeval t;
  gettimeofday(&t,NULL);
  double res = (t.tv_sec-m_t0.tv_sec)+1e-6*(t.tv_usec-m_t0.tv_usec);
  return res;
}
