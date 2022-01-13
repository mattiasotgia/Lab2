// Lab Analysis Toolset "LabTools.h" -*- C++ -*-
// Author M. Sotgia 05/11/21
// v0.0.2 alpha

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

#include "LabTools.h"


void logs::print_mmsg(std::string mmsg)
{
    std::cout << std::endl
              << " **********" << std::endl
              << "    " << mmsg << std::endl
              << " **********" << std::endl
              << std::endl;
}

void logs::print_stat(TF1 *_f)
{
    std::cout << std::endl
              << "** "
              << "CHI2 / NDF ( PROB. ) "
              << _f->GetChisquare() << " / " << _f->GetNDF() << " ( " << _f->GetProb() << " )"
              << std::endl
              << std::endl;
}

double stattools::max_to_stat(double value)
{
    return value / (std::sqrt(3));
}

std::string stattools::compatible(double G1, double errG1,
                                  double G2, double errG2)
{
    double abs_values = abs(G2 - G1);
    double err_abs_val = 3 * sqrt(pow(errG1, 2) + pow(errG2, 2));
    if (abs_values < err_abs_val)
    {
        return "COMPATIBILE";
    }
    return ("NON-COMPATIBILE: significativita' statistica  " + std::to_string(abs(G2 - G1)/sqrt(pow(errG1, 2) + pow(errG2, 2))) + " σ");
}

std::string stattools::get_statsign(double G1, double errG1,
                                   double G2, double errG2){
    return ("significativita' statistica  " + std::to_string(abs(G2 - G1)/sqrt(pow(errG1, 2) + pow(errG2, 2))) + " σ");
}

double stattools::getbestvalue(double G1, double errG1, double G2, double errG2)
{
    return (G1 / pow(errG1, 2) + G2 / pow(errG2, 2)) / (1 / pow(errG1, 2) + 1 / pow(errG2, 2));
}

double stattools::getbestvalueerr(double errG1, double errG2)
{
    return sqrt(1 / (1 / pow(errG1, 2) + 1 / pow(errG2, 2)));
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                    FUNZIONI STABILI SENZA CLASSE                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/*
!!! ERRORE: Se dichiaro una funzione come template in LabTools.h, devo implementarla
!!! in quel file, altrimenti il template<> non saprà dove cercare la funzione ed 
!!! impazzirà. 
*/

void graphset::setgraphsize(graphset::padtypes g, bool logx, bool logy, bool drawresiduals)
{

    g.Graph->SetFillColor(0);
    g.Residuals->SetFillColor(0);
    if (logx)
    {
        g.Graph->SetLogx();
        g.Residuals->SetLogx();
    }
    if (logy)
    {
        g.Graph->SetLogy();
    }
    if (!drawresiduals)
    {
        g.Graph->SetPad(0.0, 0.0, 1.0, 1.0);
        g.Residuals->SetPad(0.0, 0.0, 1.0, 0.0);
        g.Graph->SetMargin(0.14, 0.06, 0.125, 0.06);
        g.Graph->Draw();
        _isresidualon = false;
    }
    else
    {
        g.Graph->SetMargin(0.14, 0.06, 0.0, 0.06);
        g.Residuals->SetMargin(0.14, 0.06, 0.4, 1.0);
        g.Graph->SetPad(0.0, 0.3, 1.0, 1.0);
        g.Residuals->SetPad(0.0, 0.0, 1.0, 0.295);
        g.Graph->Draw();
        g.Residuals->Draw();
    }
    _isgraphset = true;
}

void graphset::init()
{
    //gStyle->SetFrameLineWidth(0); // No line all aroung pad
    gStyle->SetTextFont(43);      // Helvetice font
    gStyle->SetLineScalePS(1);    // Hi-res. pdf
    return;
}
