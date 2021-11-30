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

double get_VRangeErr(double errPercent, int partitions, double range1) { return errPercent * partitions * range1; }

void guadagno()
{

    string nomefile;

    std::cout << "nome file input" << std::endl;
    std::cin >> nomefile >> std::endl;

    double vin, vout, fsvin, fsvout;
    const double errPercent = 0.035;

    TGraphErrors*  g=new TGraphErrors();
    TF1* f= new TF1("fit", "[0]+[1]*x");
    int i = 0;

    while (file >> vin >> fsvin >> vout >> fsvout)
    {
        double err_vin = get_VRangeErr(errPercent, 8, fsvin);
        double err_vout = get_VRangeErr(errPercent, 8, fsvout);

        g->SetPoint(i, vin, vout);
        g->SetPointError(i, err_vin, err_vout);
        i++;
    }

    g->Draw("ap");
    f->SetParameter(0, 0);
    f->SetParameter(1, 80);

    g->Fit("fit");

    double G = f_>GetParameter(1);
    double quota = f.GetParameter(0);

    double e_quota = f->GetParError(0);
    double e_G = f->GetParError(1);

    double chi2 = f->GetChisquare();
    double prob = f->GetProb();
    double ndf = f->GetNDF();

    std::cout << "il valore del guadagno è:" << G << "+/-" << e_G << std::endl;
    std::cout << "il valore della quota è:" << quota << "+/-" << e_quota << std::endl;
    log::print_stat(f);
}