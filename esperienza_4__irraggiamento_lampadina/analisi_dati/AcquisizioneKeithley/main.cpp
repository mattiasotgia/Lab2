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
#include "TSystem.h"

#include "daqlib.h"

using namespace std;

int fd;

TApplication app("app",0,NULL);
ofstream     outfile("output.dat");

void stop(int sig){
  outfile.close();
  app.Run(true);
  exit(0);
}

int main(){

  //Configuro signal handler
  signal(SIGINT,stop);

  // Apertura comunicazioni
  fd = open("/dev/ttyS0",O_RDWR);
  if (fd == -1){
    cout << "Porta seriale non accessibile " << endl;
    exit(1);
  }

  // configurazione
  // STEP 1 configuro seriale
  // - bastano i primi due parametri di setserial
  //    * velocita' in bps
  //    * numero di bit
  //
  //** - elementi da usare - **
  // funzione setserial
  //** ----- **

  TCanvas can;
  TGraphErrors gr;
  gr.SetMarkerStyle(20);

  // STEP 3
  // inizializzo il calcolo del tempo T
  //
  //** - elementi da usare - **
  // classe Timer
  //** ----- **

  for (int i=0;;i++){

    double t    = i;
    double val  = 0;
    double eval = 0;

    // STEP 3
    // calcolo il tempo T (in generale diverso da i);
    //
    //** - elementi da usare - **
    // metodo Time della classe Timer
    //** ----- **

    // STEP 2
    // invio comando di lettura
    //   ":READ?\n";
    // aspetto 2 decimi di secondo
    // leggo il risultato
    //   read
    // (uso numero di byte alto, legge
    //  comunque fino alla fine).
    // con atof passo da carattere a
    // double
    //
    //** - elementi da usare - **
    // funzione write
    // funzione read
    // usleep
    // atof 
    //** ----- **

    // STEP 4 (facoltativo)
    // calcolo l'errore
    // - estraggo il range 
    //    (comando ":VOLT:DC:RANG:UPP?\n")
    // - calcolo errore come da manuale
    //
    //** - elementi da usare - **
    // funzione write
    // funzione read
    // usleep
    // atof
    //** ----- **

    cout    << t << " " << val << " " << eval << endl;
    outfile << t << " " << val << " " << eval << endl;
    gr.SetPoint(i,t,val);
    gr.SetPointError(i,0,eval);

    if (i==0)
      gr.Draw("AP");
    else {
      can.Modified();
      can.Update();
      gSystem->ProcessEvents();
    }
    
  }

  app.Run(true);
  return 0;

}












