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
const double _C = 220e-9;    // Farad (valore ideale 220nF)
const double _L = 0.01003;   // Henry (valore ideale 10 mH)
const double _R_L = 3.7;     // Ohm

const double N_spire = 900;


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

struct parameter{
    double val[2];
    double err[2];
};

struct result
{
    parameter A, Q, v0;
    
};

struct parameter_fit{
    double val, err;
};

struct result_fit{
    parameter_fit A, Q, v0;
};

struct result_circ{
    parameter_fit R, R_L, L, C;
};

result analisi_RLC_filter(std::string file, double* params, TCanvas* canvas, int position);

double getbestvalue(double G1, double G2, double errG1, double errG2){
    return (G1/pow(errG1, 2)+G2/pow(errG2, 2))/(1/pow(errG1, 2)+1/pow(errG2, 2));
}

double getbestvalueerr(double errG1, double errG2){
    return 1/(1/pow(errG1, 2)+1/pow(errG2, 2));
}

result_fit getresult(result_fit r){

    double R, err_R, R_L, err_R_L, L, err_L, C, err_C;

    R_L = _R_L;

    R = R_L/(sqrt(r.A.val)+1);
    L = R * r.Q.val / (2*M_PI*r.v0.val);
    C = 1/(2*M_PI*r.v0.val*R*r.Q.val);

    err_R = sqrt(pow(err_R_L/((sqrt(r.A.val)+1)), 2) + pow(R_L*r.A.err/(4*r.A.val*pow((sqrt(r.A.val)+1,2))), 2));
    // err_L = 
    // err_C = 

    return {{R, err_R}, {R_L, err_R_L}, {L, err_L}, {C, err_C}};
}

void analisi_permeabilita(){

    gStyle->SetFrameLineWidth(0);
    gStyle->SetTextFont(43);
    gStyle->SetLineScalePS(1);

    TCanvas* c1 = new TCanvas("c1", "", 1300, 1500);
    graphset::setcanvas(c1, 2, 3);

    double A_1 = 1.2;
    double Q_1 = 6;

    double lib0[3] = {A_1, 3, 3400};
    double mat1[3] = {A_1, Q_1*1.5, 2311};
    double mat2[3] = {A_1, Q_1, 3562};

    result libero = analisi_RLC_filter("presa_dati_libero.txt", lib0, c1, 0);
    result materiale1 = analisi_RLC_filter("presa_dati_materiale1.txt", mat1, c1, 2);
    result materiale2 = analisi_RLC_filter("presa_dati_materiale2.txt", mat2, c1, 4);
    c1->SaveAs("../fig/plot.pdf");
    std::ofstream output("../misc/output.csv");
    output << "materiale, A_amp, err_A_amp, A_fase, err_A_fase, Q_amp, err_Q_amp, Q_fase, err_Q_fase, v0_amp, err_v0_amp, v0_fase, err_v0_fase" << std::endl;
    output << "libero, " << libero.A.val[0]  << ", " << libero.A.err[0]  << ", " << libero.A.val[1]  << ", " << libero.A.err[1]  << ", "
                         << libero.Q.val[0]  << ", " << libero.Q.err[0]  << ", " << libero.Q.val[1]  << ", " << libero.Q.err[1]  << ", "
                         << libero.v0.val[0] << ", " << libero.v0.err[0] << ", " << libero.v0.val[1] << ", " << libero.v0.err[1] << std::endl;
    
    output << "materiale1, " << materiale1.A.val[0]  << ", " << materiale1.A.err[0]  << ", " << materiale1.A.val[1]  << ", " << materiale1.A.err[1]  << ", "
                             << materiale1.Q.val[0]  << ", " << materiale1.Q.err[0]  << ", " << materiale1.Q.val[1]  << ", " << materiale1.Q.err[1]  << ", "
                             << materiale1.v0.val[0] << ", " << materiale1.v0.err[0] << ", " << materiale1.v0.val[1] << ", " << materiale1.v0.err[1] << std::endl;
    
    output << "materiale2, " << materiale2.A.val[0]  << ", " << materiale2.A.err[0]  << ", " << materiale2.A.val[1]  << ", " << materiale2.A.err[1]  << ", "
                             << materiale2.Q.val[0]  << ", " << materiale2.Q.err[0]  << ", " << materiale2.Q.val[1]  << ", " << materiale2.Q.err[1]  << ", "
                             << materiale2.v0.val[0] << ", " << materiale2.v0.err[0] << ", " << materiale2.v0.val[1] << ", " << materiale2.v0.err[1] << std::endl;
    return;
}

