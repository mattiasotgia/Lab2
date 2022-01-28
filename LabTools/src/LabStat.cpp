// Lab Analysis Toolset - Statistics - "LabStat" -*- C++ -*-
// Author M. Sotgia 14/01/22
// v0.1.0 alpha

#include<vector>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>

#include<TCanvas.h>
#include<TGraphErrors.h>
#include<TH1D.h>
#include<TF1.h>
#include<TStyle.h>
#include<TAxis.h>
#include<TMath.h>
#include<TLatex.h>
#include<TLegend.h>

#include"LabStat.h"

std::string LabStat::compatible_nocorr(double G1, double errG1,
                              double G2, double errG2){
    double abs_values = abs(G2 - G1);
    double err_abs_val = 3 * sqrt(pow(errG1, 2) + pow(errG2, 2));
    if (abs_values < err_abs_val)
    {
        return "COMPATIBILE";
    }
    return ("NON-COMPATIBILE: " + get_statsign(G1, errG1, G2, errG2));
}

std::string LabStat::get_statsign(double G1, double errG1,
                         double G2, double errG2){
    return ("significativita' statistica  " + std::to_string(abs(G2 - G1)/sqrt(pow(errG1, 2) + pow(errG2, 2))) + " σ");
}

double LabStat::getbestvalue(double G1, double errG1, double G2, double errG2){
    return (G1 / pow(errG1, 2) + G2 / pow(errG2, 2)) / (1 / pow(errG1, 2) + 1 / pow(errG2, 2));
}

double LabStat::getbestvalueerr(double errG1, double errG2){
    return sqrt(1 / (1 / pow(errG1, 2) + 1 / pow(errG2, 2)));
}