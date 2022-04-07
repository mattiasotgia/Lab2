// Lab Analysis Toolset "LabTools.h" -*- C++ -*-
// Author M. Sotgia 05/11/21
// v0.1.0 alpha

#ifndef LABTOOLS_LabTools
#define LABTOOLS_LabTools

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

const double title_size = 21;
const double label_size = 15;
const double offsetx = 4;

namespace base
{
    template <class _container, class _Ty>
    bool isIn(_container _C, const _Ty &_Val)
    {
        return std::find(_C.begin(), _C.end(), _Val) != _C.end();
    }
} // namespace base

namespace logs
{
    void print_mmsg(std::string mmsg);
    void print_stat(TF1 *_f);
} // namespace log

namespace stattools
{
    double max_to_stat(double value);

    std::string compatible(double G1, double errG1, double G2, double errG2);
    std::string get_statsign(double G1, double errG1, double G2, double errG2);

    double getbestvalue(double G1, double errG1, double G2, double errG2);
    double getbestvalueerr(double errG1, double errG2);
} // namespace stat

namespace graphset
{

    class graph
    {

        // cosa serve che si possa fare:
        // - creato un elemento della classe voglio che questa mi dia accesso a:
        //   1 grafico dei dati, 1 grafico dei residui, 1 pad con grafico e residui.
        // - voglio poter decidere se avere il grafico dei residui oppure no

    private:
        bool _has_fitted = false;
        bool _show_res = true;
        bool _logx = false;
        bool _logy = false;

        TPad *_full_pad = new TPad();
        TPad *_g_pad;
        TPad *_r_pad;
        TGraphErrors *_g_graph = new TGraphErrors();
        TGraphErrors *_r_graph = new TGraphErrors();
        TF1 *_g_fit;
        TF1 *_r_fit = new TF1("_r_fit", "0");

        const double _title_size = 21;

        Double_t xmin, xmax;

        void _fillresiduals();
        void _fitgraph();
        void _drawgraph();
        void _drawresiduals();

    public:
        graph(bool showresiduals = true);

        TPad *GetPad() { return _full_pad; };
        TGraphErrors *GetGraph() { return _g_graph; };
        TF1 *GetFitTF1() { return _g_fit; } // needed to get SetParameters() method;

        void SetGraph(TGraphErrors *clonegraph);
        void SetFitTF1(TF1 *clonefit);

        void SetFitFormula(std::string formula);
        void SetFitLimits(Double_t min, Double_t max);

        void SetLogX() { _logx = true; }
        void SetLogY() { _logx = true; }
        ~graph();
    };

    void graph::_fillresiduals()
    {

        if (!_show_res)
        {
            return;
        }
        if (_has_fitted)
        {
            for (int i = 0; i < _g_graph->GetN(); i++)
            {
                _r_graph->SetPoint(i, _g_graph->GetX()[i], (_g_graph->GetY()[i] - _g_fit->Eval(_g_graph->GetX()[i])) / _g_graph->GetEY()[i]);
                _r_graph->SetPointError(i, 0, 1);
            }
        }
    }

    void graph::SetFitFormula(std::string formula)
    {
        _g_fit = new TF1("_g_fit", formula.c_str());
    }

