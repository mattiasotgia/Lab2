// Lab Analysis Toolset "LabTools.h" -*- C++ -*- 
// Author M. Sotgia 05/11/21
// v0.0.2 alpha

#ifndef _LABTOOLS_H
#define _LABTOOLS_H

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

const double title_size = 21;
const double label_size = 15;
const double offsetx = 4;

namespace log
{
    void print_mmsg(std::string mmsg){
        std::cout << std::endl
                  << " **********" << std::endl
                  << "    " << mmsg << std::endl
                  << " **********" << std::endl
                  << std::endl;
    }

    void print_stat(TF1 *_f){
        std::cout << std::endl
                  << "** "
                  << "CHI2 / NDF ( PROB. ) "
                  << _f->GetChisquare() << " / " << _f->GetNDF() << " ( " << _f->GetProb() << " )"
                  << std::endl
                  << std::endl;
    }
} // namespace log

namespace stattools
{
    double max_to_stat(double value){
        return value / (std::sqrt(3));
    }

    std::string compatible(double G1, double errG1,
                       double G2, double errG2){
        double abs_values = abs(G2 - G1);
        double err_abs_val = 3 * sqrt(pow(errG1, 2) + pow(errG2, 2));
        if (abs_values < err_abs_val){
            return "COMPATIBILE";
        }
        return "NON-COMPATIBILE";
    }

    double getbestvalue(double G1, double G2, double errG1, double errG2){
        return (G1 / pow(errG1, 2) + G2 / pow(errG2, 2)) / (1 / pow(errG1, 2) + 1 / pow(errG2, 2));
    }

    double getbestvalueerr(double errG1, double errG2){
        return sqrt(1 / (1 / pow(errG1, 2) + 1 / pow(errG2, 2)));
    }
} // namespace stat

namespace graphset
{   

    class graph{

        // cosa serve che si possa fare:
        // * creato un elemento della classe voglio che questa mi dia accesso a:
        //   1 grafico dei dati, 1 grafico dei residui, 1 pad con grafico e residui.
        // * voglio poter decidere se avere il grafico dei residui oppure no

    private:

        bool _has_fitted = false;
        bool _show_res = true;
        bool _logx = false;
        bool _logy = false;

        TPad* _full_pad = new TPad();
        TPad* _g_pad;
        TPad* _r_pad;
        TGraphErrors* _g_graph = new TGraphErrors();
        TGraphErrors* _r_graph = new TGraphErrors();
        TF1* _g_fit;
        TF1* _r_fit = new TF1("_r_fit", "0");

        const double _title_size = 21;

        Double_t xmin, xmax;

        void _fillresiduals();
        void _fitgraph();
        void _drawgraph();
        void _drawresiduals();

    public:
        graph(bool showresiduals = true);

        TPad* GetPad(){return _full_pad;};
        TGraphErrors* GetGraph(){return _g_graph;};
        TF1* GetFitTF1(){return _g_fit;} // needed to get SetParameters() method;

        void SetFitFormula(std::string formula);
        void SetFitLimits(Double_t min = (0.0), Double_t max = (1.0));

        void SetLogX(){_logx = true;}
        void SetLogY(){_logx = true;}
        ~graph();
    };

    void graph::_fillresiduals(){

        if(!_show_res){
            return;
        }
        if(_has_fitted){
            for (int i = 0; i < _g_graph->GetN(); i++){
                _r_graph->SetPoint(i, _g_graph->GetX()[i], (_g_graph->GetY()[i] - _g_fit->Eval(_g_graph->GetX()[i])) / _g_graph->GetEY()[i]);
                _r_graph->SetPointError(i, 0, 1);
            }
        }
    }

    void graph::SetFitFormula(std::string formula){
        _g_fit = new TF1("_g_fit", formula.c_str());
    }

    void graph::SetFitLimits(Double_t min = (0.0), Double_t max = (1.0)){
        xmin = min;
        xmax = max;
        _r_fit->SetRange(min, max);
    }
    graph::graph(bool showresiduals = true){
        if(!showresiduals){
            _g_pad = new TPad("", "", 0.0, 0.3, 1.0, 1.0);
            _r_pad = new TPad("", "", 0.0, 0.0, 0.0, 0.0);
            _show_res = false;
            _r_pad->Delete();
        }
        _g_pad = new TPad("", "", 0.0, 0.3, 1.0, 1.0);  
        _r_pad = new TPad("", "", 0.0, 0.0, 1.0, 0.295);
    }
    
