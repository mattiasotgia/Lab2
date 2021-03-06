// ********************************************************** //
//     ___  ________    ___   _____    _____ __        __     //
//    /   |/_  __/ /   /   | / ___/   / ___// /___  __/ /__   //
//   / /| | / / / /   / /| | \__ \    \__ \/ __/ / / / / _ \  //
//  / ___ |/ / / /___/ ___ |___/ /   ___/ / /_/ /_/ / /  __/  //
// /_/  |_/_/ /_____/_/  |_/____/   /____/\__/\__, /_/\___/   //
//                                           /____/           //
// ********************************************************** //

/*
atlasplots.atlasstyle
~~~~~~~~~~~~~~~~~~~~~
Python implementation of the ATLAS PubCom style guide for plots in ROOT:
    https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PubComPlotStyle
This implementation uses ROOT fonts with precision = 3, where text sizes are
given in pixels. Using text sizes in pixels is useful for canvases with
multiple TPads of different sizes, as the text size will not depend on the
dimensions of the pad. The default font used by the ATLAS style is Arial (ROOT
font 43).
For general instructions on formatting text in ROOT, refer to,
    https://root.cern.ch/doc/master/classTAttText.html
*/

#include<TStyle.h>

#include"ATLASStyle.h"

TStyle *atlas_style(Size_t tsize, Bool_t ticky){

    TStyle *atlasStyle = new TStyle("ATLAS", "Atlas Style");

    // Use plain black on white colors
    Color_t icol = 0;  // White
    atlasStyle->SetFrameBorderMode(icol);
    atlasStyle->SetFrameFillColor(icol);
    atlasStyle->SetCanvasBorderMode(icol);
    atlasStyle->SetCanvasColor(icol);
    atlasStyle->SetPadBorderMode(icol);
    atlasStyle->SetPadBorderSize(icol);
    atlasStyle->SetPadColor(icol);
    atlasStyle->SetStatColor(icol);

    // Set the paper & margin sizes
    atlasStyle->SetPaperSize(20, 26);

    // Set margin sizes
    atlasStyle->SetPadTopMargin(0.05);
    atlasStyle->SetPadRightMargin(ticky? 0.05:0.16);
    atlasStyle->SetPadBottomMargin(0.16);
    atlasStyle->SetPadLeftMargin(0.16);

    // Set title offsets (for axis labels)
    atlasStyle->SetTitleXOffset(1.4);
    atlasStyle->SetTitleYOffset(1.4);

    // Set font
    Size_t font = 43;  // Arial

    atlasStyle->SetTextFont(font);
    atlasStyle->SetTextSize(tsize);

    atlasStyle->SetLabelFont(font, "x");
    atlasStyle->SetTitleFont(font, "x");
    atlasStyle->SetLabelFont(font, "y");
    atlasStyle->SetTitleFont(font, "y");
    atlasStyle->SetLabelFont(font, "z");
    atlasStyle->SetTitleFont(font, "z");

    atlasStyle->SetLabelSize(tsize, "x");
    atlasStyle->SetTitleSize(tsize, "x");
    atlasStyle->SetLabelSize(tsize, "y");
    atlasStyle->SetTitleSize(tsize, "y");
    atlasStyle->SetLabelSize(tsize, "z");
    atlasStyle->SetTitleSize(tsize, "z");

    // Use bold lines and markers
    atlasStyle->SetMarkerStyle(20);
    atlasStyle->SetMarkerSize(0.8);
    atlasStyle->SetHistLineWidth(1);
    atlasStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes

    // Get rid of error bar caps
    atlasStyle->SetEndErrorSize(0.);

    // Do not display any of the standard histogram decorations
    atlasStyle->SetOptTitle(0);
    atlasStyle->SetOptStat(0);
    atlasStyle->SetOptFit(0);

    // Put tick marks on top and RHS of plots
    atlasStyle->SetPadTickX(1);
    atlasStyle->SetPadTickY(1);

    // Remove legend borders and set font
    atlasStyle->SetLegendBorderSize(0);
    atlasStyle->SetLegendFillColor(0);
    atlasStyle->SetLegendFont(43);
    atlasStyle->SetLegendTextSize(tsize);

    // Miscellaneous
    atlasStyle->SetLineScalePS(1);

    return atlasStyle;
}