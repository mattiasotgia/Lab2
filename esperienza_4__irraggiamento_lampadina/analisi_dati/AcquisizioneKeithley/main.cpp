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
ofstream* outfile;

void stop(int sig){
  outfile->close();
  app.Run(true);
  exit(0);
}

int main(int argc, char ** argv){

  double sleep_time = 0;
  if(argc>1 && !strcmp(argv[1], "-q")){
    goto startDATAAQ;
  }
  std::cout << "Attesa tra due punti (s): " << std::flush;
  std::cin >> sleep_time;

  startDATAAQ:
  std::string filename;
  std::cout << "Nome file output: " << std::flush;
  std::cin >> filename;
  outfile = new ofstream(("../../dati/" + filename + ".dat").c_str());

  //Configuro signal handler
  signal(SIGINT,stop);

  // *outfile << ":START?" << std::endl;

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

  setserial(fd, B9600, CS8);

  TCanvas can;
  TGraphErrors gr;
  gr.SetMarkerStyle(20);

  // STEP 3
  // inizializzo il calcolo del tempo T
  //
  //** - elementi da usare - **
  // classe Timer
  //** ----- **
  Timer T;
  T.Start();

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
    
    t = T.Time();

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

    std::string r_cmd = ":READ?\n";
    int w = write(fd, r_cmd.c_str(), r_cmd.length());
    if(w == -1){
      std::cout << "Fallito invio comando al seriale" << std::endl;
      exit(2);
    }
    usleep(100000);
    char ch[100];
    int r = read(fd, ch, 100);
    if(r == -1){
      std::cout << "Fallita lettura dal seriale" << std::endl;
      exit(2);
    }
    val = atof(ch);

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

    std::string r_err_cmd = ":VOLT:DC:RANG:UPP?\n";
    int w_err = write(fd, r_err_cmd.c_str(), r_err_cmd.length());
    if(w_err == -1){
      std::cout << "Fallito invio comando al seriale" << std::endl;
      exit(2);
    }
    usleep(100000);
    char ch_err[100];
    int err_r = read(fd, ch_err, 100);
    if(err_r == -1){
      std::cout << "Fallita lettura dal seriale" << std::endl;
      exit(2);
    }
    double range = atof(ch_err);

    if(range == 0.1)       eval = (50e-6*val+35e-6*range)/sqrt(3);
    else if(range == 1)    eval = (30e-6*val+7e-6*range)/sqrt(3);
    else if(range == 10)   eval = (30e-6*val+5e-6*range)/sqrt(3);
    else if(range == 100)  eval = (45e-6*val+6e-6*range)/sqrt(3);
    else if(range == 1000) eval = (45e-6*val+6e-6*range)/sqrt(3);

    cout    << t << " " << val << " " << eval << " " << range << " " << ch_err << endl;
    *outfile << t << " " << val << " " << eval << " " << range << " " << ch_err << endl;
    gr.SetPoint(i,t,val);
    gr.SetPointError(i,0,eval);

    if (i==0)
      gr.Draw("AP");
    else {
      can.Modified();
      can.Update();
      gSystem->ProcessEvents();
    }
    sleep(sleep_time);
  }

  app.Run(true);
  return 0;

}












