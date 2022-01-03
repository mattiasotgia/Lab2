#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TMath.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TApplication.h>

#include "LabTools.h"
#include "ErrorAnalysis.h"

int main(int argc, char const *argv[])
{
    TApplication *app = new TApplication("app", 0, NULL);
    graphset::init();
    Color_t mk[8] = {kRed, kOrange, kGreen - 5, kAzure, kViolet - 8, kGray + 1, kBlue - 3, kSpring - 6};
    double d[] = {0.02, 0.04, 0.05, 0.06, 0.08, 0.1, 0.12, 0.2};
    double size_d = sizeof(d)/sizeof(d[0]);
    double errore_tensione = 0.0044;
    double errore_distanza = 0.006 / sqrt(3) * 2;

    std::string pfix = "../dati/_zeroed/";
    std::string all_data_table[] = {
        "dati_2.txt",  //> 0
        "dati_4.txt",  //> 1
        "dati_5.txt",  //> 2
        "dati_6.txt",  //> 3
        "dati_8.txt",  //> 4
        "dati_10.txt", //> 5
        "dati_12.txt", //> 6
        "dati_20.txt"  //> 7
    };
    const int full_dataT_s = sizeof(all_data_table)/sizeof(all_data_table[0]);

    std::ifstream data_table;

    TCanvas *c6 = new TCanvas("", "", 1000, 400);
    graphset::setcanvas(c6, 2, 1);

    double dV_mod2[8];
    double err_dV_mod2[8];

    double eval[22] = {10, 12.5, 15, 17.5, 20, 22.5, 25, 27.5, 30, 32.5, 35, 40, 42.5, 45, 50, 55, 60, 65, 70, 75};
    int h = 4;

    std::cout << "time: " << eval[h] << std::endl
              << std::endl;

    graphset::setcanvas(c6->cd(1));

    TLegend *l2 = new TLegend(0.2, 0.2, 0.6, 0.5);
    l2->SetHeader("#bf{MODELLO #2}");
    l2->SetBorderSize(0);
    l2->SetFillColorAlpha(kWhite, 0);

    TGraphErrors *plot2 = new TGraphErrors();
    TF1 *fit_mod2 = new TF1("fit_mod2", "[0]*pow((x+[2]), [1])", 0.02, 0.25);
    fit_mod2->SetParameters(1, -2, 0);


    for (int k = 0; k < full_dataT_s; k++)
    {
        TGraphErrors *g2 = new TGraphErrors();
        std::ifstream data((pfix + all_data_table[k]).c_str());
        std::string dist = all_data_table[k].substr(all_data_table[k].find("_") + 1,
                                                    all_data_table[k].find(".") - all_data_table[k].find("_") - 1);

        g2->SetName(("g2_" + dist).c_str());

        double t1, v1, ev1, range1;

        data >> t1 >> v1 >> ev1 >> range1;

        g2->SetPoint(0, 0, 0);
        g2->SetPointError(0, 0, errore_tensione);
        const double t0 = t1; // zeroing data
        const double v0 = v1;

        for (int j = 1; (data >> t1 >> v1 >> ev1 >> range1) && (j < 90); j++)
        {
            if (k == 3 && t1 < 68 && t1 > 67.5)
                continue; // tolto punto doppio per d=6cm
            g2->SetPoint(j, t1 - t0, v1 - v0);
            g2->SetPointError(j, 0, errore_tensione);
        }

        TF1 *mod2 = new TF1(("mod2_" + dist).c_str(), "[0]*x+[1]", eval[h] - 2, eval[h] + 2);
        graphset::setmarker(g2, mod2, mk[k], 21, 0.4);

        g2->Draw((k == 0) ? "ap" : "p");
        graphset::set_TGraphAxis(g2, "Voltage [V]", 1, "Time after t=0 [s]");
        g2->Fit(("mod2_" + dist).c_str(), "QR"); // fit intorno 20s
        l2->AddEntry(("g2_" + dist).c_str(), ("d=" + dist + "#times10^{-2} m").c_str());
        g2->GetXaxis()->SetLimits(-1, 120);

        dV_mod2[k] = mod2->Eval(eval[h]);
        err_dV_mod2[k] = 0.0044; //> uso per la tensione lo stesso errore che ho nella sua lettura
        std::cout << "[[" << mod2->Eval(eval[h]) << " +- " << err_dV_mod2[k] << "], ["
                  << d[k] << " +- " << errore_distanza << "]]" << std::endl;

        data.close();
    }
    std::cout << std::endl;
    l2->Draw();

    // plot2

    for (int j = 0; j < size_d; j++)
    {
        plot2->SetPoint(j, d[j], abs(dV_mod2[j]));
        plot2->SetPointError(j, errore_distanza, err_dV_mod2[j]);
    }

    graphset::setcanvas(c6->cd(2));
    plot2->Draw("ap Z");
    graphset::set_TGraphAxis(plot2, "Voltage after 20s [V/s]", 1, "Source distance [m]");
    plot2->Fit("fit_mod2", "R");

    TGraphErrors *plot2_sup = (TGraphErrors *)plot2->Clone();
    plot2_sup->SetPointError(0, errore_distanza + 0.005 / sqrt(3), err_dV_mod2[0]);
    plot2_sup->SetLineColor(kRed);
    plot2_sup->Draw("p []");

    logs::print_stat(fit_mod2);

    std::cout << std::endl
              << "d^( " << fit_mod2->GetParameter(1) << " +- " << fit_mod2->GetParError(1) << " )" << std::endl
              << "offset (sys): " << fit_mod2->GetParameter(2) << " +- " << fit_mod2->GetParError(2) << std::endl;

    std::cout << "Compatibilità di 2: " << stattools::compatible(-2, 0, fit_mod2->GetParameter(1), fit_mod2->GetParError(1)) << std::endl;
    std::cout << "compatibilità di zero: " << // offset con stima (6/sqrt(3) mm): " <<
        stattools::compatible(0, 0, fit_mod2->GetParameter(2), fit_mod2->GetParError(2)) << std::endl;

    logs::print_mmsg("Correzione sul primo punto:");

    plot2_sup->Fit("fit_mod2", "R");

    logs::print_stat(fit_mod2);
    std::cout << std::endl
              << "d^( " << fit_mod2->GetParameter(1) << " +- " << fit_mod2->GetParError(1) << " )" << std::endl
              << "offset (sys): " << fit_mod2->GetParameter(2) << " +- " << fit_mod2->GetParError(2) << std::endl;

    std::cout << "Compatibilità di 2: " << stattools::compatible(-2, 0, fit_mod2->GetParameter(1), fit_mod2->GetParError(1)) << std::endl;
    std::cout << "compatibilità di zero: " << // offset con stima (6/sqrt(3) mm): " <<
        stattools::compatible(0, 0, fit_mod2->GetParameter(2), fit_mod2->GetParError(2)) << std::endl;

    c6->Draw();
    c6->SaveAs("../fig/plot_mod2.pdf");

    app->Run(true);
    return EXIT_SUCCESS;
}
