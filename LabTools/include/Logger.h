#ifndef MESSAGES
#define MESSAGES

namespace Logger {

  void setLogLevel(int level); 

  //! Reset time counter
  void setTimer(void); 

  //! Return 1 if more than a certain number of seconds have passed since the last call of this function or the last timer reset. 
  /*!
    \param [in] secs number of seconds
    \return 0/1
  */ 
  int timer(int secs);  

  void print(int level, const char* format, ...); 

}

#endif
