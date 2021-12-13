// Lab Analysis Toolset "LabTools.h" -*- C++ -*- 
// Author M. Sotgia 05/11/21
// v0.0.2 alpha
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include<TCanvas.h>
#include<TGraphErrors.h>
#include<TF1.h>
#include<TStyle.h>
#include<TAxis.h>
#include<TMath.h>
#include<TLatex.h>
#include<TLegend.h>

#include"LabTools.h"


template<class _container, class _Ty> inline
bool base::isIn(_container _C, const _Ty& _Val){
    return std::find(_C.begin(), _C.end(), _Val) != _C.end();
}


void log::print_mmsg(std::string mmsg){
    std::cout << std::endl
              << " **********" << std::endl
              << "    " << mmsg << std::endl
              << " **********" << std::endl
              << std::endl;
}

void log::print_stat(TF1 *_f){
    std::cout << std::endl
              << "** "
              << "CHI2 / NDF ( PROB. ) "
              << _f->GetChisquare() << " / " << _f->GetNDF() << " ( " << _f->GetProb() << " )"
              << std::endl
              << std::endl;
}

double stattools::max_to_stat(double value){
    return value / (std::sqrt(3));
}

std::string stattools::compatible(double G1, double errG1,
                   double G2, double errG2){
    double abs_values = abs(G2 - G1);
    double err_abs_val = 3 * sqrt(pow(errG1, 2) + pow(errG2, 2));
    if (abs_values < err_abs_val){
        return "COMPATIBILE";
    }
    return "NON-COMPATIBILE";
}

double stattools::getbestvalue(double G1, double G2, double errG1, double errG2){
    return (G1 / pow(errG1, 2) + G2 / pow(errG2, 2)) / (1 / pow(errG1, 2) + 1 / pow(errG2, 2));
}

