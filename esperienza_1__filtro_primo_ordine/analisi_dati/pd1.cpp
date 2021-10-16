#include<vector>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>

#include<TApplication.h>
#include<TCanvas.h>
#include<TGraphErrors.h>
#include<TF1.h>
#include<TStyle.h>
#include<TAxis.h>
#include<TMath.h>
#include<TLatex.h>
#include<TLegend.h>

using namespace std;

const double R = 50; //fisso i valori di R e C
const double C = 0.00000000001;

double incertezza_1(double fsin)
{ //funzione calcolo incertezza a partire da fondo scala
  double e_Vin = 8 * 0.035 * fsin / sqrt(3);
  return e_Vin;
}

double incertezza_2(double vin, double vout, double evin, double evout)
{ //funzione calcolo propagazione errore statistici
  double eh = sqrt(pow(evin / vin, 2) + pow(evout / vout, 2));
  return eh;
}
 
int main(int argc, char** argv)
{

  TApplication app("app", 0, 0);
  
  string nomefile;

  if(argc<2){
    cout << "digita nome file input" << endl;
    cin >> nomefile;
  }else{
    nomefile = argv[2];
  }

  ifstream file(nomefile);
  if (!file.good())
  {
    cerr << "Error reading file" << endl;
    return 1;
  }


  double Vin, Vout, fsVin, fsVout, T, fsT, dt, fsdt;

  int i = 0;

  TGraphErrors g;
  TGraphErrors x;

  vector<double> vH, veH, vw, vew, vphi, vephi;

  while (file >> Vin >> fsVin >> Vout >> fsVout >> T >> fsT >> dt >> fsdt)
  {
    double eVin = incertezza_1(fsVin);
    double eVout = incertezza_1(fsVout);
    double eT = incertezza_1(fsT);
    double edt = incertezza_1(fsdt);

    double H = Vout / Vin;
    double w = 2 * M_PI / T;
    double phi = 2 * M_PI * dt / T;

    double eH = incertezza_2(Vin, Vout, eVin, eVout);
    double ew = 2 * M_PI * eT;
    double ephi = incertezza_2(dt, T, edt, T) * 2 * M_PI;

    vH.push_back(H);
    veH.push_back(eH);
    vw.push_back(w);
    vew.push_back(ew);
    vphi.push_back(phi);
    vephi.push_back(ephi);

    g.SetPoint(i, w, H);
    g.SetPointError(i, ew, eH);

    x.SetPoint(i, w, phi);
    x.SetPointError(i, ew, ephi);

    ofstream ofile; //parte per la scrittura file con i dati ottenuti
    ofile.open("dati.dat");
    for (int n = 0; n < vH.size(); n++)
    {
      ofile << vH[n] << " " << veH[n] << " " << vw[n] << " " << vew[n] << " " << vphi[n] << " " << vephi[n] << endl;
      ;
    }

    i++;
  }

  TCanvas can1("c1", "titolo1", 800, 500);
  can1.cd();
  TF1 f("f", "1/sqrt(1+pow(x/[0],2))");
  f.SetParameter(0, R * C);
  g.Fit("f");
  g.Draw("ap");
  g.SetTitle("Funzione di trasferimento vs w;w[rad/s];|H(w)|");
  can1.SetLogx();
  can1.SetLogy();
 

  TCanvas can2("can2", "titolo2", 800, 500);
  can2.cd();
  TF1 y("y", "-atan(x/[0])");
  y.SetParameter(0, R * C);
  x.Fit("y");
  x.Draw("ap");
  x.SetTitle("Fase phi vs w;w[rad/s];phi[rad]");
  can2.SetLogx();
 

  double Chi2_1 = f.GetChisquare();
  double Chi2_2 = y.GetChisquare();

  double prob1 = f.GetProb();
  double prob2 = y.GetProb();

  double w0_1 = f.GetParameter(0);
  double w0_2 = y.GetParameter(0);

  double ew0_1=f.GetParError(0);
  double ew0_2=y.GetParError(0);

  cout << "Il valore di chi^2 del fit della funzione di traferimento=" << Chi2_1 << " e la sua probabilità è:" << prob1 << endl;

  cout << "Il valore di chi^2 del fit della fase=" << Chi2_2 << " e la sua probabilità è:" << prob2 << endl;

  cout << "Il valore di w0 ottenuto dal primo fit= " << w0_1 << " +/- " << ew0_1 << endl;
  cout << "Il valore di w0 ottenuto dal secondo fit= " << w0_2 << " +/- " << ew0_2 << endl;

  if (abs(w0_1 - w0_2) < 3 * sqrt(pow(ew0_1 / w0_1, 2) + pow(ew0_2 / w0_2, 2)))
  {
    cout << "I due valori di w0 ricavati dai fit sono compatibili" << endl;
  }
  else
  {
    cout << "I due valori di w0 ricavati dai fit NON sono compatibili" << endl;
  }

  app.Run();
  return 0;
}
