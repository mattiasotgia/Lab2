#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>
#include <map>
#include <vector>
#include <time.h>
#include <TRandom.h>
#include <TF1.h>
#include <unistd.h>
#include "sd.h"

using namespace std;

int     sd_ser     = -1;
int     sd_v_count = -1;
bool    sd_meas    = false;
bool    sd_v_meas  = false;
string  sd_dist    = "";
TRandom sd_m_rnd;
map< string, vector<double> > volts;



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



int sd::open(const char *pathname, int flags){

  string str(pathname,strlen(pathname));

  int fd=-1;

  if (str=="/dev/ttyS0"){
    if (flags!=O_RDWR) return -1;
    fd=sd_ser=4;
  } else {
    return -1;
  }

  sd_m_rnd.SetSeed(time(0));

  vector<string> names;
  names.push_back("101");
  names.push_back("122");
  names.push_back("142");
  names.push_back("162");
  names.push_back("183");
  names.push_back("211");
  for (unsigned int i=0; i<names.size(); i++) {
    ifstream ifs( (string("./data/")+(names[i])+".bin").c_str(), ios::binary );
    if (!ifs.good()) return -1;
    vector<double> volt;
    while (ifs.good()) {
      double d;
      ifs.read( reinterpret_cast<char*>( &d ), sizeof d );
      ifs.read( reinterpret_cast<char*>( &d ), sizeof d );
      if (ifs.good()) volt.push_back(d);
    }
    ifs.close();
    volts.insert(make_pair(names[i], volt));
  }

  return fd;

}

int sd::close(int fd){
  if (fd==sd_ser)
    return 0;
  else
    return -1;
}


int sd::write(int fd, const void *buf,  int count){

  if (fd!=sd_ser) {
    return -1;
  }

  char          *cbuf = (char*)buf;
  unsigned char *uch = (unsigned char *)buf;
  string str(cbuf,count);

  if (count==14 && str==":MEAS:VOLT:DC?" && sd_meas) {
    sd_v_count++;
    if(sd_v_count>=volts[sd_dist].size()) return -1;
    sd_v_meas = true;
    usleep(500000);
    return count;
  }

  if (count==8 && str.substr(0,4)=="DIST"){
    sd_dist = str.substr(5,3);
    if (volts.find(sd_dist)==volts.end()) {sd_dist=""; return -1;}
    sd_v_meas = false;
    sd_meas = true;
    sd_v_count = -1;
    return count;
  }

  return -1;

}

int sd::read(int fd, void *buf,  int count){

  if (fd!=sd_ser || !sd_meas) {
    return -1;
  }

  if (sd_v_meas) {
    if(sd_v_count<0 || sd_v_count>=volts[sd_dist].size()) return -1;
    double volt = (volts[sd_dist])[sd_v_count];
    volt += sd_m_rnd.Gaus(0,0.01*volt);
    ostringstream os;
    os << volt << endl;
    string ostr = os.str();
    char *cbuf = (char*)buf;
    strncpy(cbuf,ostr.c_str(),7);
    sd_v_meas = false;
    return 7;
  }

  return -1;

}

