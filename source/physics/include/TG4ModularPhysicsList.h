// $Id: TG4ModularPhysicsList.h,v 1.2 2003/06/03 17:15:49 brun Exp $
/// \ingroup physics
//
/// \class TG4ModularPhysicsList
/// 
/// The Geant4 VMC modular physics list.
/// The default physics processes and particles are created
/// using the G4VPhysicsCreator derived classes
/// and registered to this physics list in the function Configure().
/// User can override this function and instatiate his own
/// physics constructor classes.
///
/// Author: I. Hrivnacova

#ifndef TG4_MODULAR_PHYSICS_LIST_H
#define TG4_MODULAR_PHYSICS_LIST_H

#include "TG4Verbose.h"
#include "TG4PhysicsListMessenger.h"

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

class TG4ModularPhysicsList: public G4VModularPhysicsList,
                             public TG4Verbose
{
  public:
    TG4ModularPhysicsList();
    // --> protected
    // TG4ModularPhysicsList(const TG4ModularPhysicsList& right);
    virtual ~TG4ModularPhysicsList();
  
    // methods
    virtual void Configure();
    virtual void SetCuts();
    virtual void VerboseLevel(G4int level);
    void PrintAllProcesses() const;
    void DumpAllProcesses() const;

    // set methods
    void SetRangeCut(G4double value);
    void SetProcessActivation();
    void SetEMPhysics(G4bool value);
    void SetMuonPhysics(G4bool value);
    void SetHadronPhysics(G4bool value);
    void SetOpticalPhysics(G4bool value);
    void SetSpecialCutsPhysics(G4bool value);
    void SetSpecialControlsPhysics(G4bool value);
    
  protected:
    TG4ModularPhysicsList(const TG4ModularPhysicsList& right);

    // operators
    TG4ModularPhysicsList& operator=(const TG4ModularPhysicsList& right);

    // methods
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // static data members
    static const G4double  fgkDefaultCutValue; //default cut value

    // data members
    TG4PhysicsListMessenger  fMessenger; //messenger
    G4bool  fSetEMPhysics;          //electromagnetic physics control
    G4bool  fSetMuonPhysics;        //muon physics control
    G4bool  fSetHadronPhysics;      //hadron physics control
    G4bool  fSetOpticalPhysics;     //optical physics control
    G4bool  fSetSpecialCutsPhysics; //special cuts process control 
                                    //(under development)                  
    G4bool  fSetSpecialControlsPhysics;//special controls process control
                                    //(under development)
  private:
    // methods
    void SetProcessActivation(G4ProcessManager* processManager,
                              G4int processId, G4bool activation);
};

// inline functions

inline void TG4ModularPhysicsList::SetEMPhysics(G4bool value) { 
  /// Swith on/off EM physics
  fSetEMPhysics = value; 
}

inline void TG4ModularPhysicsList::SetMuonPhysics(G4bool value) { 
  /// Switch on/off muon physics
  fSetMuonPhysics = value; 
}

inline void TG4ModularPhysicsList::SetHadronPhysics(G4bool value) { 
  /// Switch on/off hadron physics
  fSetHadronPhysics = value; 
}

inline void TG4ModularPhysicsList::SetOpticalPhysics(G4bool value) { 
  /// Switch on/off optical physics
  fSetOpticalPhysics = value; 
}

inline void TG4ModularPhysicsList::SetSpecialCutsPhysics(G4bool value) { 
  /// Switch on/off special cuts = cuts in energy defined via VMC
  fSetSpecialCutsPhysics = value; 
}

inline void TG4ModularPhysicsList::SetSpecialControlsPhysics(G4bool value) { 
  /// Switch on/off special controls = process controls defined via VMC
  fSetSpecialControlsPhysics = value; 
}

#endif //TG4_MODULAR_PHYSICS_LIST_H

