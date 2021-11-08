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


std::string rawdata = "../dati/presa_dati_2021_10_26.txt";

const double R = 175.7; // indicativo valore nominale 200 ohm?
const double C = 100; // indicativo valore nominale 100 nF
const double L = 97.6; // indicativo valore nominale 100 mH
const double R_L = 79.1;

const double R_Hi = 1.785; // kohm

// Con i valori scelti otteniamo che circa v = 1.6 kHz
// Q circa = 5
// A = 1 (che va bene finche' R_L << R)

// funzione calcolo incertezza a partire da fondo scala (per Qualsiasi grandezza)
// tab. VALORI  | Grandezza misurata | errPercent | partitions | fondoscala (range1) 
//              | V (tensione)       | 3.5%       | 8          | variabile
//              | T (periodi)        | ?.?%       | ?          | variabile

double get_VRangeErr(double errPercent, int partitions, double range1){ 
  return errPercent * partitions *  range1;
}
double get_TRangeErr(double range1, double errPercent = 0.0016, int partition = 10){
    return range1 * errPercent * partition;
}

double getH(double vin, double vout){
    return vout / vin;
}

double get_HErr(double Vin, double Vout, double eVin, double eVout){ 
  return sqrt(pow(eVout / Vin, 2) + pow(eVin * Vout / pow(Vin, 2), 2));
}

double get_phi(double T, double dt){
    return 2 * M_PI * dt / T;
}

double get_phiErr(double T, double dt, double eT, double edt){
    return 2 * M_PI * sqrt(pow(edt/T, 2) + pow(dt * eT/(pow(T, 2)), 2));
}

