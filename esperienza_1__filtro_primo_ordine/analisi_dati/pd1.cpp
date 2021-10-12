#include <TApplication.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TF1.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

double incertezza_1(double fsin){
  double e_Vin=8*0.035*sqrt(3)*fsin;
  return e_Vin;
}

double incertezza_2(double vin,double vout,double evin, double evout){
  double eh=sqrt(pow(evin/vin,2)+pow(evout/vout,2));
  return eh;
}

int main(){

  TApplication app("app", 0, 0);
  
  string nomefile;
  cout<<"digita nome file input"<<endl;
  cin>>nomefile;

  ifstream file(nomefile);
  if(! file.good()){
    cerr<<"Error reading file"<<endl;
    return 1;
  }

  const double R=50;
  const double C=0.00000000001;
  double Vin,Vout,fsVin,fsVout,T,fsT,dt,fsdt;
  int i=0;
  TGraphErrors g;
  TGraphErrors x;
  
  while(file>>Vin>>fsVin>>Vout>>fsVout>>T>>fsT>>dt>>fsdt){
   double eVin=incertezza_1(fsVin);
   double eVout=incertezza_1(fsVout);
   double eT=incertezza_1(fsT);
   double edt=incertezza_1(fsdt);

   double H=Vout/Vin;
   double w=2*M_PI/T;
   double phi=2*M_PI*dt/T;

   double eH=incertezza_2(Vin,Vout,eVin,eVout);
   double ew=2*M_PI*eT;
   double ephi=incertezza_2(dt,T,edt,T)*2*M_PI;

   g.SetPoint(i,w,H);
   g.SetPointError(i,ew,eH);

   x.SetPoint(i,w,phi);
   x.SetPointError(i,ew,ephi);

   
   i++;
  }
  TCanvas can1("can1","titolo1",800,500);
  can1.cd();
  TF1 f("f","1/sqrt(1+pow([0]/[1],2))");
  f.FixParameter(1,R*C);
  
  g.Fit("f");
  g.Draw("ap");
  TCanvas can2("can2","titolo2",800,500);
  can2.cd();
  x.Draw("ap");

 

  /*  TF1 f("f","1/sqrt(1+pow([0]/[1],2))");
   f.FixParameter(1,R*C);
  
    g.Fit("f");
  */

   
    
 app.Run();
   return 0;
}
