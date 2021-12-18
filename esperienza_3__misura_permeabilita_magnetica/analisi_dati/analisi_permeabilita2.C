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


const double _R = 38;        // Ohm   (valore ideale 38 ohm)
const double _err_R = 0.3;
const double _C = 220e-9;    // Farad (valore ideale 220nF)
const double _err_C = 2e-9;
const double _L = 0.01003;   // Henry (valore ideale 10 mH)
const double _err_L = 0.5e-3;
const double _R_L = 3.7;     // Ohm
const double _err_R_L = stattools::max_to_stat(0.01*_R_L+0.1*4);

const double N_spire = 900;


const double mu0=4*M_PI*pow(10, -7);    // Permeabilità magnetica nel vuoto
const double l = 60.0e-3;               // Lughezza rocchetto
const double n = N_spire/l;             // parametro rocchetto
const double a_Fe = 12.1e-3;            // Lato base Al
const double a_Al = 11.9e-3;            // Lato base Fe
const double diam = 24e-3;              // Diametro rocchetto


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

struct result{
    double val, err;
};


double get_mu_R_fromLL0(double L, double L0, double a){return 1 + ((L-L0)/(mu0*n*n*l*a*a));}
double get_err_mu_R_fromLL0(double L, double L0, double a, double err_L, double err_L0, double err_a = 0.05e-3/sqrt(3)){
    return sqrt(pow(err_L*l, 2) + pow(err_L0*l, 2) + pow((L-L0)*(1e-4/sqrt(3)), 2) + pow(err_a*(L-L0)*l/a, 2))/(mu0*pow(N_spire*a, 2));}


result analisi_RLC_filter(std::string file, double* params, TCanvas* canvas, int position, double fitmin = -1, double fitmax = -1, std::string fitoption = "");

void analisi_permeabilita2(){

    gStyle->SetFrameLineWidth(0);
    gStyle->SetTextFont(43);
    gStyle->SetLineScalePS(1);

    TCanvas* c1 = new TCanvas("c1", "", 1300, 1500);
    graphset::setcanvas(c1, 2, 3);

    double A_1 = 1.2;
    double Q_1 = 6;

    double lib0[4] = {38, 4, 10e-3, 220e-9};
    double mat1[4] = {38, 4, 10e-3, 220e-9}; 
    double mat2[4] = {38, 4, 10e-3, 220e-9}; 


    result _lib0 = analisi_RLC_filter("presa_dati_libero.txt", lib0, c1, 0, 8e2, 3.5e4);
    result _mat1 = analisi_RLC_filter("presa_dati_materiale1.txt", mat1, c1, 2, 7e2, 7e3);
    result _mat2 = analisi_RLC_filter("presa_dati_materiale2.txt", mat2, c1, 4);

    std::cout << "mu_R da Fe => " << get_mu_R_fromLL0(_mat1.val, _lib0.val, a_Fe) << " +- " << get_err_mu_R_fromLL0(_mat1.val, _lib0.val, a_Fe, _mat1.err, _lib0.err) << std::endl;
    std::cout << "mu_R da Al => " << get_mu_R_fromLL0(_mat2.val, _lib0.val, a_Al) << " +- " << get_err_mu_R_fromLL0(_mat2.val, _lib0.val, a_Al, _mat2.err, _lib0.err) << std::endl;
    
    return;
}

