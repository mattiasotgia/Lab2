
#include<iostream>


#include"Bode/Analysis.h"
#include"LabPlot.h" // set_atlas_style() called from here

Bode::Bode(){
    fGain = new TGraphErrors();
    fPhase = new TGraphErrors();
    fGainFit = new TF1("gainfit", _gainfit, fmin, fmax);
    fPhaseFit = new TF1("phasefit", _phasefit, fmin, fmax);
}
