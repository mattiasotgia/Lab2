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

const double offset = -3e-3;
const double err_off = -2.6 / (2 * sqrt(3));

double get_VRangeErr(double errPercent, int partitions, double range1) { return errPercent * partitions * range1; }

void guadagnoDIFF(){    
    graphset::init();
    gStyle->SetFrameLineWidth(0);

    string nomefile = "guadagnoDIFF.txt";

skip_test:
    double vin, vout, fsvin, fsvout;

    TCanvas* c1 = new TCanvas("", "", 600, 500);
    graphset::setcanvas(c1);

    TGraphErrors *g = new TGraphErrors();
    TF1 *f = new TF1("fit", "[0]+[1]*x");
    f->SetParameter(0, 0);
    f->SetParameter(1, 80);
    f->SetParName(0, "q");
    f->SetParName(1, "G");

    ifstream file(("../dati/" + nomefile).c_str());

    for(int i=0; file >> vin >> fsvin >> vout >> fsvout; i++)
    {
        double err_vin = get_VRangeErr(0.035, 8, fsvin);
        double err_vout = get_VRangeErr(0.035, 8, fsvout);

        g->SetPoint(i, vin, vout);
        g->SetPointError(i, err_vin, err_vout);
    }

    g->Draw("ap");
    g->Fit("fit");

    TLatex* text = new TLatex();
    text->SetTextFont(43);
    text->SetTextSize(20);
    text->DrawLatexNDC(0.20, 0.85, (Form("#splitline{#bf{GUADAGNO} Differenziale}{#chi^{2}/ndf (prob.) = %.2f/%d (%.3f)}", f->GetChisquare(), f->GetNDF(), f->GetProb())));

    double G = f->GetParameter(1);
    double quota = f->GetParameter(0);
    double e_quota = f->GetParError(0);
    double e_G = f->GetParError(1);

    graphset::set_TGraphAxis(g, "Tensione uscita V_{out} [V]", 1.5, "Tensione ingresso V_{in} [V]");

    logs::print_stat(f);
    std::cout << "G:" << G << "+-" << e_G << std::endl;
    std::cout << "q:" << quota << "+-" << e_quota << std::endl;

        c1->SaveAs("../fig/GuadagnoDIFF.pdf");
}