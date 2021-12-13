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

namespace base
{
    template<class _container, class _Ty> inline
    bool isIn(_container _C, const _Ty& _Val);
} // namespace base

namespace log
{
    void print_mmsg(std::string mmsg);
    void print_stat(TF1 *_f);
} // namespace log

namespace stattools
{
    double max_to_stat(double value);

    std::string compatible(double G1, double errG1, double G2, double errG2);

    double getbestvalue(double G1, double G2, double errG1, double errG2);
    double getbestvalueerr(double errG1, double errG2);
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

        void SetGraph(TGraphErrors* clonegraph);
        void SetFitTF1(TF1* clonefit);


        void SetFitFormula(std::string formula);
        void SetFitLimits(Double_t min, Double_t max);

        void SetLogX(){_logx = true;}
        void SetLogY(){_logx = true;}
        ~graph();
    };

    graph::graph(bool showresiduals = true){

        _show_res=showresiduals;

        // Alcuni accortezze da impostare inizializzando la classe
        _r_fit->SetLineStyle(2);
        
    }

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
    graph::graph(bool showresiduals){
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
    bool _isgraphset = false;
    std::string* _xtitle = new std::string("ERR: inverti set_ResidualAxis() #leftrightarrow set_TGraphAxis()");


    /* REMINDER: impostare prima il metodo `set_ResidualAxis()` e poi `set_TGraphAxis()` per
    ottenere il risultato voluto. */
    template<class _TObj> inline
    void set_TGraphAxis(_TObj *g, std::string ytitle, float offset = 2, std::string xtitle = "");

    /* REMINDER: impostare prima il metodo `set_ResidualAxis()` e poi `set_TGraphAxis()` per
    ottenere il risultato voluto. */
    template<class _TObj> inline
    void set_ResidualsAxis(_TObj *rg, std::string xtitle, float offset = 2, std::string ytitle = "Residui [#sigma]");
    
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
    void setgraphsize(graphset::padtypes g, bool logx=false, bool logy=false, bool drawresiduals = true);

    template<class _TObj> inline
    void setcanvas(_TObj* c1, int nx = 1, int ny = 1, 
                float m_left = 0.16, float m_right = 0.06, 
                float m_bottom = 0.12, float m_top = 0.06);


    template<class _TObj, class _TFObj> inline
    void fillresiduals(_TObj* g, _TFObj* g_fit, TGraphErrors* r);

    /* Includere qui tutti i parametri globali/semi-globali per 
    l'inizializzazione di grafici in ROOT 
        gStyle->SetFrameLineWidth(0);
        gStyle->SetTextFont(43);
        gStyle->SetLineScalePS(1);*/
    void init();

    template<class _TObj, class _TFObj> inline
    void setmarker(_TObj* _g, _TFObj* _g_fit, Color_t lcolor = -1, Style_t mstyle = (Style_t)1, Size_t msize = (1.0F));
    
    template<class _TObj> inline
    void setmarker(_TObj* _g, Color_t lcolor = -1, Style_t mstyle = (Style_t)1, Size_t msize = (1.0F));
    
}

// * todo: fare in modo da avere che riempendo il grafico in 
// *       automatico se io genero i residui sono già riempiti

#endif
