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

void allPlots(){
    
    graphset::init();
    std::string pfix = "../dati/_clean/";
    const int full_dataT_s = 8;
    std::string all_data_table[] = {
        "dati_2.txt",
        "dati_4.txt",
        "dati_5.txt",
        "dati_6.txt",
        "dati_8.txt",
        "dati_10.txt",
        "dati_12.txt",
        // "dati_15.txt",
        "dati_20.txt",
        // "dati_30.txt",
        // "dati_40.txt",
        // "dati_50.txt",
        // "dati_60.txt",
        // "dati_60_1.txt",
    };

    std::ifstream data_table;


    for(int i=0; i<full_dataT_s; i++){
        TCanvas *c1 = new TCanvas("", "", 2000, 1000);
        graphset::setcanvas(c1);
        double t, vin, evin, range;
        TGraphErrors *g = new TGraphErrors();
        data_table.open((pfix + all_data_table[i]).c_str());
        for(int j=0; data_table >> t >> vin >> evin >> range; j++){
            g->SetPoint(j, t, vin);
            g->SetPointError(j, 0, 0.01);
        }
        g->Draw("ap");
        g->SetTitle((pfix + all_data_table[i] + ";Time [s];Voltage output from AMP V_{out} [V]").c_str());
        data_table.close();
        std::string output = all_data_table[i].substr(0, all_data_table[i].find("."));
            c1->SaveAs(("../fig/allplots/plot_" + output + ".pdf").c_str());
    }

    return;
}

#ifndef __CLING__
int main(){
    allPlots();
}
#endif