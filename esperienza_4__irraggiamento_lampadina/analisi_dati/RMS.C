#include<TH1D.h>
#include<fstream>

void RMS(){
    std::ifstream file("../dati/valutazione_rumore.dat");
    double T, V, eV, range;
    char crange;
    TH1D* hist = new TH1D("", "", 20, -40e-3, 40e-3);
    while(file >> T >> V >> eV >> range >> crange){
        hist->Fill(V);
    }
    hist->Draw("HIST E");
}

#ifndef __CINT__
int main(){
    RMS();
    return 0;
}
#endif