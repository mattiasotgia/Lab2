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
#ifndef LABTOOLS_ATLASStyle
#define LABTOOLS_ATLASStyle

#include<TStyle.h>
TStyle *atlas_style(Size_t tsize=29);

#endif //LABTOOLS_ATLASStyle