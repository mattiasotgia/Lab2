// Lab Analysis Toolset - CorePlot - "LabPlotCore" -*- C++ -*-
// Author M. Sotgia 14/01/22
// v0.1.0 alpha

#ifndef LABTOOLS_LabPlotCore
#define LABTOOLS_LabPlotCore

#include<TROOT.h>
#include<TCanvas.h>

class Axes{
private:
    
public:
    Axes();
    ~Axes();
}; // Axes class

class Figure{
private:
    TCanvas *_canvas;
    Axes _ax; // ! TODO: define Axes object class !
public:
    Figure(const char *name="", const char *title="", Size_t figsizex=800, Size_t figsizey=600);
    
    TCanvas *canvas() { return _canvas; };
    Axes axes() { return _ax; };

    void add_axes(Axes ax);

    ~Figure();
}; // Figure class



#endif