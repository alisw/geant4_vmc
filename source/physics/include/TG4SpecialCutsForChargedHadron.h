// $Id: TG4SpecialCutsForChargedHadron.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup physics
//
/// \class TG4SpecialCutsForChargedHadron
/// 
/// Special process that activates kinetic energy cuts
/// for charged hadrons.
///
/// Author: I. Hrivnacova

#ifndef TG4_SPECIAL_CUTS_FOR_CHARGED_HADRON_H
#define TG4_SPECIAL_CUTS_FOR_CHARGED_HADRON_H

#include "TG4VSpecialCuts.h"

class TG4Limits;

class G4Track;

class TG4SpecialCutsForChargedHadron: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForChargedHadron(const G4String& processName 
                                                   = "specialCutForChHadron");
    // --> protected
    // TG4SpecialCutsForChargedHadron();		   
    virtual ~TG4SpecialCutsForChargedHadron();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForChargedHadron();		   
};

#endif //TG4_SPECIAL_CUTS_FOR_CHARGED_HADRON_H



