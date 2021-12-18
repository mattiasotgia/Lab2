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
#include"ErrorAnalysis.h"


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

double get_VRangeErr(double errPercent, int partitions, double range1){return errPercent * partitions *  range1;}
double get_TRangeErr(double range1, double errPercent = 0.0016, int partition = 10){return range1 * errPercent * partition;}
double getH(double vin, double vout){return vout / vin;}
double get_HErr(double Vin, double Vout, double eVin, double eVout){ return sqrt(pow(eVout / Vin, 2) + pow(eVin * Vout / pow(Vin, 2), 2));}
double get_phi(double T, double dt){return 2 * M_PI * dt / T;}
double get_phiErr(double T, double dt, double eT, double edt){return 2 * M_PI * sqrt(pow(edt/T, 2) + pow(dt * eT/(pow(T, 2)), 2));}

struct parameter{double val, err;};
struct result{parameter A_amp, Q_amp, v0_amp, Q_sqrtA_fase, v0_fase;};
struct result_circ{parameter R, R_L, L, C;};

result analisi_RLC_filter(std::string file, double* params, TCanvas* canvas, int position, double fitmin = -1, double fitmax = -1, std::string fitoption = "");

result_circ get_result(result r){

    double R, err_R, R_L, err_R_L, L, err_L, C, err_C;

    R_L = _R_L;
    err_R_L = _err_R_L;

    R = R_L/(sqrt(r.A_amp.val)-1);
    L = (R+R_L) * r.Q_amp.val / (2*M_PI*r.v0_amp.val);
    C = 1/(2*M_PI*r.v0_amp.val*(R+R_L)*r.Q_amp.val);

    err_R = sqrt(pow(err_R_L/((sqrt(r.A_amp.val)-1)), 2) + pow(R_L*r.A_amp.err / (2*sqrt(r.A_amp.val)*pow((sqrt(r.A_amp.val)-1, 2), 2)), 2));
    err_L = sqrt(pow(r.Q_amp.val*err_R, 2) + pow(R*r.Q_amp.err, 2) + pow(R*r.Q_amp.val*r.v0_amp.err/r.v0_amp.val, 2))/(2*M_PI*r.v0_amp.val);
    err_C = sqrt(pow(R*r.Q_amp.val*r.v0_amp.err, 2) + pow(r.Q_amp.val*r.v0_amp.val*err_R, 2) + pow(R*r.v0_amp.val*r.Q_amp.err, 2))/(2*M_PI*pow(r.v0_amp.val*r.Q_amp.val*R, 2));

    return {{R, err_R}, {R_L, err_R_L}, {L, err_L}, {C, err_C}};
}

double get_L_fromCv0(double v0){return 1/(_C*pow(2*M_PI*v0, 2));}
double get_err_L_fromCv0(double v0, double err_v0){return sqrt(pow(_err_C/_C, 2) + pow(err_v0/v0, 2))/(pow(2*M_PI*v0, 2)*_C);}

double get_mu_R_fromLL0(double L, double L0, double a){return 1 + ((L-L0)/(mu0*n*n*l*a*a));}
double get_err_mu_R_fromLL0(double L, double L0, double a, double err_L, double err_L0, double err_a = 0.05e-3/sqrt(3)){
    return sqrt(pow(err_L*l, 2) + pow(err_L0*l, 2) + pow((L-L0)*(1e-4/sqrt(3)), 2) + pow(err_a*(L-L0)*l/a, 2))/(mu0*pow(N_spire*a, 2));}

