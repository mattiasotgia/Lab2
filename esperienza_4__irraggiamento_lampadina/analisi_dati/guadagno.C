#include<vector>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>

#include<TCanvas.h>
#include<TGraphErrors.h>
#include<TF1.h>
#include<TStyle.h>
#include<TAxis.h>
#include<TMath.h>
#include<TLatex.h>
#include<TLegend.h>

#include"../../LabTools/LabTools.h"

double get_VRangeErr(double errPercent, int partitions, double range1){return errPercent * partitions *  range1;}

void guadagno(){
    string nomefile;    

std::cout<<"nome file input"<<std::endl;
std::cin>>nomefile>>endl;

double vin, vout, fsvin, fsvout;
double errPercent=0.035;

TGraphErrors g;
int i=0;

while (file>>vin>>fsvin>>vout>>fsvout){
double err_vin=get_VRangeErr(errPercent, 8, fsvin);
double err_vout=get_VRangeErr(errPercent,8,fsvout);

g.SetPoint(i,vin,vout);
g.SetPointError(i,err_vin,err_vout);
i++;

}

}