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

#include "LabTools.h"

std::string data = "../dati/generatore.txt";
std::string fig_name = "../fig/generatore.pdf";

double get_VRangeErr(double errPercent, int partitions, double range1) { return errPercent * partitions * range1; }

double get_IRangeErr(double meas) { return meas * 0.000001 * 500 + 0.000001 * 400 * 0.1; } // verificare se la misura della corrente è in A o mA !!!

void generatore()
{

    ifstream file(data.c_str());

    const double errPercent = 0.035;

    double V, meas, range1;

    TCanvas *c1 = new TCanvas("", "", 600, 500);
    TGraphErrors *g = new TGraphErrors();
    TF1 *f = new TF1("f", "[0]+[1]*x");

    int i = 0;

    while (file >> V >> range1 >> meas)
    {

        g->SetPoint(i, V, meas);
        g->SetPointError(i, get_VRangeErr(errPercent, 8, range1), get_IRangeErr(meas));

        i++;
    }

    g->Draw("ap");

    f->SetParameters(0, 1 / 500);

    g->Fit("f");

    f->SetParName(0, "quota");
    f->SetParName(1, "k");

    double k = f->GetParameter(1);
    double quota = f->GetParameter(0);
    double e_quota = f->GetParError(0);
    double e_k = f->GetParError(1);

    double chi2 = f->GetChisquare();
    double prob = f->GetProb();
    double ndf = f->GetNDF();

    std::cout << "k [1]: " << k << " +- " << e_k << std::endl;
    std::cout << "quota [0]: " << quota << " +- " << e_quota << std::endl;

    
    logs::print_stat(f);

    c1->SaveAs(fig_name.c_str());
}