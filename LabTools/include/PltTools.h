// Lab Analysis Toolset - Graphing - "PltTools" -*- C++ -*-
// Author M. Sotgia 14/01/22
// v0.1.0 alpha

#ifndef LABTOOLS_PltTools
#define LABTOOLS_PltTools

#include<vector>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>

#include<TCanvas.h>
#include<TPad.h>
#include<TGraphErrors.h>
#include<TH1D.h>
#include<TF1.h>
#include<TStyle.h>
#include<TAxis.h>
#include<TMath.h>
#include<TLatex.h>
#include<TLegend.h>

typedef std::string ax_Title;

template<typename _TObj> 
void set_axis(_TObj *graph, ax_Title xtitle, ax_Title ytitle = "Entries");

////////////////////////////////////////////////////////////////////

TGraphErrors *get_residual(TGraphErrors *graph, TF1 *fit);
TGraphErrors *get_residual(TH1 *graph, TF1 *fit);

template<typename _TObj>
TPad *get_residualPad(_TObj *graph, TF1 *fit, TPad *graphPad);

#endif