    void graph::SetFitLimits(Double_t min = (0.0), Double_t max = (1.0))
    {
        xmin = min;
        xmax = max;
        _r_fit->SetRange(min, max);
    }
    graph::graph(bool showresiduals)
    {
        if (!showresiduals)
        {
            _g_pad = new TPad("", "", 0.0, 0.3, 1.0, 1.0);
            _r_pad = new TPad("", "", 0.0, 0.0, 0.0, 0.0);
            _show_res = false;
            _r_pad->Delete();
        }
        _g_pad = new TPad("", "", 0.0, 0.3, 1.0, 1.0);
        _r_pad = new TPad("", "", 0.0, 0.0, 1.0, 0.295);

        // Alcuni accortezze da impostare inizializzando la classe
        _r_fit->SetLineStyle(2);
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
    std::string *_xtitle = new std::string("ERR: inverti set_ResidualAxis() #leftrightarrow set_TGraphAxis()");

    /**
     * @brief Set the TGraphAxis object
     * REMINDER: impostare prima il metodo @a set_ResidualAxis() e poi @a set_TGraphAxis() per ottenere il risultato voluto. 
     * 
     * @tparam _TObj General TObject from ROOT
     * @param rg Residuals dynamically declared TGraphError graph for residuals
     * @param ytitle y axis title
     * @param offset y axis offset, to be conjugated with the offser in @a set_ResidualAxis().
     * @param xtitle x axis title
     */
    template <class _TObj>
    void set_TGraphAxis(_TObj *g, std::string ytitle, float offset = 2, std::string xtitle = "")
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

    /**
     * @brief Set the ResidualsAxis object. 
     * REMINDER: impostare prima il metodo @a set_ResidualAxis() e poi @a set_TGraphAxis() per ottenere il risultato voluto. 
     * 
     * @tparam _TObj General TObject from ROOT (In this case always use TGraphErros typeclass)
     * @param rg Residuals dynamically declared TGraphError graph for residuals
     * @param xtitle x axis title
     * @param offset y axis offset, to be conjugated with the offser in @a set_TGraphAxis().
     * @param ytitle automatic set to "Residui [σ]"
     */
    template <class _TObj>
    void set_ResidualsAxis(_TObj *rg, std::string xtitle, float offset = 2, std::string ytitle = "Residui [#sigma]")
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

    struct padtypes
    {
        TPad *Graph = new TPad();
        TPad *Residuals = new TPad();
    };

    /**
     * @brief Impostazione dei pad del grafico
     * Il primo argomento è una struttura che contiene i TPad dei grafico e dei resudui;
     * Se dico di disegnare i residui tutto OK, se dico false al valore booleano drawresiduals
     * allora il grafico verrà eseguito senza i residui, e il nome degli assi impostato di
     * conseguenza. È importante chiamare i comandi @a set_ResidualAxis() e @a set_TGraphAxis()
     * in questo ordine, di modo che il titolo sull'asse x venga mostrato, altrimenti si mostrerà
     * un titolo di errore.
     * @return Imposta le dimensioni del grafico allocato dinamicamente
     */
    void setgraphsize(graphset::padtypes g, bool logx = false, bool logy = false, bool drawresiduals = true);

    /**
     * @brief Set canvas/pad division and margins
     * 
     * @tparam _TObj General ROOT TObject
     * @param c1 Dynamically allocated canvas/pad
     * @param nx number of division over x
     * @param ny number of division over y
     * @param m_left Left margin
     * @param m_right Right margin
     * @param m_bottom Bottom margin
     * @param m_top Top margin
     */
    template <class _TObj>
    void setcanvas(_TObj *c1, int nx = 1, int ny = 1,
                   float m_left = 0.16, float m_right = 0.06,
                   float m_bottom = 0.12, float m_top = 0.06)
    {
        c1->SetMargin(m_left, m_right, m_bottom, m_top);
        c1->SetFillStyle(4000);
        c1->Divide(nx, ny);
    }

    /**
     * @brief Fill residual TGraphErrors
     * 
     * @tparam _TObj General ROOT TObject
     * @tparam _TFObj General ROOT TObject (TF1 typeclass)
     * @param g Data graph
     * @param g_fit TF1 fitting curve
     * @param r Residual TGraphErrors
     */
    template <class _TObj, class _TFObj>
    void fillresiduals(_TObj *g, _TFObj *g_fit, TGraphErrors *r)
    {
        for (int i = 0; i < g->GetN(); i++)
        {
            r->SetPoint(i, g->GetX()[i], (g->GetY()[i] - g_fit->Eval(g->GetX()[i])) / g->GetEY()[i]);
            r->SetPointError(i, 0, 1);
        }
    }

    /**
     * @brief Includere qui tutti i parametri globali/semi-globali per l'inizializzazione di grafici in ROOT
     * @param gStyle->SetFrameLineWidth(0); Set graph only with two axis
     * @param gStyle->SetTextFont(43);      Set graph font to be Helvetica
     * @param gStyle->SetLineScalePS(1);    Set ok pdf scaling
     */
    void init();

    template <class _TObj, class _TFObj>
    void setmarker(_TObj *_g, _TFObj *_g_fit, Color_t lcolor = -1, Style_t mstyle = (Style_t)1, Size_t msize = (1.0F))
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
    void setmarker(_TObj *_g, Color_t lcolor = -1, Style_t mstyle = (Style_t)1, Size_t msize = (1.0F))
    {
        _g->SetMarkerStyle(mstyle);
        _g->SetMarkerSize(msize);
        if (lcolor != -1)
        {
            _g->SetLineColor(lcolor);
            _g->SetMarkerColor(lcolor);
        }
    }
}

// * todo: fare in modo da avere che riempendo il grafico in
// *       automatico se io genero i residui sono già riempiti

#endif