// Copied from https://gitlab.cern.ch/lhcb/Urania/-/blob/master/PhysTools/Meerkat/src/Logger.cpp

#include <time.h>
#include <stdarg.h>
#include <stdio.h>

#include "Logger.h"

static int last_timer; 
static int log_level = 0;

void Logger::setTimer(void) {
    last_timer = time(0);
}

int Logger::timer(int secs) {
    int t = time(0); 
    if ( t - last_timer > secs ) {
      last_timer = t; 
      return 1; 
    } else {
      return 0; 
    }
}

void Logger::setLogLevel(int level) {
    log_level = level; 
}

void Logger::print(int level, const char* fmt, ...) {
    if (level >= log_level) {
      va_list ap;
      va_start(ap, fmt);
      vprintf(fmt, ap);
      va_end(ap);
    }
}