result analisi_RLC_filter(std::string file, double* params, TCanvas* canvas, int position){

    std::string rawdata = "../dati/" + file;
    std::string output = file.substr(file.find_last_of("_"), file.find("."));

    log::print_mmsg("Analisi di " + rawdata);

    std::ifstream data(rawdata.c_str());

    std::ofstream out_rawdata(("../misc/rawdata_" + output + ".txt").c_str()); // carbon copy of original data
    std::ofstream out_cleandata(("../misc/cleandata_" + output + ".txt").c_str()); // values from rawdata with error
    std::ofstream out_computeddata(("../misc/computeddata_" + output + ".txt").c_str()); // computed data for final graph

    double Vin, fsVin, Vout, fsVout, T, fsT, dt, fsdt;


    // Analisi 1mo diagramma di BODE, |H(w)| su w
    canvas->cd(position+1);

    TGraphErrors* H_plot = new TGraphErrors();
    H_plot->SetName("H_plot");
    TF1* H_fit = new TF1("Hf", "1/sqrt([0]+[1]*(pow(x/[2]-[2]/x, 2)))"); // ! Controllare formule
    H_fit->SetParameters(params[0], params[1]*params[1], params[2]);
    // H_fit->SetParameter(1, params[1]*params[1]);
    // H_fit->SetParameter(2, params[2]);
    // H_fit->FixParameter(0, params[0]);
    // H_fit->SetParLimits(0, 0, 4);
    H_fit->SetParLimits(1, 0, 100);
    // [0] = A = (1 + R_L / R)^2
    // [1] = Q^2 = fattore di qualita = (1/(R C w_0))^2
    // [2] = w_0


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
    TF1* phi_fit = new TF1("phi_f", "-atan([1]*(x/[2]-[2]/x)/[0])"); // ! Controllare formule
    phi_fit->SetParameters(sqrt(params[0]),params[1], params[2]);
    // phi_fit->SetParameter(1, params[1]);
    // phi_fit->SetParameter(2, params[2]);
    // phi_fit->FixParameter(0, sqrt(params[0]));
    // phi_fit->SetParLimits(0, 0, 4);
    phi_fit->SetParLimits(1, 0, 10);
    // [0] = sqrt(A) = (1 + R_L / R)
    // [1] = Q = fattore di qualita = 1/(R C w_0)
    // [2] = w_0

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

    header->DrawLatexNDC(0.45, 0.15, ("#splitline{#it{#bf{" + rawdata + "}}}{#splitline{#bf{A} #it{1#circ diagramma di Bode}}{" + H_stat + "}}").c_str());

    log::print_stat(H_fit);

    // RESIDUI
    Hp2->cd();
    graphset::fillresiduals(H_plot, H_fit, H_resd);
    H_resd->Draw("ap");
    H_res_f->Draw("same");

    double A_amp= H_fit->GetParameter(0);
    double err_A_amp = H_fit->GetParError(0);
    double Q_amp = sqrt(H_fit->GetParameter(1));
    double err_Q_amp = 1/(2*Q_amp)*H_fit->GetParError(1);
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

    phi_header->DrawLatexNDC(0.18, 0.15, ("#splitline{#it{#bf{" + rawdata + "}}}{#splitline{#bf{B} #it{2#circ diagramma di Bode}}{" + phi_stat + "}}").c_str());

    log::print_stat(phi_fit);

    // RESIDUI
    phi_p2->cd();
    graphset::fillresiduals(phi_plot, phi_fit, phi_resd);
    phi_resd->Draw("ap");
    phi_res_f->Draw("same");

    double A_fase = pow(phi_fit->GetParameter(0), 2);
    double err_A_fase = 2*phi_fit->GetParameter(0)*phi_fit->GetParError(0);
    double Q_fase = phi_fit->GetParameter(1);
    double err_Q_fase = phi_fit->GetParError(1);
    double frequenza_taglio_fase = phi_fit->GetParameter(2);
    double err_frequenza_taglio_fase = phi_fit->GetParError(2);

    std::cout << "A da phi(w) = (1 + R_L / R)^2 = " << A_fase << " +/- " << err_A_fase << std::endl
            << "Fattore di Qualita' da phi(w), Q = " << Q_fase << " +/- " << err_Q_fase << std::endl
            << "Frequenza di Taglio da phi(w), v = " << frequenza_taglio_fase << " +/- " << err_frequenza_taglio_fase << " Hz" << std::endl;

    std::cout << std::endl << "** Verifica compatibilita =>  (A)" << stattools::compatible(A_amp, err_A_amp, A_fase, err_A_fase) << std::endl;
    std::cout              << "** Verifica compatibilita =>  (Q)" << stattools::compatible(Q_amp, err_Q_amp, Q_fase, err_Q_fase) << std::endl;
    std::cout              << "** Verifica compatibilita => (v0)" << stattools::compatible(frequenza_taglio_amp, err_frequenza_taglio_amp, frequenza_taglio_fase, err_frequenza_taglio_fase) << std::endl;


    graphset::set_TGraphAxis(H_plot, "#left|H(#nu)#right| [a. u.]", 4);
    graphset::set_ResidualsAxis(H_resd, "Frequenza #nu [Hz]", 4);

    graphset::set_TGraphAxis(phi_plot, "Fase #varphi(#nu) [rad]", 4);
    graphset::set_ResidualsAxis(phi_resd, "Frequenza #nu [Hz]", 4);

    return {{{A_amp, A_fase}, {err_A_amp, err_A_fase}}, {{Q_amp, Q_fase}, {err_Q_amp, err_Q_fase}}, {{frequenza_taglio_amp, frequenza_taglio_fase}, {err_frequenza_taglio_amp, err_frequenza_taglio_fase}}};
}


#ifndef __CINT__
int main(){
    analisi_permeabilita();
    return 0;
}
#endif