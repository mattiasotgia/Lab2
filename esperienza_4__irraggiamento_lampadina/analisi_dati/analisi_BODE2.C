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

#include"../../LabTools/LabTools.h"

double get_VRangeErr(double errPercent, int partitions, double range1){return errPercent * partitions *  range1;}
double get_TRangeErr(double range1, double errPercent = 0.0016, int partition = 10){return range1 * errPercent * partition;}
double getH(double vin, double vout){return vout / vin;}
double get_HErr(double Vin, double Vout, double eVin, double eVout){ return sqrt(pow(eVout / Vin, 2) + pow(eVin * Vout / pow(Vin, 2), 2));}

void analisi_BODE2(double fitmin = -1){

    graphset::init();

    std::string file1 = "input_bode8k.txt";
    std::string rawdata1 = "../dati/" + file1;
    std::ifstream data1(rawdata1.c_str());

    std::string file2 = "input_bode80k.txt";
    std::string rawdata2 = "../dati/" + file2;
    std::ifstream data2(rawdata2.c_str());

    TCanvas* c1 = new TCanvas("c1", "", 500, 500);
    graphset::setcanvas(c1);

    TGraphErrors* H_plot1 = new TGraphErrors();
    H_plot1->SetName("H_plot1");
    TF1*          H_fit1 = new TF1("H_f1", "[0]/sqrt(1+pow(x/[1], 2))"); // ! Controllare formule
    H_fit1->SetParameters(8, 100e3);
    graphset::setmarker(H_plot1, H_fit1, kOrange-3, 25, 0.3);

    TGraphErrors* H_plot2 = new TGraphErrors();
    H_plot2->SetName("H_plot2");
    TF1*          H_fit2 = new TF1("H_f2", "[0]/sqrt(1+pow(x/[1], 2))"); // ! Controllare formule
    H_fit2->SetParameters(80, 10e3);
    graphset::setmarker(H_plot2, H_fit2, kGreen+1, 25, 0.3);

    
    TGraphErrors* H_resd1 = new TGraphErrors();
    TGraphErrors* H_resd2 = new TGraphErrors();
    TF1*          H_res_f = new TF1("H_rf", "0", 10, 10e6);
    H_res_f->SetLineStyle(2);

    graphset::padtypes H_pad;
    TPad* Hp1 = H_pad.Graph;
    TPad* Hp2 = H_pad.Residuals;
    graphset::setgraphsize(H_pad, true, true, false);

    double Vin, fsVin, Vout, fsVout, T, fsT;

    for(int i=0; data1 >> Vin >> fsVin >> Vout >> fsVout >> T >> fsT; i++){
        double eVin, eVout;
        eVin = (fsVin<=0.01)? stattools::max_to_stat(get_VRangeErr(0.045, 8, fsVin)):stattools::max_to_stat(get_VRangeErr(0.035, 8, fsVin));
        eVout = (fsVout<=0.01)? stattools::max_to_stat(get_VRangeErr(0.045, 8, fsVout)):stattools::max_to_stat(get_VRangeErr(0.035, 8, fsVout));
        double eT = stattools::max_to_stat(get_TRangeErr(fsT));


        H_plot1->SetPoint(i, 1 / T, Vout / Vin);
        H_plot1->SetPointError(i, eT/pow(T, 2), get_HErr(Vin, Vout, eVin, eVout));

    }
    for(int i=0; data2 >> Vin >> fsVin >> Vout >> fsVout >> T >> fsT; i++){
        double eVin, eVout;
        eVin = (fsVin<=0.01)? stattools::max_to_stat(get_VRangeErr(0.045, 8, fsVin)):stattools::max_to_stat(get_VRangeErr(0.035, 8, fsVin));
        eVout = (fsVout<=0.01)? stattools::max_to_stat(get_VRangeErr(0.045, 8, fsVout)):stattools::max_to_stat(get_VRangeErr(0.035, 8, fsVout));
        double eT = stattools::max_to_stat(get_TRangeErr(fsT));


        H_plot2->SetPoint(i, 1 / T, Vout / Vin);
        H_plot2->SetPointError(i, eT/pow(T, 2), get_HErr(Vin, Vout, eVin, eVout));

    }

    // Grafico 1 Bode
    log::print_mmsg("PRIMO DIAGRAMMA DI BODE (AMPIEZZA)");
    Hp1->cd();

    H_plot1->GetXaxis()->SetLimits(450, 400e3);
    H_plot1->GetYaxis()->SetRangeUser(0.85, 100);
    H_plot1->Draw("ap");
    H_plot2->Draw("p");
    c1->Modified();
    c1->Update();
    H_plot1->Fit("H_f1", "", "same");
    log::print_stat(H_fit1);
    H_plot2->Fit("H_f2", "", "same", 450, 400e3);
    log::print_stat(H_fit2);

    TLegend* l = new TLegend(0.2, 0.2, 0.9, 0.3);
    l->SetHeader("#bf{Guadagno LM741} (Input invertente)");
    l->AddEntry("H_plot1", "G = 8" , "ep");
    l->AddEntry("H_plot2", "G = 80", "ep");
    l->SetBorderSize(0);
    l->SetFillColorAlpha(kWhite, 0);
    l->Draw();


    // RESIDUI
    Hp2->cd();
    graphset::fillresiduals(H_plot1, H_fit1, H_resd1);
    graphset::fillresiduals(H_plot2, H_fit2, H_resd2);
    H_resd1->GetXaxis()->SetLimits(450, 400e3);
    H_res_f->GetXaxis()->SetLimits(450, 400e3);
    H_resd1->Draw("ap");
    H_resd2->Draw("p");
    H_res_f->Draw("same");


    graphset::set_ResidualsAxis(H_resd1, "Frequency #nu [Hz]", 1);
    graphset::set_TGraphAxis(H_plot1, "Closed-loop Gain G_{open}", 1);

    std::cout << "** Analisi R=8k :" << std::endl 
    << "freq : " << H_fit1->GetParameter(1) << " +- " << H_fit1->GetParError(1) << std::endl
    << "G    : " << H_fit1->GetParameter(0) << " +- " << H_fit1->GetParError(0) << std::endl << std::endl;

    std::cout << "** Analisi R=80k :" << std::endl 
    << "freq : " << H_fit2->GetParameter(1) << " +- " << H_fit2->GetParError(1) << std::endl
    << "G    : " << H_fit2->GetParameter(0) << " +- " << H_fit2->GetParError(0) << std::endl << std::endl;

    c1->SaveAs("../fig/plot_combined.pdf");

    return;
}

#ifndef __CINT__
int main(){
    analisi_BODE2(-1);
    return 0;
}
#endif