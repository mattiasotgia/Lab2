#include<vector>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>

#include<TCanvas.h>
#include<TGraphErrors.h>
#include<TF1.h>
#include<TStyle.h>
#include<TAxis.h>
#include<TMath.h>
#include<TLatex.h>
#include<TLegend.h>

#include"LabTools.h"

double get_VRangeErr(double errPercent, int partitions, double range1){return errPercent * partitions *  range1;}
double get_TRangeErr(double range1, double errPercent = 0.0016, int partition = 10){return range1 * errPercent * partition;}
double getH(double vin, double vout){return vout / vin;}
double get_HErr(double Vin, double Vout, double eVin, double eVout){ return sqrt(pow(eVout / Vin, 2) + pow(eVin * Vout / pow(Vin, 2), 2));}

void analisi_BODE(double fitmin = -1){

    graphset::init();
    gStyle->SetFrameLineWidth(0);

    std::string file;
    std::cout << "File input : " << std::flush;
    std::cin >> file;
    std::string rawdata = "../dati/" + file;
    std::string output = file.substr(0, file.find("."));
    std::ifstream data(rawdata.c_str());

    double guadagno;
    std::cout << "Guadagno : " << std::flush;
    std::cin >> guadagno;

    double freq;
    std::cout << "Fraquenza : " << std::flush;
    std::cin >> freq;

    TCanvas* c1 = new TCanvas("c1", "", 600, 625);
    graphset::setcanvas(c1);

    double Vin, fsVin, Vout, fsVout, T, fsT;

    std::ofstream out_rawdata(("../misc/rawdata_" + output + ".txt").c_str()); // carbon copy of original data
    std::ofstream out_cleandata(("../misc/cleandata_" + output + ".txt").c_str()); // values from rawdata with error
    std::ofstream out_computeddata(("../misc/computeddata_" + output + ".txt").c_str()); // computed data for final graph

    TGraphErrors* H_plot = new TGraphErrors();
    H_plot->SetName("H_plot");
    TF1* H_fit = new TF1("H_f", "[0]/sqrt(1+pow(x/[1], 2))"); // ! Controllare formule
    H_fit->SetParameters(guadagno, freq);
    // H_fit->SetParLimits(0, 0, 10000);
    // H_fit->SetParLimits(1, 0, 10000);
    // H_fit->SetParLimits(2, 0, 10000);
    // [0] -> A = (1 + R_L / R)^2
    // [1] -> Q^2 = fattore di qualita = (1/(R C w_0))^2
    // [2] -> v0

    TGraphErrors* H_resd = new TGraphErrors();
    TF1* H_res_f = new TF1("H_rf", "0", 10, 10e6);
    H_res_f->SetLineStyle(2);

    graphset::padtypes H_pad;
    TPad* Hp1 = H_pad.Graph;
    TPad* Hp2 = H_pad.Residuals;
    graphset::setgraphsize(H_pad, true, true);
    H_plot->GetYaxis()->SetMoreLogLabels(kTRUE);

    for(int i=0; data >> Vin >> fsVin >> Vout >> fsVout >> T >> fsT; i++){
        out_rawdata << Vin << " & " << fsVin << " & " << Vout << " & " << fsVout << " & " << T << " & " << fsT << " \\\\ " << std::endl;
        double eVin, eVout;
        if(fsVin<=0.01){
            eVin = stattools::max_to_stat(get_VRangeErr(0.045, 8, fsVin));
        }else{
            eVin = stattools::max_to_stat(get_VRangeErr(0.035, 8, fsVin));
        }
        if(fsVout<=0.01){
            eVout = stattools::max_to_stat(get_VRangeErr(0.045, 8, fsVout));
        }else{
            eVout = stattools::max_to_stat(get_VRangeErr(0.035, 8, fsVout));
        }
        double eT = stattools::max_to_stat(get_TRangeErr(fsT));


        out_cleandata << Vin << " " << eVin << " " << Vout << " " << eVout << " " << T << " " << eT << std::endl;

        H_plot->SetPoint(i, 1 / T, Vout / Vin);
        H_plot->SetPointError(i, eT/pow(T, 2), get_HErr(Vin, Vout, eVin, eVout));

        out_computeddata << "\\num{" << Vout / Vin << " +- " << get_HErr(Vin, Vout, eVin, eVout) << "} "
                       << "& \\num{" << 1 / T << " +- " << eT/pow(T, 2) << "} \\\\" << std::endl;
    }
    out_rawdata << "EOF" << std::endl;
    out_cleandata << "EOF" << std::endl;
    out_computeddata << "EOF" << std::endl;

    // Grafico 1 Bode
    logs::print_mmsg("PRIMO DIAGRAMMA DI BODE (AMPIEZZA)");
    Hp1->cd();
    // H_plot->GetXaxis()->SetLimits(450, 55e3);
    // H_fit->GetXaxis()->SetLimits(450, 55e3);
    H_plot->Draw("ap");
    if(fitmin == -1){
        H_plot->Fit("H_f");
    }else{
        H_plot->Fit("H_f", "", "same");
    }

    TLatex* text = new TLatex();
    text->SetTextFont(43);
    text->SetTextSize(20);
    text->DrawLatexNDC(0.20, 0.15, (Form("#splitline{#splitline{#bf{GBW (Gain-Bandwidth)}}{Amplificatore per strumentazione}}{#chi^{2}/ndf (prob.) = %.2f/%d (%.2f)}", H_fit->GetChisquare(), H_fit->GetNDF(), H_fit->GetProb())));

    logs::print_stat(H_fit);

    // RESIDUI
    Hp2->cd();
    graphset::fillresiduals(H_plot, H_fit, H_resd);
    // H_resd->GetXaxis()->SetLimits(450, 55e3);
    // H_res_f->GetXaxis()->SetLimits(450, 55e3);
    H_resd->Draw("ap");
    H_res_f->Draw("same");

    graphset::set_ResidualsAxis(H_resd, "Frequency #nu [Hz]", 1);
    graphset::set_TGraphAxis(H_plot, "Gain |H[#nu]|", 1);

    c1->SaveAs(("../fig/plot_" + output + ".pdf").c_str());

    return;
}

#ifndef __CLING__
int main(){
    analisi_BODE(-1);
    return 0;
}
#endif