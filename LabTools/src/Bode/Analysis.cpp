/**
 * @file Analysis.cpp
 * @author Mattia Sotgia (mattiasotgia01@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<iostream>

#include<TCanvas.h>
#include<TF1.h>
#include<TGraphErrors.h>

#include"Bode/Analysis.h"
#include"ErrorAnalysis.h"
#include"LabPlot.h" // set_atlas_style() called from here
#include"Logger.h"

Bode::Bode(){
    set_atlas_style(tsize);
    fGain = new TGraphErrors();
    fPhase = new TGraphErrors();
    fGainFit = new TF1("gainfit", _gainfit, fmin, fmax);
    fPhaseFit = new TF1("phasefit", _phasefit, fmin, fmax);
}

Bode::Bode(System_t sys){
    fSystem = sys;
    set_atlas_style(tsize);
    fGain = new TGraphErrors();
    fPhase = new TGraphErrors();
    fGainFit = new TF1("gainfit", _gainfit, fmin, fmax);
    fPhaseFit = new TF1("phasefit", _phasefit, fmin, fmax);
}

Bode::Bode(System_t sys, const char *filename, Option_t *option){
    fSystem = sys;
    set_atlas_style(tsize);
    fGain = new TGraphErrors();
    fPhase = new TGraphErrors();
    fGainFit = new TF1("gainfit", _gainfit, fmin, fmax);
    fPhaseFit = new TF1("phasefit", _phasefit, fmin, fmax);
}

bool Bode::SetGainVec(std::vector<Double_t> Gain, std::vector<Double_t> ErrGain){
    fPointGain = Gain.data(); 
    fPErrGain = ErrGain.data();

    if (Gain.size()!=ErrGain.size()){
        printf("%s", Logger::error("array size of Gain and its Error do not match!"));
        return false;
    }

    Int_t kSizePG = sizeof(fPointGain)/sizeof(Double_t);
    Int_t kSizeEPG = sizeof(fPErrGain)/sizeof(Double_t);

    if(Gain.size()!=kSizePG || ErrGain.size()!=kSizeEPG){
        printf("%s", Logger::error(Form("size of Gain input vec: %lu, size of Gain copy vec: %d", Gain.size(), kSizePG)));
        printf("%s", Logger::error(Form("size of ErrGain input vec: %lu, size of ErrGain copy vec: %d", Gain.size(), kSizePG)));
        return false;
    }
    return true;
}

bool Bode::SetPhaseVec(std::vector<Double_t> Phase, std::vector<Double_t> ErrPhase){
    fPointPhase = Phase.data(); 
    fPErrPhase = ErrPhase.data();

    if (Phase.size()!=ErrPhase.size()){
        printf("%s", Logger::error("array size of Phase and its Error do not match!"));
        return false;
    }

    Int_t kSizePG = sizeof(fPointPhase)/sizeof(Double_t);
    Int_t kSizeEPG = sizeof(fPErrPhase)/sizeof(Double_t);

    if(Phase.size()!=kSizePG || ErrPhase.size()!=kSizeEPG){
        printf("%s", Logger::error(Form("size of Phase input vec: %lu, size of Phase copy vec: %d", Phase.size(), kSizePG)));
        printf("%s", Logger::error(Form("size of ErrPhase input vec: %lu, size of ErrPhase copy vec: %d", Phase.size(), kSizePG)));
        return false;
    }
    return true;
}

Bode::~Bode(){
    fprintf(stderr, "%s\n", Logger::warning("Deleted obj Bode"));
}