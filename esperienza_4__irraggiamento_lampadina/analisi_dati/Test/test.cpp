#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include <signal.h>

#include "TApplication.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

using namespace std;


int main(){

  //Configuro TApplication e file di output
  TApplication *app = new TApplication("app",0,NULL);
  ofstream outfile("output.dat");

  //Creo la TCanvas e il TGraph
  TCanvas can;
  TGraphErrors gr;
  gr.SetMarkerStyle(20);

  // Loop su quelle che saranno le acquisizioni

  for (int i=0;i<100;i++){

    // Dati "fake"
    double t    = i;
    double val  = 0;
    double eval = 0;

    // Stampo, scrivo su file, inserisco nel grafico
    cout    << t << " " << val << " " << eval << endl;
    outfile << t << " " << val << " " << eval << endl;
    gr.SetPoint(i,t,val);
    gr.SetPointError(i,0,eval);

    // Update del grafico
    if (i==0)
      gr.Draw("AP");
    else {
      can.Modified();
      can.Update();
    }
    
  }

  app->Run(true);
  outfile.close();
  return 0;

}












