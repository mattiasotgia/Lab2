// Lab Analysis Toolset - CorePlot - "LabPlotCore" -*- C++ -*-
// Author M. Sotgia 14/01/22
// v0.1.0 alpha

#include<iostream>
#include<fstream>

#include<TROOT.h>
#include<TCanvas.h>

#include"LabPlotCore.h"

template<typename Base, typename T>
inline bool isinstance(const T*){
    return std::is_base_of<Base, T>::value;
}


Figure::Figure(const char *name, const char *title, Size_t figsizex, Size_t figsizey){
    _canvas = new TCanvas(name, title, figsizex, figsizey);
    _canvas->Update();
}

void Figure::add_axes(Axes ax){
    _canvas->cd();
    _ax = ax;
    _canvas->Modified();
}

