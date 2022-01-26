// Lab Analysis Toolset - Statistics - "LabStat" -*- C++ -*-
// Author M. Sotgia 14/01/22
// v0.1.0 alpha

#ifndef LABTOOLS_StaTools
#define LABTOOLS_StaTools

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

namespace unilab{

    std::string compatible_nocorr(double G1, double errG1, double G2, double errG2);

    std::string get_statsign(double G1, double errG1, double G2, double errG2);

    double getbestvalue(double G1, double errG1, double G2, double errG2);
    double getbestvalueerr(double errG1, double errG2);

} // unilab

#endif