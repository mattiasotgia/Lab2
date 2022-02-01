/**
 * @file Analysis.h
 * @author Mattia Sotgia (mattiasotgia01@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LABTOOLS_BODE_Analysis
#define LABTOOLS_BODE_Analysis

#include<iostream>
#include<vector>

#include<TCanvas.h>
#include<TF1.h>
#include<TGraphErrors.h>

class Bode{
private:
    bool                _isfunctioncalled = false; ///> check if user called for function different from base;

    const char         *_gainfit = ""; // todo: fill function
    const char         *_phasefit = ""; // todo: fill function

    /// graphical objects []
    TCanvas            *fFigure;
    TGraphErrors       *fGain;
    TGraphErrors       *fPhase;
    TF1                *fGainFit; 
    TF1                *fPhaseFit;

    /// function variables declaration
    Double_t            fmin = (0.0);   ///> minimum for frequency range
    Double_t            fmax = (1.0);   ///> maximum for frequency range
    Double_t           *fPointGain;     ///>[fNpoints] array for gain points
    Double_t           *fPErrGain;      ///>[fNpoints] array for error gain points
    Double_t           *fPointPhase;    ///>[fNpoints] array for phase points
    Double_t           *fPErrPhase;     ///>[fNpoints] array for error phase points
public:
    Bode();
    Bode(const char *filename, Option_t *option="");
    ~Bode();
    void                FitCorrelated(Option_t *option="", Option_t *goption="", Axis_t xmin=0, Axis_t xmax=0);
    bool                ReadInput(const char *filename, Option_t *option="");       ///> read input for both phase and gain data 
    bool                ReadInputGain(const char *filename, Option_t *option="");   ///> read input for gain data
    bool                ReadInputPhase(const char *filename, Option_t *option="");  ///> read input for phase data
    // bool                ReadInputRDF()  // TO BE IMPLEMENTED
    void                SetGainFunction(const char *formula, Option_t *option="");
    bool                SetGainVec(std::vector<Double_t> Gain, std::vector<Double_t> ErrGain);
    void                SetPhaseFunction(const char *formula, Option_t *option="");
    bool                SetPhaseVec(std::vector<Double_t> Phase, std::vector<Double_t> ErrPhase);
    void                SetLogx();
    void                SetLogy();
};

#endif
