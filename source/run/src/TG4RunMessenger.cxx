// $Id: TG4RunMessenger.cxx,v 1.2 2002/12/18 09:35:32 brun Exp $
// Category: run
//
// Class TG4RunMessenger
// ---------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4RunMessenger.h"
#include "TG4RunManager.h"
#include "TG4Globals.h"
#include "TG4UICmdWithAComplexString.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>

//_____________________________________________________________________________
TG4RunMessenger::TG4RunMessenger(TG4RunManager* runManager)
  : fRunManager(runManager)
{ 
//
  fDirectory = new G4UIdirectory("/mcControl/");
  fDirectory->SetGuidance("TGeant4 control commands.");

  fRootCmd = new G4UIcmdWithoutParameter("/mcControl/root", this);
  fRootCmd->SetGuidance("Switch to Root interactive shell.");
  fRootCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle, 
                               G4State_GeomClosed, G4State_EventProc);

  fRootMacroCmd = new G4UIcmdWithAString("/mcControl/rootMacro", this);
  fRootMacroCmd->SetGuidance("Process Root macro with given name (from file name.C)");
  fRootMacroCmd->SetParameterName("macroName", true);
  fRootMacroCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle, 
                                    G4State_GeomClosed, G4State_EventProc);

  fRootCommandCmd = new TG4UICmdWithAComplexString("/mcControl/rootCmd", this);
  fRootCommandCmd->SetGuidance("Process Root command");
  fRootCommandCmd->SetParameterName("command", false);
  fRootCommandCmd->SetDefaultValue(" ");
  fRootCommandCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle, 
                                      G4State_GeomClosed, G4State_EventProc);

  fG3DefaultsCmd = new G4UIcmdWithoutParameter("/mcControl/g3Defaults", this);
  fG3DefaultsCmd->SetGuidance("Set G3 default parameters (cut values,");
  fG3DefaultsCmd->SetGuidance("tracking media max step values, ...)");
  fG3DefaultsCmd->AvailableForStates(G4State_PreInit);
}

//_____________________________________________________________________________
TG4RunMessenger::TG4RunMessenger(){
//
} 

//_____________________________________________________________________________
TG4RunMessenger::TG4RunMessenger(const TG4RunMessenger& right) {
// 
  TG4Globals::Exception("TG4RunMessenger is protected from copying.");
}

//_____________________________________________________________________________
TG4RunMessenger::~TG4RunMessenger() {
//
  delete fDirectory;
  delete fRootCmd;
  delete fRootMacroCmd;
  delete fRootCommandCmd;
  delete fG3DefaultsCmd;
}

//
// operators
//

//_____________________________________________________________________________
TG4RunMessenger& TG4RunMessenger::operator=(const TG4RunMessenger& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception("TG4RunMessenger is protected from assigning.");
    
  return *this;  
}    
          
//
// public methods
//

//_____________________________________________________________________________
void TG4RunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
/// Apply command to the associated object.

  if (command == fRootCmd) {
    fRunManager->StartRootUI(); 
  }
  else if (command == fRootMacroCmd) {  
    fRunManager->ProcessRootMacro(newValue); 
  }
  else if (command == fRootCommandCmd) {
    fRunManager->ProcessRootCommand(newValue); 
  }
  else if (command == fG3DefaultsCmd) {
    fRunManager->UseG3Defaults(); 
  }
}
