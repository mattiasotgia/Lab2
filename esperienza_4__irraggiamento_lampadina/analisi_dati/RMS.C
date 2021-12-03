#include <TH1D.h>
#include<TF1.h>
#include <TCanvas.h>
#include <fstream>

void RMS(){
    gStyle->SetFrameLineWidth(0);
    gStyle->SetTextFont(43);
    gStyle->SetLineScalePS(1);

    TCanvas *c1 = new TCanvas("", "", 600, 500);
    TF1* f = new TF1("f", "gaus(0)");
    f->SetLineColor(kRed);
    std::ifstream file("../dati/valutazione_rumore.dat");
    double T, V, eV, range;
    char crange;
    TH1D *hist = new TH1D("", "", 20, -40e-3, 40e-3);
    while (file >> T >> V >> eV >> range >> crange)
    {
        hist->Fill(V);
    }
    hist->Draw("HIST E");
    f->SetParameters(100, 0, 0.01);
    hist->Fit("f", "", "same");
    std::cout << "RMS " << hist->GetRMS() << " +- " << hist->GetRMSError() << std::endl
    << "Std Dev " << hist->GetStdDev() << " +- " << hist->GetStdDevError() << std::endl;
    c1->SaveAs("../fig/RMS.pdf");
}

#ifndef __CINT__
int main(){
    RMS();
    return 0;
}
#endif