// Lab Analysis Toolset - Graphing - "RooPlot" -*- C++ -*-
// Author M. Sotgia 14/01/22
// v0.1.0 alpha

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
#include<TLine.h>

#include"ATLASStyle.h"
#include"LabPlot.h"

template<typename _TObj>
void set_axis(_TObj *graph, ax_Title xtitle, ax_Title ytitle){

    Double_t fontsize = 8;
    
    graph->SetTitle("");
    graph->GetYaxis()->SetTitle(ytitle.c_str());
    graph->GetYaxis()->CenterTitle();

    graph->GetXaxis()->SetTitle(xtitle.c_str());
    graph->GetXaxis()->CenterTitle();
}

template void set_axis<TGraphErrors>(TGraphErrors*, ax_Title, ax_Title);
template void set_axis<TH1>(TH1*, ax_Title, ax_Title);
template void set_axis<TH1D>(TH1D*, ax_Title, ax_Title);
template void set_axis<TH1F>(TH1F*, ax_Title, ax_Title);
template void set_axis<TH1I>(TH1I*, ax_Title, ax_Title);
template void set_axis<TH1C>(TH1C*, ax_Title, ax_Title);
template void set_axis<TH1S>(TH1S*, ax_Title, ax_Title);
template void set_axis<TF1>(TF1*, ax_Title, ax_Title);

////////////////////////////////////////////////////////////////////

TGraphErrors *get_residual(TGraphErrors *graph, TF1 *fit){

    TGraphErrors *res = new TGraphErrors();
    res->GetXaxis()->SetLimits(graph->GetXaxis()->GetXmin(), graph->GetXaxis()->GetXmax());

    for (int i = 0; i < graph->GetN(); i++){
        res->SetPoint(i, graph->GetX()[i], 
            (graph->GetY()[i] - fit->Eval(graph->GetX()[i])) / graph->GetEY()[i]);
        res->SetPointError(i, 0, 1);
    }

    /* Implementare funzione per impostare gli assi, quindi gli assi del
    residuo saranno gli stessi*/
    set_axis<TGraphErrors>(res, graph->GetXaxis()->GetTitle(), "Residui [#sigma]");

    return res;
}

TGraphErrors *get_residual(TH1 *graph, TF1 *fit){

    TGraphErrors *res = new TGraphErrors();
    res->GetXaxis()->SetLimits(graph->GetXaxis()->GetXmin(), graph->GetXaxis()->GetXmax());

    for (int i = 0; i < graph->GetNbinsX(); i++){
        res->SetPoint(i, graph->GetXaxis()->GetBinCenter(i), 
            (graph->GetBinContent(i) - fit->Eval(graph->GetXaxis()->GetBinCenter(i))) / graph->GetBinError(i));
        res->SetPointError(i, 0, 1);
    }

    /* Implementare funzione per impostare gli assi, quindi gli assi del
    residuo saranno gli stessi*/
    set_axis<TGraphErrors>(res, graph->GetXaxis()->GetTitle(), "Residui [#sigma]");

    return res;
}

template<typename _TObj>
TPad *get_residualPad(_TObj *graph, TF1 *fit, TPad *graphPad){
    TPad *_rPad = new TPad("res", "", 0, 0, graphPad->GetXlowNDC()-0.05, 1);
    _rPad->cd();

    TGraphErrors *_rGraph = get_residual(graph, fit);
    TLine *_rZero = new TLine(graph->GetXaxis()->GetXmin(), 0, graph->GetXaxis()->GetXmax(), 0);
    _rZero->SetLineColor(kRed);
    _rZero->SetLineStyle(kDashed);

    _rGraph->Draw("ap Z");
    _rZero->Draw();

    return _rPad;
}

template TPad *get_residualPad<TGraphErrors>(TGraphErrors*, TF1*, TPad*);
template TPad *get_residualPad<TH1>(TH1*, TF1*, TPad*);