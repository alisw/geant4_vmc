// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007-2010 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4FieldParameters.cxx
/// \brief Implementation of the TG4FieldParameters class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4FieldParameters.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"

#include <TVirtualMCApplication.h>
#include <TVirtualMC.h>

const G4double  TG4FieldParameters::fgkDefaultStepMinimum = 0.01*mm;
const G4double  TG4FieldParameters::fgkDefaultDeltaChord = 0.25 * mm;
const G4double  TG4FieldParameters::fgkDefaultDeltaOneStep = 0.01*mm;
const G4double  TG4FieldParameters::fgkDefaultDeltaIntersection = 0.001*mm;
const G4double  TG4FieldParameters::fgkDefaultMinimumEpsilonStep = 5.0e-5;
const G4double  TG4FieldParameters::fgkDefaultMaximumEpsilonStep = 0.001;

//
// static methods
//

//_____________________________________________________________________________
G4String TG4FieldParameters::EquationTypeName(EquationType equation) 
{
/// Return the equation type as a string

  switch ( equation ) {
    case kMagUsualEqRhs:     return G4String("MagUsualEqRhs"); 
    case kMagSpinEqRhs:      return G4String("MagSpinEqRhs"); 
    case kEqMagElectric:     return G4String("EqMagElectric");
    case kEqEMFieldWithSpin: return G4String("EqEMFieldWithSpin"); 
    case kEqEMFieldWithEDM:  return G4String("EqEMFieldWithEDM"); 
  } 
  
  TG4Globals::Exception(
    "TG4FieldParameters", "EquationTypeName:",
    "Unknown equation value.");
  return G4String();  
}      

//_____________________________________________________________________________
G4String TG4FieldParameters::StepperTypeName(StepperType stepper)
{
/// Return the stepper type as a string

  switch ( stepper ) {
    case kCashKarpRKF45:      return G4String("CashKarpRKF45");
    case kClassicalRK4:       return G4String("ClassicalRK4");
    case kExplicitEuler:      return G4String("ExplicitEuler");
    case kImplicitEuler:      return G4String("ImplicitEuler");
    case kSimpleHeum:         return G4String("SimpleHeum");
    case kSimpleRunge:        return G4String("SimpleRunge");
    case kConstRK4:           return G4String("ConstRK4");
    case kExactHelixStepper:  return G4String("ExactHelixStepper");
    case kHelixExplicitEuler: return G4String("HelixExplicitEuler");
    case kHelixHeum:          return G4String("HelixHeum");
    case kHelixImplicitEuler: return G4String("HelixImplicitEuler");
    case kHelixMixedStepper:  return G4String("HelixMixedStepper");
    case kHelixSimpleRunge:   return G4String("HelixSimpleRunge");
    case kNystromRK4:         return G4String("NystromRK4");
    case kRKG3Stepper:        return G4String("RKG3_Stepper");
  }  
  
  TG4Globals::Exception(
    "TG4FieldParameters", "StepperTypeName:",
    "Unknown stepper value.");
  return G4String();  
}      

//_____________________________________________________________________________
EquationType TG4FieldParameters::GetEquationType(const G4String& name)
{
/// Return the equation type for given equation type name

  if ( name == EquationTypeName(kMagUsualEqRhs) )     return kMagUsualEqRhs; 
  if ( name == EquationTypeName(kMagSpinEqRhs) )      return kMagSpinEqRhs; 
  if ( name == EquationTypeName(kEqMagElectric) )     return kEqMagElectric;
  if ( name == EquationTypeName(kEqEMFieldWithSpin) ) return kEqEMFieldWithSpin; 
  if ( name == EquationTypeName(kEqEMFieldWithEDM) )  return kEqEMFieldWithEDM; 
  
  TG4Globals::Exception(
    "TG4FieldParameters", "GetEquationType:",
    "Unknown equation name.");
  return kMagUsualEqRhs; 
}      


//_____________________________________________________________________________
StepperType  TG4FieldParameters::GetStepperType(const G4String& name)
{
/// Return the stepper type for given stepper type name

  if ( name == StepperTypeName(kCashKarpRKF45) )      return kCashKarpRKF45;
  if ( name == StepperTypeName(kClassicalRK4) )       return kClassicalRK4;
  if ( name == StepperTypeName(kExplicitEuler) )      return kExplicitEuler;
  if ( name == StepperTypeName(kImplicitEuler) )      return kImplicitEuler;
  if ( name == StepperTypeName(kSimpleHeum) )         return kSimpleHeum;
  if ( name == StepperTypeName(kSimpleRunge) )        return kSimpleRunge;
  if ( name == StepperTypeName(kConstRK4) )           return kConstRK4;
  if ( name == StepperTypeName(kExactHelixStepper) )  return kExactHelixStepper;
  if ( name == StepperTypeName(kHelixExplicitEuler) ) return kHelixExplicitEuler;
  if ( name == StepperTypeName(kHelixHeum) )          return kHelixHeum;
  if ( name == StepperTypeName(kHelixImplicitEuler) ) return kHelixImplicitEuler;
  if ( name == StepperTypeName(kHelixMixedStepper) )  return kHelixMixedStepper;
  if ( name == StepperTypeName(kHelixSimpleRunge) )   return kHelixSimpleRunge;
  if ( name == StepperTypeName(kNystromRK4) )         return kNystromRK4;
  if ( name == StepperTypeName(kRKG3Stepper) )        return kRKG3Stepper;
  
  TG4Globals::Exception(
    "TG4FieldParameters", "GetStepperType:",
    "Unknown stepper name.");
  return kClassicalRK4; 
}      
    
//
// ctors, dtor
//


//_____________________________________________________________________________
TG4FieldParameters::TG4FieldParameters()
  : fMessenger(this),
    fStepMinimum(fgkDefaultStepMinimum),
    fDeltaChord(fgkDefaultDeltaChord),
    fDeltaOneStep(fgkDefaultDeltaOneStep),
    fDeltaIntersection(fgkDefaultDeltaIntersection),
    fMinimumEpsilonStep(fgkDefaultMinimumEpsilonStep),
    fMaximumEpsilonStep(fgkDefaultMaximumEpsilonStep),
    fEquation(kMagUsualEqRhs),
    fStepper(kClassicalRK4)
{
/// Default constructor
}

//_____________________________________________________________________________
TG4FieldParameters::~TG4FieldParameters() 
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4FieldParameters::PrintParameters() const
{
/// Prints all customizable accuracy parameters

  G4cout << "Magnetic field parameters: " << G4endl
         << "  equation type = " << EquationTypeName(fEquation) << G4endl
         << "  stepper type = "  << StepperTypeName(fStepper) << G4endl 
         << "  minStep = "       << fStepMinimum  << " mm" << G4endl
         << "  deltaChord = "    << fDeltaChord   << " mm" << G4endl
         << "  deltaOneStep = "  << fDeltaOneStep << " mm" << G4endl
         << "  deltaIntersection = " << fDeltaIntersection << " mm" << G4endl
         << "  epsMin = " << fMinimumEpsilonStep << G4endl
         << "  epsMax=  " << fMaximumEpsilonStep <<  G4endl;
}         

 