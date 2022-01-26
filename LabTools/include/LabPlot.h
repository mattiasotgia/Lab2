// Lab Analysis Toolset - Graphing - "LabPlot" -*- C++ -*-
// Author M. Sotgia 14/01/22
// v0.1.0 alpha

#ifndef LABTOOLS_LabPlot
#define LABTOOLS_LabPlot

#include<vector>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>

#include<TStyle.h>
#include<TCanvas.h>
#include<TPad.h>
#include<TH1.h>
#include<TGraphErrors.h>
#include<TF1.h>
#include<TAxis.h>
#include<TMath.h>
#include<TLatex.h>
#include<TLegend.h>

#define _FNULL (float)0

void set_atlas_style(Size_t tsize=_FNULL);

typedef std::string ax_Title; //> axis title object

template<typename _TObj> 
void set_axis(_TObj *graph, ax_Title xtitle, ax_Title ytitle = "Entries");

////////////////////////////////////////////////////////////////////

TGraphErrors *get_residual(TGraphErrors *graph, TF1 *fit);
TGraphErrors *get_residual(TH1 *graph, TF1 *fit);

template<typename _TObj>
TPad *get_residualPad(_TObj *graph, TF1 *fit, TPad *graphPad);

#endif