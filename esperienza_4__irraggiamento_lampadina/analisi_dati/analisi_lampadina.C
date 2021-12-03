#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TMath.h>
#include <TLatex.h>
#include <TLegend.h>

#include "../../LabTools/LabTools.h"

//double get_VRangeErr(double errPercent, int partitions, double range1) { return errPercent * partitions * range1; }

void analisi_lampadina()
{

    string nomefile;

    std::cout << "nome file input" << std::endl;
    std::cin >> nomefile;

    double t, vin, e_vin, range;
    const double s_vin = 0.01;
    // const double errPercent = 0.035;

    TGraphErrors *g = new TGraphErrors();
    //TF1* f= new TF1("fit", "[0]+[1]*x");

    int i = 0;

    ifstream file(("../dati/" + nomefile).c_str());

    /* if(! file.good() ){
        std::cout<<"errore lettura file"<<std::endl;
   */

    while (file >> t >> vin >> e_vin >> range)
    {

        g->SetPoint(i, t, vin);
        g->SetPointError(i, 0, e_vin);
        i++;
    }

    g->Draw("ap");

    g->SetTitle("V_in vs t;tempo[s];V_in[V]");
}