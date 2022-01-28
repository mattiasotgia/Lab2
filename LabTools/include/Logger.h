#ifndef LABTOOLS_Logger
#define LABTOOLS_Logger

#define warnstr "\u001b[31;1mwarning:\u001b[0m"

namespace Logger {

  const char*   warning(const char* _warning);
  const char*   error(const char* _error);
  void          setLogLevel(int level); 
  void          setTimer(void); //> ! Reset time counter

  /*!
    \param [in] secs number of seconds
    \return 0/1
  */ 
  int           timer(int secs); //> ! Return 1 if more than a certain number of seconds have passed since the last call of this function or the last timer reset. 
  void          print(int level, const char* format, ...); 

} //Logger

#endif //LABTOOLS_Logger
