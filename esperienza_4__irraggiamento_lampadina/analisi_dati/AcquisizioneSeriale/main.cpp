#include <iostream>
#include <math.h>
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

#include "sd.h"

using namespace std;


int main(){

  TApplication app("app",0,NULL);
  ofstream     outfile("output.dat");
  int fd;
  
  TCanvas can;
  TGraphErrors gr;
  gr.SetMarkerStyle(20);

  // STEP 0
  // Aprire la comunicazione seriale
  // 
  // Voglio aprire la porta seriale "/dev/ttyS0"
  // in modo di lettura e scrittura (O_RDWR)
  //
  // Controllo il valore di ritorno:
  //   numero intero positivo --> numero del file descriptor da usare successivamente
  //   -1 --> apertura fallita --> esco dal programma
  //
  //** - elementi da usare - **
  // funzione sd::open
  //** ----- **

  fd = sd::open("/dev/ttyS0", O_RDWR);
  if(fd = -1) return 1;


  // STEP 1
  // Impostare la distanza
  //
  // Invio comando di distanza
  //   "DIST XXX"
  // dove XXX = 101 / 122 / 142 / 162 / 183 / 211
  //
  // Controllo il valore di ritorno:
  //   numero intero positivo --> OK
  //   -1 --> comando fallito --> esco dal programma
  //
  //** - elementi da usare - **
  // funzione sd::write
  //** ----- **



  // STEP 3
  // Inizializzazione del timer T
  //
  // Creo un oggetto T della classe Timer
  //
  //** - elementi da usare - **
  // classe Timer
  //** ----- **



  for (int i=0;i<1000;i++){

    double t = i;
    double val = 0;
    double eval = 0;
 
    // STEP 3
    // Calcolo il tempo t dall'inizializzazione del timer T
    //
    // Chiamata al metodo Time()
    //
    //** - elementi da usare - **
    // metodo Time() della classe Timer
    //** ----- **
    


    // STEP 2
    // Lettura di un valore di tensione
    //
    // Invio comando di lettura
    //   ":MEAS:VOLT:DC?";
    //
    // Controllo il valore di ritorno:
    //   numero intero positivo --> procedo alla lettura del risultato
    //   -1 --> comando fallito --> dati finiti, esco dal ciclo for
    //
    // Leggo il risultato
    // (uso numero di byte alto, legge comunque fino alla fine).
    //
    // Converto i caratteri letti in un double (val)
    //
    // Calcolo l'errore (1%) sul valore letto (eval)
    //
    //** - elementi da usare - **
    // funzione sd::write
    // funzione sd::read
    // funzione atof 
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