    // graph::~graph(){
    // }




    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    //                    FUNZIONI STABILI SENZA CLASSE                        //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////

    bool _isresidualon = true;
    std::string* _xtitle = new std::string("ERR: inverti set_ResidualAxis() #leftrightarrow set_TGraphAxis()");

    /* REMINDER: impostare prima il metodo `set_ResidualAxis()` e poi `set_TGraphAxis()` per
    ottenere il risultato voluto. */
    void set_TGraphAxis(TGraphErrors *g, std::string ytitle, float offset = 2){
        g->SetTitle("");
        g->GetYaxis()->SetTitle(ytitle.c_str());
        g->GetYaxis()->SetTitleOffset(offset);
        g->GetYaxis()->SetTitleFont(43);
        g->GetYaxis()->SetTitleSize(title_size);
        g->GetYaxis()->SetLabelFont(43);
        g->GetYaxis()->SetLabelSize(label_size);
        g->GetYaxis()->CenterTitle();

        if(!_isresidualon){
            std::cout << _isresidualon << std::endl;
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
    void set_ResidualsAxis(TGraphErrors *rg, std::string xtitle, float offset = 2, std::string ytitle = "Residui [#sigma]"){
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
    
    struct padtypes{
        TPad* Graph = new TPad();
        TPad* Residuals = new TPad();
    };


    /* Impostazione dei pad del grafico:
    Il primo argomento è una struttura che contiene i TPad dei grafico e dei resudui;
    Se dico di disegnare i residui tutto OK, se dico false al valore booleano drawresiduals
    allora il grafico verrà eseguito senza i residui, e il nome degli assi impostato di con-
    seguenza. È importante chiamare i comandi `set_ResidualAxis()` e `set_TGraphAxis()` in 
    questo ordine, di modo che il titolo sull'asse x venga mostrato, altrimenti si mostrerà 
    un titolo di errore */
    void setgraphsize(graphset::padtypes g, bool logx=false, bool logy=false, bool drawresiduals = true){

        g.Graph->SetFillColor(0);
        g.Residuals->SetFillColor(0);
        if(logx){
            g.Graph->SetLogx();
            g.Residuals->SetLogx();
        }
        if(logy){
            g.Graph->SetLogy();
        }
        if(!drawresiduals){
            g.Graph->SetPad(0.0, 0.0, 1.0, 1.0);
            g.Residuals->SetPad(0.0, 0.0, 1.0, 0.0);
            g.Graph->SetMargin(0.14, 0.06, 0.125, 0.06);
            g.Graph->Draw();
            _isresidualon = false;
        }else{
            g.Graph->SetMargin(0.14, 0.06, 0.0, 0.06);
            g.Residuals->SetMargin(0.14, 0.06, 0.4, 1.0);
            g.Graph->SetPad(0.0, 0.3, 1.0, 1.0);
            g.Residuals->SetPad(0.0, 0.0, 1.0, 0.295);            
            g.Graph->Draw();
            g.Residuals->Draw();
        }
    }

    void setcanvas(TCanvas* c1, int nx = 1, int ny = 1, 
                float m_left = 0.16, float m_right = 0.06, 
                float m_bottom = 0.12, float m_top = 0.06){
        c1->SetMargin(m_left, m_right, m_bottom, m_top);
        c1->SetFillStyle(4000);
        c1->Divide(nx, ny);
    }

    void setpad(TPad* p1, int nx, int ny, 
            float m_left = 0.16, float m_right = 0.06, 
            float m_bottom = 0.12, float m_top = 0.06){
        // p1->SetMargin(m_left, m_right, m_bottom, m_top);
        p1->SetFillStyle(4000);
        p1->Divide(nx, ny);
    }

    void fillresiduals(TGraphErrors* g, TF1* g_fit, TGraphErrors* r){
            for(int i=0; i<g->GetN(); i++){
                r->SetPoint(i, g->GetX()[i], (g->GetY()[i] - g_fit->Eval(g->GetX()[i])) / g->GetEY()[i]);
                r->SetPointError(i, 0, 1);
            }
    }
}

// * todo: fare in modo da avere che riempendo il grafico in 
// *       automatico se io genero i residui sono già riempiti

#endif
