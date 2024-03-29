#include <TH1D.h>
#include<TF1.h>
#include <TCanvas.h>
#include <fstream>

#include"LabTools.h"

void RMS(){

    graphset::init();

    TCanvas *c1 = new TCanvas("", "", 600, 500);
    c1->SetLogy();
    TF1* f = new TF1("f", "gaus(0)");
    f->SetLineColor(kRed);
    std::ifstream file("../dati/valutazione_rumore.txt");
    double T, V, eV, range;
    double crange;
    TH1D *hist = new TH1D("", "", 20, -40e-3, 40e-3);
    graphset::set_TGraphAxis(hist, "", 2, "V_{out} (con V_{in} = 0V) [V]");
    graphset::setmarker(hist, kRed-3);
    while (file >> T >> V >> eV >> range >> crange){hist->Fill(V);}
    hist->Draw("HIST");
    f->SetParameters(100, 0, 0.01);
    hist->Fit("f", "", "same");
    std::cout << "RMS " << hist->GetRMS() << " +- " << hist->GetRMSError() << std::endl
    << "Std Dev " << hist->GetStdDev() << " +- " << hist->GetStdDevError() << std::endl;
    c1->SaveAs("../fig/RMS.pdf");
}

#ifndef __CLING__
int main(){
    RMS();
    return 0;
}
#endif