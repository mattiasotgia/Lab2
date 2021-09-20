#include "stat.h"

//#include <TMath.h> // it will need to be compiled trough ROOT!!
#include <cmath>
#include <vector>

using namespace std;

stat::stat(){}

stat::~stat(){}

void stat::clear(){
    _v.clear();
}

double stat::avg(){
    double sum = 0;
    for(int i=0; i< _v.size(); i++)
        sum += _v[i];
    return sum/_v.size();
}

double stat::sqr_avg(){
    double sqr_sum = 0;
    for(int i = 0; i<_v.size(); i++)
        sqr_sum += _v[i]*_v[i];
    return sqr_sum/_v.size();
}

double stat::variance(){
    return sqr_avg() - avg()*avg();
}

double stat::emp_var(){
    return variance() * _v.size()/(_v.size() - 1);
}

double stat::std(){
    return sqrt(variance());
}

double stat::std_adp(){
    return sqrt(emp_var());
}

double stat::std_err(){
    return std_adp()/sqrt(_v.size());
}
