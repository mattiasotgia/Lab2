// Lab Analysis Toolset - Graphing - "LabPlot" -*- C++ -*-
// Author M. Sotgia 14/01/22
// v0.1.0 alpha

#include<vector>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>
#include<tuple>

#include<TCanvas.h>
#include<TPad.h>
#include<TGraphErrors.h>
#include<TH1.h>
#include<TF1.h>
#include<TStyle.h>
#include<TAxis.h>
#include<TMath.h>
#include<TLatex.h>
#include<TLegend.h>
#include<TLine.h>
#include<TROOT.h>

#include"ATLASStyle.h"
#include"LabPlot.h"
// #include"LabPlotCore.h"
#include"Logger.h"

void set_atlas_style(Size_t tsize, Bool_t setticky){

    printf("\u001b[34;1mApplying ATLAS style settings\u001b[0m\n");

    TStyle *style = (tsize==_FNULL ? atlas_style():atlas_style(tsize));

    gROOT->SetStyle("ATLAS");
    gROOT->ForceStyle();
}

template<typename _TObj>
void set_axis(_TObj *graph, ax_Title xtitle, ax_Title ytitle){

    Double_t fontsize = 8;
    
    graph->SetTitle(Form(";%s;%s", xtitle, ytitle));
    graph->GetYaxis()->CenterTitle();
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

////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////

TLatex *set_label( const char* text, const char* loc, 
                   Float_t x, Float_t y, 
                   Size_t size, Font_t font, Size_t align, 
                   Color_t color, Float_t alpha, Float_t angle
){
    if(!gROOT->GetStyle("ATLAS")) 
        fprintf(stderr, "%s", Logger::warning("The 'ATLAS' style has not been set"));
    
    TLatex *label = new TLatex();
    label->SetNDC();

    Float_t xpos, ypos;

    if (size!=_FNULL)
        label->SetTextSize(size);

    if (font!=_FNULL)
        label->SetTextFont(font);

    if (align!=_FNULL)
        label->SetTextAlign(align);

    if (color!=_FNULL && alpha!=_FNULL)
        label->SetTextColorAlpha(color, alpha);
    else if (color != _FNULL)
        label->SetTextColor(color);

    if (angle != _FNULL)
        label->SetTextAngle(angle);

    if (font != _FNULL)
        label->SetTextFont(font);

    // Decide on label position
    if (x != _FNULL && y != _FNULL) {
        // Set location manually
        xpos = x;
        ypos = y;
    } else if (!strncmp(loc, "", 10)) {
        // Set location automatically from 'loc' argument
        if (strncmp(loc, "upper left", 10)){
            label->SetTextAlign(13);
            xpos = gPad->GetLeftMargin() + 0.04;
            ypos = 1 - gPad->GetTopMargin() - 0.04;
        } else if (strncmp(loc, "upper right", 10)) {
            label->SetTextAlign(33);
            xpos = 1 - gPad->GetRightMargin() - 0.04;
            ypos = 1 - gPad->GetTopMargin() - 0.04;
        }else if (strncmp(loc, "lower left", 10)) {
            label->SetTextAlign(11);
            xpos = gPad->GetLeftMargin() + 0.04;
            ypos = gPad->GetBottomMargin() + 0.04;
        } else if (strncmp(loc, "lower right", 10)) {
            label->SetTextAlign(31);
            xpos = 1 - gPad->GetRightMargin() - 0.04;
            ypos = gPad->GetBottomMargin() + 0.04;
        } else {
            fprintf(stderr, "\u001b[31;1mwarning:\u001b[0m unrecognized location '%s'.\nFalling back on 'upper left'\n", loc);
            label->SetTextAlign(13);
            xpos = gPad->GetLeftMargin() + 0.04;
            ypos = 1 - gPad->GetTopMargin() - 0.04;
        }
    } else {
        // User 'upper left' if no position arguments are given
        label->SetTextAlign(13);
        xpos = gPad->GetLeftMargin() + 0.04;
        ypos = 1 - gPad->GetTopMargin() - 0.04;
    }

    if (strncmp(text, "", 1)){
        label->DrawLatex(xpos, ypos, "#bf{#it{LAB. 2}}");
    } else {
        label->DrawLatex(xpos, ypos, Form("#bf{#it{LAB. 2}} %s", text));
    }

    return label;

}