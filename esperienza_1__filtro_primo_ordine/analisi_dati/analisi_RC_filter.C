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

const double title_size = 21;

void print_mmsg(std::string mmsg){
    std::cout << std::endl 
        << " **********" << std::endl
        << "    " << mmsg << std::endl
        << " **********" << std::endl
        << std::endl;
}

void print_stat(TF1* _f){
    std::cout << std::endl
        << "** " << "CHI2 / NDF ( PROB. ) " 
        << _f->GetChisquare() << " / " << _f->GetNDF() << " ( " << _f->GetProb() << " )"
        << std::endl << std::endl;
}

std::string compatible(double G1, double errG1,
                       double G2, double errG2){
    double abs_values = abs(G2-G1);
    double err_abs_val = 3*sqrt(pow(errG1, 2) + pow(errG2, 2));
    if(abs_values<err_abs_val){
        return "COMPATIBILE";
    }
    return "NON-COMPATIBILE";
}

void set_TGraphAxis(TGraphErrors* g, std::string ytitle){
    g->SetTitle("");
    g->GetYaxis()->SetTitle(ytitle.c_str());
    g->GetYaxis()->SetTitleOffset(2);
    g->GetYaxis()->SetTitleFont(43);
    g->GetYaxis()->SetTitleSize(title_size);
    g->GetYaxis()->SetLabelFont(43);
    g->GetYaxis()->SetLabelSize(12);
    g->GetYaxis()->CenterTitle();
}

void set_ResidualsAxis(TGraphErrors* rg, std::string xtitle, std::string ytitle="Residui [#sigma]"){
    rg->GetXaxis()->SetTitle(xtitle.c_str());
    rg->GetXaxis()->SetTitleOffset(5);
    rg->GetXaxis()->SetTitleFont(43);
    rg->GetXaxis()->SetTitleSize(title_size);

    rg->GetYaxis()->SetTitle(ytitle.c_str());
    rg->GetYaxis()->SetTitleOffset(2);
    rg->GetYaxis()->SetTitleFont(43);
    rg->GetYaxis()->SetTitleSize(title_size);
    rg->GetYaxis()->CenterTitle();

    rg->GetYaxis()->SetLabelFont(43);
    rg->GetYaxis()->SetLabelSize(12);
    rg->GetYaxis()->SetNdivisions(5, 5, 0);
    rg->GetXaxis()->SetLabelFont(43);
    rg->GetXaxis()->SetLabelSize(12);
    rg->GetXaxis()->CenterTitle();
}

double max_to_stat(double value){
    return value/(std::sqrt(3));
}

// funzione calcolo incertezza a partire da fondo scala (per Qualsiasi grandezza)
// tab. VALORI  | Grandezza misurata | errPercent | partitions | fondoscala (range1) 
//              | V (tensione)       | 3.5%       | 8          | variabile
//              | T (periodi)        | ?.?%       | ?          | variabile

double get_RangeErr(double errPercent, int partitions, double range1){ 
  return errPercent * partitions *  range1;
}

double getH(double vin, double vout){
    return vout / vin;
}

double get_phi(double T, double dt){
    return 2 * M_PI * dt / T;
}

// funzione calcolo propagazione errore statistici
double get_Err2(double G1, double G2, double eG1, double eG2){ 
  return std::sqrt(std::pow(max_to_stat(eG1) / G1, 2) + std::pow(max_to_stat(eG2) / G2, 2));
}

// fisso i valori di R e C ???
const double R = 50;
const double C = 0.00000000001;

void analisi_RC_filter(){

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

    std::string rawdata="../dati/presadati1.txt";
    std::ifstream data(rawdata.c_str());

    std::ofstream out_rawdata("../misc/rawdata.txt"); // carbon copy of original data
    std::ofstream out_cleandata("../misc/cleandata.txt"); // values from rawdata with error
    std::ofstream out_computeddata("../misc/computeddata.txt"); // computed data for final graph

    double Vin, fsVin, Vout, fsVout, T, fsT, dt, fsdt;

    TCanvas* c1 = new TCanvas("c1", "", 600, 1000);
    c1->SetMargin(0.16, 0.06, 0.12, 0.06);
    c1->SetFillStyle(4000);
    c1->Divide(1, 2);

    // Analisi 1mo diagramma di BODE, |H(w)| su w
    c1->cd(1);

    TGraphErrors* H_plot = new TGraphErrors();
    TF1* H_fit = new TF1("Hf", "1/sqrt(1+(pow(x/[0], 2)))");
    H_fit->SetParameter(0, 10e4);

    TGraphErrors* H_resd = new TGraphErrors();
    TF1* H_res_f = new TF1("rf", "0", 0, 1100);
    H_res_f->SetLineStyle(2);

    TPad* Hp1 = new TPad("", "", 0.0, 0.3, 1.0, 1.0);
    TPad* Hp2 = new TPad("", "", 0.0, 0.0, 1.0, 0.295);
    Hp1->SetMargin(0.14, 0.06, 0.0, 0.06);
    Hp1->SetFillStyle(4000);
    Hp1->Draw();
    Hp2->SetMargin(0.14, 0.06, 0.4, 1.0);
    Hp2->SetFillStyle(4000);
    Hp2->Draw();

    for(int i=0; data >> Vin >> fsVin >> Vout >> fsVout >> T >> fsT >> dt >> fsdt; i++){
        out_rawdata << Vin << " " << fsVin << " " << Vout << " " << fsVout << " " << T << " " << fsT << " " << dt << " " << fsdt << std::endl;
        double eVin = max_to_stat(get_RangeErr(0.035, 8, fsVin));
        double eVout = max_to_stat(get_RangeErr(0.035, 8, fsVout));
        double eT = max_to_stat(get_RangeErr(0.035, 8, fsT));
        double edt = max_to_stat(get_RangeErr(0.035, 8, fsdt));

        out_cleandata << Vin << " " << eVin << " " << Vout << " " << eVout << " " << T << " " << eT << " " << dt << " " << edt << std::endl;

        H_plot->SetPoint(i, Vout / Vin, 1 / T);
        H_plot->SetPointError(i, get_Err2(Vin, Vout, eVin, eVout), eT);
    }
    out_rawdata << "EOF" << std::endl;
    out_cleandata << "EOF" << std::endl;



    Hp1->cd();
    H_plot->Draw("ap");
    H_plot->Fit("Hf");

    Hp2->cd();

    for(int i=0; i<H_plot->GetN(); i++){
            H_resd->SetPoint(i, H_plot->GetX()[i], (H_plot->GetY()[i] - H_fit->Eval(H_plot->GetX()[i]))/H_plot->GetEY()[i]);
            H_resd->SetPointError(i, 0, 1);
    }
    H_resd->Draw("ap");
    H_res_f->Draw("same");

    set_TGraphAxis(H_plot, "|H(#omega)| [Db]");
    set_ResidualsAxis(H_resd, "#omega [2#pi Hz]");

    // todo: salvare il file come pdf, aggiungere statistiche analisi dati

    // Analisi 2do diagramma di BODE, phi su w
    c1->cd(2);






    

    return;
}

#ifndef __CINT__
int main(){
    analisi_RC_filter();
    return 0;
}
#endif