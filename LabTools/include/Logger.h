#ifndef MESSAGES
#define MESSAGES

namespace Logger {

  #define warnstr "\u001b[31;1mwarning:\u001b[0m"

  const char* warning(const char* _warning);
  const char* error(const char* _error);

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