void analisi_permeabilita(){

    gInterpreter->AddIncludePath("~/Documents/CustomLibs/ErrorAnalysis/");
    gSystem->Load("~/Documents/CustomLibs/libErrorAnalysis.dylib");

    gStyle->SetFrameLineWidth(0);
    gStyle->SetTextFont(43);
    gStyle->SetLineScalePS(1);

    TCanvas* c1 = new TCanvas("c1", "", 1300, 1500);
    graphset::setcanvas(c1, 2, 3);

    double A_1 = 1;

    double lib0[3] = {A_1, 5, 3330};
    double mat1[3] = {A_1, 7, 2311};
    double mat2[3] = {A_1, 5, 3562};

    result libero = analisi_RLC_filter("presa_dati_libero.txt", lib0, c1, 0, 8e2, 3.5e4);
    result m1 = analisi_RLC_filter("presa_dati_materiale1.txt", mat1, c1, 2, 7e2, 7e3);
    result m2 = analisi_RLC_filter("presa_dati_materiale2.txt", mat2, c1, 4);
    
    // Calcoli successivi
    // Calcoli per libero--------------------------------------------------------------------------------------------------------------------------------------
    logs::print_mmsg("Analisi da libero, ampiezza, valori dal FIT");

    result_circ libero_circ = get_result(libero);
    std::cout << "R = " << libero_circ.R.val << " +- " << libero_circ.R.err << " ohm" << std::endl
              << "L = " << libero_circ.L.val << " +- " << libero_circ.L.err << " H" << std::endl
              << "C = " << libero_circ.C.val << " +- " << libero_circ.C.err << " F" << std::endl;

    const double L_0_fit = libero_circ.L.val;
    const double err_L_0_fit = libero_circ.L.err;

    double bestfit_v0_lib = stattools::getbestvalue(libero.v0_amp.val, libero.v0_fase.val, libero.v0_amp.err, libero.v0_fase.err);
    double err_bestfit_v0_lib = stattools::getbestvalueerr(libero.v0_amp.err, libero.v0_fase.err);

    std::cout << "Miglior stima v0 (da libero) = " << bestfit_v0_lib << " +- " << err_bestfit_v0_lib << "Hz" << std::endl;

    double L_0 = get_L_fromCv0(bestfit_v0_lib); double err_L_0 = get_err_L_fromCv0(bestfit_v0_lib, err_bestfit_v0_lib);
    std::cout << "L (noto C = " << _C << ", v0 = " << bestfit_v0_lib << ", miglior stima)  = " << L_0 << " +- " << err_L_0 << " H" 
              << " => " << stattools::compatible(L_0, err_L_0, _L, _err_L) << std::endl;


    // calcoli per m1------------------------------------------------------------------------------------------------------------------------------------------
    logs::print_mmsg("Valore da m1, ampiezza, valori dal FIT");
    
    result_circ m1_circ_amp = get_result(m1);
    std::cout << "R = " << m1_circ_amp.R.val << " +- " << m1_circ_amp.R.err << " ohm" << std::endl
              << "L = " << m1_circ_amp.L.val << " +- " << m1_circ_amp.L.err << " H" << std::endl
              << "C = " << m1_circ_amp.C.val << " +- " << m1_circ_amp.C.err << " F" << std::endl;
    
    double mu_R_m1_amp_fit = (m1_circ_amp.L.val-(libero_circ.L.val))/(mu0*n*n*l*a_Fe*a_Fe)+1;
    double err_mu_R_m1_amp_fit = sqrt(pow(m1_circ_amp.L.err/m1_circ_amp.L.val, 2) + pow(libero_circ.L.err/libero_circ.L.val, 2) + pow(0.05e-3/(sqrt(3)*l), 2) + pow(0.05e-3/(sqrt(3)*a_Fe), 2));

    std::cout << "mu_R per Fe (da amp, valori FIT) => " << mu_R_m1_amp_fit << " +- " << err_mu_R_m1_amp_fit << std::endl << std::endl;

    double L_m1_v0amp = get_L_fromCv0(m1.v0_amp.val); double err_L_m1_v0amp = get_err_L_fromCv0(m1.v0_amp.val, m1.v0_amp.err);
    double L_m1_v0fase = get_L_fromCv0(m1.v0_fase.val); double err_L_m1_v0fase = get_err_L_fromCv0(m1.v0_fase.val, m1.v0_fase.err);

    std::cout << "L (noto C = " << _C << ", v0 = " << m1.v0_amp.val << ", da amp)  = " << L_m1_v0amp << " +- " << err_L_m1_v0amp << " H" << std::endl;
    std::cout << "L (noto C = " << _C << ", v0 = " << m1.v0_fase.val << ", da fase) = " << L_m1_v0fase << " +- " << err_L_m1_v0fase << " H" << std::endl;

    double mu_R_m1_amp = get_mu_R_fromLL0(L_m1_v0amp, L_0, a_Fe); double err_mu_R_m1_amp = get_err_mu_R_fromLL0(L_m1_v0amp, L_0, a_Fe, err_L_m1_v0amp, err_L_0);
    double mu_R_m1_fase = get_mu_R_fromLL0(L_m1_v0fase, L_0, a_Fe); double err_mu_R_m1_fase = get_err_mu_R_fromLL0(L_m1_v0fase, L_0, a_Fe, err_L_m1_v0fase, err_L_0);

    std::cout << "mu_R per Fe (da L e L0, da amp)  => " << mu_R_m1_amp << " +- " << err_mu_R_m1_amp << std::endl;
    std::cout << "mu_R per Fe (da L e L0, da fase) => " << mu_R_m1_fase << " +- " << err_mu_R_m1_fase << std::endl;


    // Calcoli per m2------------------------------------------------------------------------------------------------------------------------------------------
    logs::print_mmsg("Valore da m2");
    
    result_circ m2_circ = get_result(m2);
    std::cout << "R = " << m2_circ.R.val << " +- " << m2_circ.R.err << " ohm" << std::endl
              << "L = " << m2_circ.L.val << " +- " << m2_circ.L.err << " H" << std::endl
              << "C = " << m2_circ.C.val << " +- " << m2_circ.C.err << " F" << std::endl;
    
    double mu_R_m2_fit = (m2_circ.L.val-(libero_circ.L.val))/(mu0*n*n*l*a_Al*a_Al)+1;
    double err_mu_R_m2_fit = sqrt(pow(m2_circ.L.err/m2_circ.L.val, 2) + pow(libero_circ.L.err/libero_circ.L.val, 2) + pow(0.05e-3/(sqrt(3)*l), 2) + pow(0.05e-3/(sqrt(3)*a_Al), 2));

    std::cout << "mu_R per Al (da amp, valori FIT) => " << mu_R_m2_fit << " +- " << err_mu_R_m2_fit << std::endl;

    double bestfit_v0_m2 = stattools::getbestvalue(m2.v0_amp.val, m2.v0_fase.val, m2.v0_amp.err, m2.v0_fase.err);
    double err_bestfit_v0_m2 = stattools::getbestvalueerr(m2.v0_amp.err, m2.v0_fase.err);

    std::cout << "Miglior stima v0 (da libero) = " << bestfit_v0_m2 << " +- " << err_bestfit_v0_m2 << "Hz" << std::endl;

    double L_m2_v0 = get_L_fromCv0(bestfit_v0_m2); double err_L_m2_v0 = get_err_L_fromCv0(bestfit_v0_m2, err_bestfit_v0_m2);
    std::cout << "L (noto C = " << _C << ", v0 = " << bestfit_v0_m2 << ", miglior stima)  = " << L_m2_v0 << " +- " << err_L_m2_v0 << " H" << std::endl;

    double mu_R_m2 = get_mu_R_fromLL0(L_m2_v0, L_0, a_Al); double err_mu_R_m2 = get_err_mu_R_fromLL0(L_m2_v0, L_0, a_Al, err_L_m2_v0, err_L_0);
    std::cout << "mu_R per Al (da L e L0)  => " << mu_R_m2 << " +- " << err_mu_R_m2 << std::endl;

    double val[6] = {L_m2_v0, L_0, mu0, n, l, a_Al};
    double err[6] = {err_L_m2_v0, err_L_0, 0, 0, 1e-4/sqrt(3), 0.05e-3/sqrt(3)};

    std::cout << "mu_R per AL (da L e L0)  => " << get_pValue("(x[0]-x[1])/(x[2]*x[3]*x[5]*x[3]*x[5]*x[4])+1", val) << " +- " << get_pError("(x[0]-x[1])/(x[2]*x[3]*x[5]*x[3]*x[5]*x[4])+1", val, err) << std::endl;

    
    c1->SaveAs("../fig/plot.pdf");
    return;
}

