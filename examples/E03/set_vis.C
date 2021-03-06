//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file E03/set_vis.C
/// \brief Macro for setting visualization for Example03 

#include "TVirtualMC.h"

void set_vis()
{
/// Macro for setting visualization for Example03 

  if ( TString(gMC->GetName()) == "TGeant3TGeo" ) {
    // Set drawing options
    TGeoVolume* vol;
    vol = gGeoManager->GetVolume("ABSO");
    if (vol) vol->SetLineColor(kMagenta);
    vol = gGeoManager->GetVolume("GAPX");
    if (vol) vol->SetLineColor(kYellow);
    gMC->SetCollectTracks(kTRUE);
  }    

  if (TString(gMC->GetName()) == "TGeant4") {
    // Setting Geant4 visualization
   ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");
  }  
}  