void analisi_RLC_filter(){

    // todo:
    // * leggere file formato: 
    // Vin | scalaVin | Vout | scalaVout | T | scalaT | dt | scaladt
    // * trascrivere i file con gli errori:
    // Vin | eVin | Vout | eVout | T | eT | dt | edt 
    // * calcolare i valori H, eH, phi, ephi, w, ew
    // H | eH | phi | ephi | w | ew
    // H = Vin/Vout
    // phi = 2 * pi * dt / T
    // w = 2 * pi / T -> meglio forse usare v = 1 / T [Hz]?


    gStyle->SetFrameLineWidth(0);
    gStyle->SetTextFont(43);
    gStyle->SetLineScalePS(1);

    std::ifstream data(rawdata.c_str());

    std::ofstream out_rawdata("../misc/rawdata.txt"); // carbon copy of original data
    std::ofstream out_cleandata("../misc/cleandata.txt"); // values from rawdata with error
    std::ofstream out_computeddata("../misc/computeddata.txt"); // computed data for final graph

    double Vin, fsVin, Vout, fsVout, T, fsT, dt, fsdt;

    TCanvas* c1 = new TCanvas("c1", "", 600, 1000); // ! Modificare per avere grafico orizzontale piu pratico
    graphset::setcanvas(c1,1,2);

    // Analisi 1mo diagramma di BODE, |H(w)| su w
    c1->cd(1);

    TGraphErrors* H_plot = new TGraphErrors();
    H_plot->SetName("H_plot");
    TF1* H_fit = new TF1("Hf", "1/sqrt([0]+pow([1],2)*(pow(x/[2]-[2]/x, 2)))"); // ! Controllare formule
    H_fit->SetParameters(1, 5, 2000);
    // [0] = A = (1 + R_L / R)^2
    // [1] = Q = fattore di qualita = 1/(R C w_0)
    // [2] = w_0


    TGraphErrors* H_resd = new TGraphErrors();
    TF1* H_res_f = new TF1("H_rf", "0", 10, 10e6);
    H_res_f->SetLineStyle(2);

    TLatex* header = new TLatex();
    header->SetTextFont(43);
    header->SetTextSize(15);

    graphset::padtypes H;
    TPad* Hp1 = H.Graph;
    TPad* Hp2 = H.Residuals;
    graphset::setgraphsize(H, true, true);


    // Analisi 2do diagramma di BODE, phi su w
    c1->cd(2);

    TGraphErrors* phi_plot = new TGraphErrors();
    phi_plot->SetName("phi_plot");
    TF1* phi_fit = new TF1("phi_f", "-atan([1]*(x/[2]-[2]/x)/sqrt([0]))"); // ! Controllare formule
    phi_fit->SetParameters(1, 5, 2000);
    // [0] = A = (1 + R_L / R)^2
    // [1] = Q = fattore di qualita = 1/(R C w_0)
    // [2] = w_0

    TGraphErrors* phi_resd = new TGraphErrors();
    TF1* phi_res_f = new TF1("phi_rf", "0", 10, 10e6);
    phi_res_f->SetLineStyle(2);

    TLatex* phi_header = new TLatex();
    phi_header->SetTextFont(43);
    phi_header->SetTextSize(15);

    graphset::padtypes phi;
    TPad* phi_p1 = phi.Graph;
    TPad* phi_p2 = phi.Residuals;
    graphset::setgraphsize(phi, 1, 0);


    for(int i=0; data >> Vin >> fsVin >> Vout >> fsVout >> T >> fsT >> dt >> fsdt; i++){
        out_rawdata << Vin << " " << fsVin << " " << Vout << " " << fsVout << " " << T << " " << fsT << " " << dt << " " << fsdt << std::endl;
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
        double edt = stattools::max_to_stat(get_TRangeErr(fsdt));

        out_cleandata << Vin << " " << eVin << " " << Vout << " " << eVout << " " << T << " " << eT << " " << dt << " " << edt << std::endl;

        H_plot->SetPoint(i, 1 / T, Vout / Vin);
        H_plot->SetPointError(i, eT/pow(T, 2), get_HErr(Vin, Vout, eVin, eVout));

        phi_plot->SetPoint(i, 1 / T, 2 * M_PI * dt / T);
        phi_plot->SetPointError(i, eT/pow(T, 2), get_phiErr(T, dt, eT, edt));


        out_computeddata << Vout / Vin << " " << get_HErr(Vin, Vout, eVin, eVout) << " "
                         << 2 * M_PI * dt / T << " " << 2 * M_PI * sqrt(pow(edt/T, 2) + pow(dt * eT/(pow(T, 2)), 2)) << " "
                         << 1 / T << " " << eT/pow(T, 2) << std::endl;
    }
    out_rawdata << "EOF" << std::endl;
    out_cleandata << "EOF" << std::endl;
    out_computeddata << "EOF" << std::endl;


    // Grafico 1 Bode
    log::print_mmsg("PRIMO DIAGRAMMA DI BODE (AMPIEZZA)");
    Hp1->cd();
    H_plot->Draw("ap");
    H_plot->Fit("Hf");

    std::string H_stat="#chi^{2}/ndf (prob.) = "
            +std::to_string(H_fit->GetChisquare())+"/"
            +std::to_string(H_fit->GetNDF())
            +" ("+std::to_string(H_fit->GetProb())+")";

    header->DrawLatexNDC(0.35, 0.15, ("#splitline{#bf{A} #it{1#circ diagramma di Bode}}{" + H_stat + "}").c_str());

    log::print_stat(H_fit);

    // RESIDUI
    Hp2->cd();

    for(int i=0; i<H_plot->GetN(); i++){
            H_resd->SetPoint(i, H_plot->GetX()[i], (H_plot->GetY()[i] - H_fit->Eval(H_plot->GetX()[i]))/H_plot->GetEY()[i]);
            H_resd->SetPointError(i, 0, 1);
    }
    H_resd->Draw("ap");
    H_res_f->Draw("same");

    double A_amp= H_fit->GetParameter(0);
    double err_A_amp = H_fit->GetParError(0);
    double Q_amp = H_fit->GetParameter(1);
    double err_Q_amp = H_fit->GetParError(1);
    double frequenza_taglio_amp = H_fit->GetParameter(2);
    double err_frequenza_taglio_amp = H_fit->GetParError(2);

    std::cout << "A da |H(w)| = (1 + R_L / R)^2 = " << A_amp << " +/- " << err_A_amp << std::endl
            << "Fattore di Qualita' da |H(w)|, Q = " << Q_amp << " +/- " << err_Q_amp << std::endl
            << "Frequenza di Taglio da |H(w)|, v = " << frequenza_taglio_amp << " +/- " << err_frequenza_taglio_amp << " Hz" << std::endl;

    // Grafico 2 Bode
    log::print_mmsg("SECONDO DIAGRAMMA DI BODE (FASE)");
    phi_p1->cd();
    phi_plot->Draw("ap");
    phi_plot->Fit("phi_f");

    std::string phi_stat="#chi^{2}/ndf (prob.) = "
            +std::to_string(phi_fit->GetChisquare())+"/"
            +std::to_string(phi_fit->GetNDF())
            +" ("+std::to_string(phi_fit->GetProb())+")";

    phi_header->DrawLatexNDC(0.35, 0.15, ("#splitline{#bf{B} #it{2#circ diagramma di Bode}}{" + phi_stat + "}").c_str());

    log::print_stat(phi_fit);

    // RESIDUI
    phi_p2->cd();

    for(int i=0; i<phi_plot->GetN(); i++){
            phi_resd->SetPoint(i, phi_plot->GetX()[i], (phi_plot->GetY()[i] - phi_fit->Eval(phi_plot->GetX()[i]))/phi_plot->GetEY()[i]);
            phi_resd->SetPointError(i, 0, 1);
    }
    phi_resd->Draw("ap");
    phi_res_f->Draw("same");

    double A_fase = H_fit->GetParameter(0);
    double err_A_fase = H_fit->GetParError(0);
    double Q_fase = H_fit->GetParameter(1);
    double err_Q_fase = H_fit->GetParError(1);
    double frequenza_taglio_fase = H_fit->GetParameter(2);
    double err_frequenza_taglio_fase = H_fit->GetParError(2);

    std::cout << "A da phi(w) = (1 + R_L / R)^2 = " << A_fase << " +/- " << err_A_fase << std::endl
            << "Fattore di Qualita' da phi(w), Q = " << Q_fase << " +/- " << err_Q_fase << std::endl
            << "Frequenza di Taglio da phi(w), v = " << frequenza_taglio_fase << " +/- " << err_frequenza_taglio_fase << " Hz" << std::endl;

    std::cout << std::endl << "** Verifica compatibilita => (w0)" << stattools::compatible(frequenza_taglio_amp, err_frequenza_taglio_amp, frequenza_taglio_fase, err_frequenza_taglio_fase) << std::endl;


    graphset::set_TGraphAxis(H_plot, "#left|H(#nu)#right| [a. u.]");
    graphset::set_ResidualsAxis(H_resd, "Frequenza #nu [Hz]");

    graphset::set_TGraphAxis(phi_plot, "Fase #varphi(#nu) [rad]");
    graphset::set_ResidualsAxis(phi_resd, "Frequenza #nu [Hz]");

    // TODO: salvare il file come pdf

    return;
}

#ifndef __CINT__
int main(){
    analisi_RLC_filter();
    return 0;
}
#endif