// ============================================================================================================================================================

result analisi_RLC_filter(std::string file, double* params, TCanvas* canvas, int position, double fitmin = -1, double fitmax = -1, std::string fitoption = ""){

    std::string rawdata = "../dati/" + file;
    std::string output = file.substr(file.find_last_of("_"), file.find(".")-file.find_last_of("_"));

    logs::print_mmsg("Analisi di " + rawdata);

    std::ifstream data(rawdata.c_str());

    std::ofstream out_rawdata(("../misc/rawdata_" + output + ".txt").c_str()); // carbon copy of original data
    std::ofstream out_cleandata(("../misc/cleandata_" + output + ".txt").c_str()); // values from rawdata with error
    std::ofstream out_computeddata(("../misc/computeddata_" + output + ".txt").c_str()); // computed data for final graph

    double Vin, fsVin, Vout, fsVout, T, fsT, dt, fsdt;


    // Analisi 1mo diagramma di BODE, |H(w)| su w
    canvas->cd(position+1);

    TGraphErrors* H_plot = new TGraphErrors();
    H_plot->SetName("H_plot");
    TF1* H_fit = new TF1("H_f", "1/sqrt([0]+[1]*(pow(x/[2]-[2]/x, 2)))"); // ! Controllare formule
    H_fit->SetParameters(params[0], params[1]*params[1], params[2]);
    H_fit->SetParLimits(0, 0, 10000);
    H_fit->SetParLimits(1, 0, 10000);
    // H_fit->SetParLimits(2, 0, 10000);
    // [0] -> A = (1 + R_L / R)^2
    // [1] -> Q^2 = fattore di qualita = (1/(R C w_0))^2
    // [2] -> v0


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
    TF1* phi_fit = new TF1("phi_f", "-atan([0]*(x/[1]-[1]/x))"); // ! Controllare formule
    phi_fit->SetParameters(params[1]/sqrt(params[0]), params[2]);
    phi_fit->SetParLimits(0, 0, 10000);
    // phi_fit->SetParLimits(1, 0, 10000);
    // [0] -> Q/sqrt(A)
    // [1] -> v0

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
        out_rawdata << Vin << " & " << fsVin << " & " << Vout << " & " << fsVout << " & " << T << " & " << fsT << " & " << dt << " & " << fsdt << " \\\\ " << std::endl;
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


        out_computeddata << "\\num{" << Vout / Vin << " +- " << get_HErr(Vin, Vout, eVin, eVout) << "} "
                       << "& \\num{" << 2 * M_PI * dt / T << " +- " << 2 * M_PI * sqrt(pow(edt/T, 2) + pow(dt * eT/(pow(T, 2)), 2)) << "} "
                       << "& \\num{" << 1 / T << " +- " << eT/pow(T, 2) << "} \\\\" << std::endl;
    }
    out_rawdata << "EOF" << std::endl;
    out_cleandata << "EOF" << std::endl;
    out_computeddata << "EOF" << std::endl;


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

    header->DrawLatexNDC(0.3, 0.15, ("#splitline{#it{#bf{" + rawdata + "}}}{#splitline{#it{1#circ diagramma di Bode} #bf{(A)}}{" + H_stat + "}}").c_str());

    logs::print_stat(H_fit);

    // RESIDUI
    Hp2->cd();
    graphset::fillresiduals(H_plot, H_fit, H_resd);
    H_resd->GetXaxis()->SetLimits(450, 55e3);
    H_res_f->GetXaxis()->SetLimits(450, 55e3);
    H_resd->Draw("ap");
    H_res_f->Draw("same");

    double A_amp= H_fit->GetParameter(0);
    double err_A_amp = H_fit->GetParError(0);
    double Q_amp = sqrt(H_fit->GetParameter(1));
    double err_Q_amp = 1/(2*Q_amp)*H_fit->GetParError(1);
    double v0_amp = H_fit->GetParameter(2);
    double err_v0_amp = H_fit->GetParError(2);
    double Q_sqrtA_amp = Q_amp/sqrt(A_amp);
    double err_Q_sqrtA_amp = sqrt(pow(err_Q_amp/A_amp, 2) + pow(err_A_amp*Q_amp/(2*pow(A_amp, 3/2)), 2));

    std::cout << "A da |H(w)|                      = " << A_amp  << " +- " << err_A_amp << std::endl
              << "Fattore di Qualita' da |H(w)|, Q = " << Q_amp  << " +- " << err_Q_amp << std::endl
              << "Frequenza di Taglio da |H(w)|, v = " << v0_amp << " +- " << err_v0_amp << " Hz" << std::endl;

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

    phi_header->DrawLatexNDC(0.5, 0.8, ("#splitline{#it{#bf{" + rawdata + "}}}{#splitline{#it{2#circ diagramma di Bode} #bf{(B)}}{" + phi_stat + "}}").c_str());

    logs::print_stat(phi_fit);

    // RESIDUI
    phi_p2->cd();
    graphset::fillresiduals(phi_plot, phi_fit, phi_resd);
    phi_resd->GetXaxis()->SetLimits(450, 55e3);
    phi_res_f->GetXaxis()->SetLimits(450, 55e3);
    phi_resd->Draw("ap");
    phi_res_f->Draw("same");

    double Q_sqrtA_fase = phi_fit->GetParameter(0);
    double err_Q_sqrtA_fase = phi_fit->GetParError(0);
    double v0_fase = phi_fit->GetParameter(1);
    double err_v0_fase = phi_fit->GetParError(1);

    std::cout << "Q/sqrt(A) da phi(w)            = " << Q_sqrtA_fase << " +- " << err_Q_sqrtA_fase << std::endl
            << "Frequenza di Taglio da phi(w), v = " << v0_fase      << " +- " << err_v0_fase << " Hz" << std::endl;

    std::cout << std::endl << "** Verifica compatibilita =>  (Q/sqrt(A)) " << stattools::compatible(Q_sqrtA_fase, err_Q_sqrtA_fase, Q_sqrtA_amp, err_Q_sqrtA_amp) << std::endl;
    std::cout              << "** Verifica compatibilita =>         (v0) " << stattools::compatible(v0_amp, err_v0_amp, v0_fase, err_v0_fase) << std::endl;


    graphset::set_TGraphAxis(H_plot, "#left|H(#nu)#right| [a. u.]", 4);
    graphset::set_ResidualsAxis(H_resd, "Frequenza #nu [Hz]", 4);

    graphset::set_TGraphAxis(phi_plot, "Fase #varphi(#nu) [rad]", 4);
    graphset::set_ResidualsAxis(phi_resd, "Frequenza #nu [Hz]", 4);

    // Formato dati ritorno funzione => A_amp, Q_amp, v0_amp, Q_sqrtA_fase, v0_fase;
    return {{A_amp, err_A_amp}, {Q_amp, err_Q_amp}, {v0_amp, err_v0_amp}, {Q_sqrtA_fase, err_Q_sqrtA_fase}, {v0_fase, err_v0_fase}};
}


#ifndef __CLING__
int main(){
    gInterpreter->AddIncludePath("~/Documents/CustomLibs/ErrorAnalysis/");
    gSystem->Load("~/Documents/CustomLibs/libErrorAnalysis.dylib");
    analisi_permeabilita();
    return 0;
}
#endif