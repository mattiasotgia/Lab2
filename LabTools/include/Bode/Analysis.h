#ifndef LABTOOLS_BODE_Analysis
#define LABTOOLS_BODE_Analysis

#include<iostream>
#include<TGraphErrors.h>
#include<TF1.h>

class Bode{
private:
    bool                _isfunctioncalled; //> check if user called for function different from base;

    const char         *_gainfit = ""; // todo: fill function
    const char         *_phasefit = ""; // todo: fill function

    Int_t               fmin = (0.0); //> minimum for frequency range
    Int_t               fmax = (1.0); //> maximum for frequency range

    TGraphErrors       *fGain;
    TGraphErrors       *fPhase;
    TF1                *fGainFit; 
    TF1                *fPhaseFit;
public:
    Bode();
    ~Bode();
    inline void         SetLogx();
    inline void         SetLogy();
};

#endif