// result
result analisi_RLC_filter(std::string file, double* params, TCanvas* canvas, int position, double fitmin = -1, double fitmax = -1, std::string fitoption = ""){

    std::string rawdata = "../dati/" + file;
    std::string output = file.substr(file.find_last_of("_"), file.find("."));

    logs::print_mmsg("Analisi di " + rawdata);

    std::ifstream data(rawdata.c_str());


    double Vin, fsVin, Vout, fsVout, T, fsT, dt, fsdt;


    // Analisi 1mo diagramma di BODE, |H(w)| su w
    canvas->cd(position+1);

    TGraphErrors* H_plot = new TGraphErrors();
    H_plot->SetName("H_plot");
    TF1* H_fit = new TF1("H_f", "1/sqrt(pow(1+[1]/[0], 2)+pow(2*[4]*x*[2]-1/(2*[4]*x*[3]), 2)/([0]*[0]))");
    H_fit->SetParameters(params[0], params[1], params[2], params[3]);
    H_fit->FixParameter(4, M_PI);


    TGraphErrors* H_resd = new TGraphErrors();
    TF1* H_res_f = new TF1("H_rf", "0", 10, 10e6);
    H_res_f->SetLineStyle(2);


    TLatex* header = new TLatex();
    header->SetTextFont(43);
    header->SetTextSize(15);

    graphset::padtypes H_pad;
    TPad* Hp1 = H_pad.Graph;
    TPad* Hp2 = H_pad.Residuals;
    graphset::setgraphsize(H_pad, true, true);


    // Analisi 2do diagramma di BODE, phi su w
    canvas->cd(position+2);

    TGraphErrors* phi_plot = new TGraphErrors();
    phi_plot->SetName("phi_plot");
    TF1* phi_fit = new TF1("phi_f", "-atan((2*[3]*x*[0]-1/(2*[3]*x*[1]))/([2]))");
    phi_fit->SetParameters(params[2], params[3], params[0]+params[1]);
    phi_fit->FixParameter(3, M_PI);

    TGraphErrors* phi_resd = new TGraphErrors();
    TF1* phi_res_f = new TF1("phi_rf", "0", 10, 10e6);
    phi_res_f->SetLineStyle(2);

    TLatex* phi_header = new TLatex();
    phi_header->SetTextFont(43);
    phi_header->SetTextSize(15);

    graphset::padtypes phi_pad;
    TPad* phi_p1 = phi_pad.Graph;
    TPad* phi_p2 = phi_pad.Residuals;
    graphset::setgraphsize(phi_pad, 1, 0);


    for(int i=0; data >> Vin >> fsVin >> Vout >> fsVout >> T >> fsT >> dt >> fsdt; i++){
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

        H_plot->SetPoint(i, 1 / T, Vout / Vin);
        H_plot->SetPointError(i, eT/pow(T, 2), get_HErr(Vin, Vout, eVin, eVout));

        phi_plot->SetPoint(i, 1 / T, 2 * M_PI * dt / T);
        phi_plot->SetPointError(i, eT/pow(T, 2), get_phiErr(T, dt, eT, edt));
    }


    // Grafico 1 Bode
    logs::print_mmsg("PRIMO DIAGRAMMA DI BODE (AMPIEZZA)");
    Hp1->cd();
    H_plot->GetXaxis()->SetLimits(450, 55e3);
    H_fit->GetXaxis()->SetLimits(450, 55e3);
    H_plot->Draw("ap");
    if(fitmin == -1){
        H_plot->Fit("H_f");
    }else{
        H_plot->Fit("H_f", "", "same", fitmin, fitmax);
    }

    std::string H_stat="#chi^{2}/ndf (prob.) = "
            +std::to_string(H_fit->GetChisquare())+"/"
            +std::to_string(H_fit->GetNDF())
            +" ("+std::to_string(H_fit->GetProb())+")";

    header->DrawLatexNDC(0.45, 0.15, ("#splitline{#it{#bf{" + rawdata + "}}}{#splitline{#it{1#circ diagramma di Bode} #bf{(A)}}{" + H_stat + "}}").c_str());

    logs::print_stat(H_fit);

    // RESIDUI
    Hp2->cd();
    graphset::fillresiduals(H_plot, H_fit, H_resd);
    H_resd->GetXaxis()->SetLimits(450, 55e3);
    H_res_f->GetXaxis()->SetLimits(450, 55e3);
    H_resd->Draw("ap");
    H_res_f->Draw("same");

    std::cout << "R   = " << H_fit->GetParameter(0) << " +- " << H_fit->GetParError(0) << std::endl
              << "R_L = " << H_fit->GetParameter(1) << " +- " << H_fit->GetParError(1) << std::endl
              << "L   = " << H_fit->GetParameter(2) << " +- " << H_fit->GetParError(2) << std::endl
              << "C   = " << H_fit->GetParameter(3) << " +- " << H_fit->GetParError(3) << std::endl;


    // Grafico 2 Bode
    logs::print_mmsg("SECONDO DIAGRAMMA DI BODE (FASE)");
    phi_p1->cd();
    phi_plot->GetXaxis()->SetLimits(450, 55e3);
    phi_fit->GetXaxis()->SetLimits(450, 55e3);
    phi_plot->Draw("ap");
    if(fitmin == -1){
        phi_plot->Fit("phi_f");
    }else{
        phi_plot->Fit("phi_f", "", "same", fitmin, fitmax);
    }

    std::string phi_stat="#chi^{2}/ndf (prob.) = "
            +std::to_string(phi_fit->GetChisquare())+"/"
            +std::to_string(phi_fit->GetNDF())
            +" ("+std::to_string(phi_fit->GetProb())+")";

    phi_header->DrawLatexNDC(0.18, 0.15, ("#splitline{#it{#bf{" + rawdata + "}}}{#splitline{#it{2#circ diagramma di Bode} #bf{(B)}}{" + phi_stat + "}}").c_str());

    logs::print_stat(phi_fit);

    // RESIDUI
    phi_p2->cd();
    graphset::fillresiduals(phi_plot, phi_fit, phi_resd);
    phi_resd->GetXaxis()->SetLimits(450, 55e3);
    phi_res_f->GetXaxis()->SetLimits(450, 55e3);
    phi_resd->Draw("ap");
    phi_res_f->Draw("same");

    
    std::cout << "R + R_L   = " << phi_fit->GetParameter(2) << " +- " << phi_fit->GetParError(2) << std::endl
              << "L   = " << phi_fit->GetParameter(0) << " +- " << phi_fit->GetParError(0) << std::endl
              << "C   = " << phi_fit->GetParameter(1) << " +- " << phi_fit->GetParError(1) << std::endl;

    std::cout << "Compatibilità L => " << stattools::compatible(phi_fit->GetParameter(0), phi_fit->GetParError(0), H_fit->GetParameter(2), H_fit->GetParError(2)) << std::endl;
    std::cout << "Bestfit L => " << stattools::getbestvalue(phi_fit->GetParameter(0), H_fit->GetParameter(2), phi_fit->GetParError(0), H_fit->GetParError(2)) << " +- " << stattools::getbestvalueerr(phi_fit->GetParError(0), H_fit->GetParError(2)) << std::endl;

    graphset::set_TGraphAxis(H_plot, "#left|H(#nu)#right| [a. u.]", 4);
    graphset::set_ResidualsAxis(H_resd, "Frequenza #nu [Hz]", 4);

    graphset::set_TGraphAxis(phi_plot, "Fase #varphi(#nu) [rad]", 4);
    graphset::set_ResidualsAxis(phi_resd, "Frequenza #nu [Hz]", 4);

    return {stattools::getbestvalue(phi_fit->GetParameter(0), H_fit->GetParameter(2), phi_fit->GetParError(0), H_fit->GetParError(2)), stattools::getbestvalueerr(phi_fit->GetParError(0), H_fit->GetParError(2))};
}


#ifndef __CINT__
int main(){
    analisi_permeabilita2();
    return 0;
}
#endif