double stattools::getbestvalueerr(double errG1, double errG2){
    return sqrt(1 / (1 / pow(errG1, 2) + 1 / pow(errG2, 2)));
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                    FUNZIONI STABILI SENZA CLASSE                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/* REMINDER: impostare prima il metodo `set_ResidualAxis()` e poi `set_TGraphAxis()` per
ottenere il risultato voluto. */
template <class _TObj>
inline void graphset::set_TGraphAxis(_TObj *g, std::string ytitle, float offset, std::string xtitle)
{
    g->SetTitle("");
    g->GetYaxis()->SetTitle(ytitle.c_str());
    g->GetYaxis()->SetTitleOffset(offset);
    g->GetYaxis()->SetTitleFont(43);
    g->GetYaxis()->SetTitleSize(title_size);
    g->GetYaxis()->SetLabelFont(43);
    g->GetYaxis()->SetLabelSize(label_size);
    g->GetYaxis()->CenterTitle();

    if (xtitle != "")
    {
        // std::cout << _isresidualon << std::endl;
        g->GetXaxis()->SetTitle(xtitle.c_str());
        g->GetXaxis()->SetTitleOffset(1);
        g->GetXaxis()->SetTitleFont(43);
        g->GetXaxis()->SetTitleSize(title_size);

        g->GetXaxis()->SetLabelFont(43);
        g->GetXaxis()->SetLabelSize(label_size);
        g->GetXaxis()->CenterTitle();
    }
    else if (!_isresidualon)
    {
        // std::cout << _isresidualon << std::endl;
        g->GetXaxis()->SetTitle(((std::string)*_xtitle).c_str());
        g->GetXaxis()->SetTitleOffset(1);
        g->GetXaxis()->SetTitleFont(43);
        g->GetXaxis()->SetTitleSize(title_size);

        g->GetXaxis()->SetLabelFont(43);
        g->GetXaxis()->SetLabelSize(label_size);
        g->GetXaxis()->CenterTitle();
    }

    g->GetXaxis()->SetTickLength(0.05);
}

/* REMINDER: impostare prima il metodo `set_ResidualAxis()` e poi `set_TGraphAxis()` per
ottenere il risultato voluto. */
template <class _TObj>
inline void graphset::set_ResidualsAxis(_TObj *rg, std::string xtitle, float offset, std::string ytitle)
{
    rg->GetXaxis()->SetTitle(xtitle.c_str());
    rg->GetXaxis()->SetTitleOffset(offsetx);
    rg->GetXaxis()->SetTitleFont(43);
    rg->GetXaxis()->SetTitleSize(title_size);

    rg->GetYaxis()->SetTitle(ytitle.c_str());
    rg->GetYaxis()->SetTitleOffset(offset);
    rg->GetYaxis()->SetTitleFont(43);
    rg->GetYaxis()->SetTitleSize(title_size);
    rg->GetYaxis()->CenterTitle();

    rg->GetYaxis()->SetLabelFont(43);
    rg->GetYaxis()->SetLabelSize(label_size);
    rg->GetYaxis()->SetNdivisions(5, 5, 0);
    rg->GetXaxis()->SetLabelFont(43);
    rg->GetXaxis()->SetLabelSize(label_size);
    rg->GetXaxis()->CenterTitle();
    *_xtitle = xtitle;

    rg->GetXaxis()->SetTickLength(0.08);
}

/* Impostazione dei pad del grafico:
Il primo argomento è una struttura che contiene i TPad dei grafico e dei resudui;
Se dico di disegnare i residui tutto OK, se dico false al valore booleano drawresiduals
allora il grafico verrà eseguito senza i residui, e il nome degli assi impostato di con-
seguenza. È importante chiamare i comandi `set_ResidualAxis()` e `set_TGraphAxis()` in
questo ordine, di modo che il titolo sull'asse x venga mostrato, altrimenti si mostrerà
un titolo di errore */
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

template <class _TObj>
inline void graphset::setcanvas(_TObj *c1, int nx, int ny,
                      float m_left, float m_right,
                      float m_bottom, float m_top)
{
    c1->SetMargin(m_left, m_right, m_bottom, m_top);
    c1->SetFillStyle(4000);
    c1->Divide(nx, ny);
}

template <class _TObj, class _TFObj>
inline void graphset::fillresiduals(_TObj *g, _TFObj *g_fit, TGraphErrors *r)
{
    for (int i = 0; i < g->GetN(); i++)
    {
        r->SetPoint(i, g->GetX()[i], (g->GetY()[i] - g_fit->Eval(g->GetX()[i])) / g->GetEY()[i]);
        r->SetPointError(i, 0, 1);
    }
}

/* Includere qui tutti i parametri globali/semi-globali per
l'inizializzazione di grafici in ROOT
    gStyle->SetFrameLineWidth(0);
    gStyle->SetTextFont(43);
    gStyle->SetLineScalePS(1);*/
void graphset::init()
{
    gStyle->SetFrameLineWidth(0); // No line all aroung pad
    gStyle->SetTextFont(43);      // Helvetice font
    gStyle->SetLineScalePS(1);    // Hi-res. pdf
    return;
}

template <class _TObj, class _TFObj>
inline void graphset::setmarker(_TObj *_g, _TFObj *_g_fit, Color_t lcolor, Style_t mstyle, Size_t msize)
{
    _g->SetMarkerStyle(mstyle);
    _g->SetMarkerSize(msize);
    if (lcolor != -1)
    {
        _g->SetLineColor(lcolor);
        _g->SetMarkerColor(lcolor);
        _g_fit->SetLineColor(lcolor);
    }
}

template <class _TObj>
inline void graphset::setmarker(_TObj *_g, Color_t lcolor, Style_t mstyle, Size_t msize)
{
    _g->SetMarkerStyle(mstyle);
    _g->SetMarkerSize(msize);
    if (lcolor != -1)
    {
        _g->SetLineColor(lcolor);
        _g->SetMarkerColor(lcolor);
    }
}


// * todo: fare in modo da avere che riempendo il grafico in 
// *       automatico se io genero i residui sono già riempiti

#endif
