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
} // namespace stat

namespace graphset
{   
    void set_TGraphAxis(TGraphErrors *g, std::string ytitle, float offset = 2){
        g->SetTitle("");
        g->GetYaxis()->SetTitle(ytitle.c_str());
        g->GetYaxis()->SetTitleOffset(offset);
        g->GetYaxis()->SetTitleFont(43);
        g->GetYaxis()->SetTitleSize(title_size);
        g->GetYaxis()->SetLabelFont(43);
        g->GetYaxis()->SetLabelSize(12);
        g->GetYaxis()->CenterTitle();

        g->GetXaxis()->SetTickLength(0.05);
    }

    void set_ResidualsAxis(TGraphErrors *rg, std::string xtitle, float offset = 2, std::string ytitle = "Residui [#sigma]"){
        rg->GetXaxis()->SetTitle(xtitle.c_str());
        rg->GetXaxis()->SetTitleOffset(5);
        rg->GetXaxis()->SetTitleFont(43);
        rg->GetXaxis()->SetTitleSize(title_size);

        rg->GetYaxis()->SetTitle(ytitle.c_str());
        rg->GetYaxis()->SetTitleOffset(offset);
        rg->GetYaxis()->SetTitleFont(43);
        rg->GetYaxis()->SetTitleSize(title_size);
        rg->GetYaxis()->CenterTitle();

        rg->GetYaxis()->SetLabelFont(43);
        rg->GetYaxis()->SetLabelSize(12);
        rg->GetYaxis()->SetNdivisions(5, 5, 0);
        rg->GetXaxis()->SetLabelFont(43);
        rg->GetXaxis()->SetLabelSize(12);
        rg->GetXaxis()->CenterTitle();

        rg->GetXaxis()->SetTickLength(0.08);
    }

    // class graph{
    // private:
    //     const double _title_size = 21;
        
    // public:
    //     graph(/* args */);
    //     ~graph();
    // };
    
    // graph::graph(/* args */){
    // }
    
    // graph::~graph(){
    // }
    
    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    //                    FUNZIONI STABILI SENZA CLASSE                        //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////

    struct padtypes{
        TPad* Graph = new TPad("", "", 0.0, 0.3, 1.0, 1.0);
        TPad* Residuals = new TPad("", "", 0.0, 0.0, 1.0, 0.295);
    };

    void setgraphsize(graphset::padtypes g, bool logx=false, bool logy=false){
        g.Graph->SetMargin(0.14, 0.06, 0.0, 0.06);
        g.Residuals->SetMargin(0.14, 0.06, 0.4, 1.0);
        if(logx){
            g.Graph->SetLogx();
            g.Residuals->SetLogx();
        }
        if(logy){
            g.Graph->SetLogy();
        }
        g.Graph->Draw();
        g.Residuals->Draw();
    }

    void setcanvas(TCanvas* c1, int nx, int ny, 
                float m_left = 0.16, float m_right = 0.06, 
                float m_bottom = 0.12, float m_top = 0.06){
        c1->SetMargin(m_left, m_right, m_bottom, m_top);
        c1->SetFillStyle(4000);
        c1->Divide(nx, ny);
    }
}

// * todo: fare in modo da avere che riempendo il grafico in 
// *       automatico se io genero i residui sono già riempiti

#endif