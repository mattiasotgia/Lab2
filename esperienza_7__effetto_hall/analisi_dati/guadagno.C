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

double get_VRangeErr(double errPercent, int partitions, double range1) { return errPercent * partitions * range1; }

void guadagno()
{

    graphset::init();
    gStyle->SetFrameLineWidth(0);

    std::string nomefile;

    std::cout << "nome file input" << std::endl;
    std::cin >> nomefile;
    std::string output = nomefile.substr(nomefile.find_last_of("_"), nomefile.find(".")-nomefile.find_last_of("_"));

    double vin, vout, fsvin, fsvout;
    const double errPercent = 0.035;
    TCanvas* c1 = new TCanvas("", "", 600, 500);
    graphset::setcanvas(c1);

    TGraphErrors*  g=new TGraphErrors();
    TF1* f= new TF1("fit", "[0]+[1]*x");
    int i = 0;

    ifstream file(("../dati/" + nomefile).c_str());

    while (file >> vin >> fsvin >> vout >> fsvout)
    {
        double err_vin = get_VRangeErr(errPercent, 8, fsvin);
        double err_vout = get_VRangeErr(errPercent, 8, fsvout);

        g->SetPoint(i, vin, vout);
        g->SetPointError(i, err_vin, err_vout
        i++;
    }

    g->Draw("ap");
    f->SetParameter(0, 0);
    f->SetParameter(1, 80);
    f->SetParName(0,"quota");
    f->SetParName(1,"G");

    g->Fit("fit");

    std::cout << Form("%s \n", output.c_str());

    TLatex* text = new TLatex();
    text->SetTextFont(43);
    text->SetTextSize(20);
    text->DrawLatexNDC(0.20, 0.85, (Form("#splitline{#bf{GUADAGNO} Configurazione %s}{#chi^{2}/ndf (prob.) = %.2f/%d (%.3f)}", (output=="_invertente"? "Invertente":"Non-Invertente"), f->GetChisquare(), f->GetNDF(), f->GetProb())));



    double G = f->GetParameter(1);
    double quota = f->GetParameter(0);

    double e_quota = f->GetParError(0);
    double e_G = f->GetParError(1);

    double chi2 = f->GetChisquare();
    double prob = f->GetProb();
    double ndf = f->GetNDF();

    graphset::set_TGraphAxis(g, "Tensione uscita V_{out} [V]", 1.5, "Tensione ingresso V_{in} [V]");

    std::cout << "G: " << G << " +- " << e_G << std::endl;
    std::cout << "q: " << quota << " +- " << e_quota << std::endl;
    logs::print_stat(f);

    c1->SaveAs(("../fig/Guadagno_" + output + ".pdf").c_str());
}