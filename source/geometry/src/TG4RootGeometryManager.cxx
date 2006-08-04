// $Id: TG4RootGeometryManager.cxx,v 1.7 2005/09/01 10:04:32 brun Exp $
// Category: geometry
//
// Class TG4RootGeometryManager
// ----------------------------
// Class for importing Root TGeo geometry in Geant4 VMC.
//
// Author: I. Hrivnacova,  4.5.2004

#ifdef USE_VGM

#include <TGeoManager.h>
#include <TGeoMedium.h>

#include <G3MedTable.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Material.hh>
#include <G4ReflectionFactory.hh>

#include <VGM/volumes/IVolume.h>
#include <RootGM/volumes/Factory.h>

#include "TG4RootGeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4Limits.h"
#include "TG4IntMap.h"
#include "TG4Globals.h"

//_____________________________________________________________________________
TG4RootGeometryManager::TG4RootGeometryManager(
                                       TG4GeometryServices* geometryServices,
                                       TG4IntMap* mediumMap,  
                                       TG4StringVector* mediumNameVector) 
  : TG4Verbose("rootGeometryManager"),
    fG4Factory(0),
    fGeometryServices(geometryServices),
    fMediumMap(mediumMap),
    fMediumIdMap(),
    fMediumNameVector(mediumNameVector),
    fMediumCounter(0)
{
//
}

//_____________________________________________________________________________
TG4RootGeometryManager::TG4RootGeometryManager() 
  : TG4Verbose("rootGeometryManager"),
    fG4Factory(0),
    fGeometryServices(0),
    fMediumMap(0),
    fMediumNameVector(0),
    fMediumCounter(0)
{
//
}

//_____________________________________________________________________________
TG4RootGeometryManager::TG4RootGeometryManager(const TG4RootGeometryManager& right)
  : TG4Verbose("geometryManager") {
// 
  TG4Globals::Exception(
    "Attempt to copy TG4RootGeometryManager singleton.");
}


//_____________________________________________________________________________
TG4RootGeometryManager::~TG4RootGeometryManager() {
//
  delete fG4Factory;
}

//
// operators
//

//_____________________________________________________________________________
TG4RootGeometryManager& 
TG4RootGeometryManager::operator=(const TG4RootGeometryManager& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception(
    "TG4RootGeometryManager::operator= not implemented.");
    
  return *this;  
}    
          

//
// private methods
//
 
//_____________________________________________________________________________
void TG4RootGeometryManager::Gstmed(G4int itmed, G4String, 
              const G4Material* material,
              G4int isvol, G4int, G4double, G4double,
              G4double stemax, G4double, G4double,
              G4double, G4double*, G4int useG3TMLimits)
{
/// Fill G3Med table with a medium with specified properties.               \n
/// In difference from G4gstmed in g3tog4 the material is passed
/// directly via G4Material* and not nmat index.

  // NB. there is the possibility for redundancy in the mag field
  //     and user limits objects. Who cares.
  // Generate the mag field object
  // $$$ G4MagneticField* field = new G4MagneticField(ifield, fieldm, tmaxfd);
  G4MagneticField* field = 0;

  // Generate the user limits object
  // !!! only "stemax" has its equivalent in G4

  G4UserLimits* limits = 0;
  if (useG3TMLimits) {
    limits = new G4UserLimits();
    limits->SetMaxAllowedStep(stemax*cm);
    // limits->SetG3DefaultCuts();
         // this is arranged globally by physics manager
  }

  // Store this medium in the G3Med structure
  G3Med.put(itmed, const_cast<G4Material*>(material), field, limits, isvol);
}

//_____________________________________________________________________________
void TG4RootGeometryManager::Medium(Int_t& kmed, const char *name, 
          const G4Material* material, 
          Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
          Double_t stemax, Double_t deemax, Double_t epsil, 
          Double_t stmin, Double_t* ubuf, Int_t nbuf)
{ 
/// Create a temporary "medium" that is used for 
/// assigning corresponding parameters to G4 objects:
/// - NTMED is stored as a second material index;
/// - ISVOL is used for builing G3SensVolVector;
/// - STEMAX is passed in G4UserLimits
/// - !! The other parameters (IFIELD, FIELDM, TMAXFD, DEEMAX, EPSIL, STMIN)
/// are ignored by Geant4.                                                   \n
/// In difference from TG4GeometryManager::Medium the material is passed
/// directly via G4Material* and not nmat index.

//  Geant3 desription:
//  ==================
//  NTMED  Tracking medium number
//  NAME   Tracking medium name
//  NMAT   Material number
//  ISVOL  Sensitive volume flag
//  IFIELD Magnetic field
//  FIELDM Max. field value (Kilogauss)
//  TMAXFD Max. angle due to field (deg/step)
//  STEMAX Max. step allowed
//  DEEMAX Max. fraction of energy lost in a step
//  EPSIL  Tracking precision (cm)
//  STMIN  Min. step due to continuos processes (cm)
//
//  IFIELD = 0 if no magnetic field; IFIELD = -1 if user decision in GUSWIM;
//  IFIELD = 1 if tracking performed with GRKUTA; IFIELD = 2 if tracking
//  performed with GHELIX; IFIELD = 3 if tracking performed with GHELX3.  

  G4String namein = fGeometryServices->CutMaterialName(name);

  kmed = ++fMediumCounter;

  Gstmed(kmed, name, material, isvol, ifield, fieldm, tmaxfd, stemax, deemax, 
       epsil, stmin, 0, stemax > 0.);
     // instead of the nbuf argument the bool is passed
     // in order to pass stemax into G4UserLimits

  // generate new unique name  
  G4String newName 
    = fGeometryServices
        ->GenerateLimitsName(kmed, namein, material->GetName());
  fMediumNameVector->push_back(newName);
  
  if (nbuf > 0) {  
    G4String medName = name;
    G4String text = "TG4RootGeometryManager: Medium: \n";
    text = text + "    User defined parameters for medium ";
    text = text + medName;
    text = text + " are ignored by Geant4.";  
    TG4Globals::Warning(text);
  }
} 

//_____________________________________________________________________________
void TG4RootGeometryManager::ConvertRootGeometry()
{
/// Convert Root geometry to G4 geometry objects
/// using roottog4 convertor.

  // Check Root manager
  if (!gGeoManager) {
    TG4Globals::Exception(
      "TG4RootGeometryManager::SetRootGeometry: Root geometry manager not found.");
  }    

  // Get and eventually also set the Root top volume 
  TGeoVolume* top = gGeoManager->GetTopVolume();
  if (!top) {
    top = (TGeoVolume*)gGeoManager->GetListOfVolumes()->First();
    if (!top) {
      TG4Globals::Exception(
        "TG4RootGeometryManager::SetRootGeometry: Root top volume not found.");
    }    
    gGeoManager->SetTopVolume(top);
  }  

  // Close Root geometry
  if (!gGeoManager->IsClosed()) gGeoManager->CloseGeometry();  

  // Convert Root geometry to G4
  if (VerboseLevel()>0)
    G4cout << "Converting Root geometry to Geant4 via VGM ... " << G4endl;
  
  // import Root geometry in VGM
  RootGM::Factory rootFactory;
  if (VerboseLevel()>1) rootFactory.SetDebug(1);
  rootFactory.Import(top);
    
  // export Root VGM geometry in Geant4
  fG4Factory = new Geant4GM::Factory();
  if (VerboseLevel()>1) fG4Factory->SetDebug(1);
  rootFactory.Export(fG4Factory);
    
  G4VPhysicalVolume* g4World = fG4Factory->World();
  fGeometryServices->SetWorld(g4World);
}                   
 
//_____________________________________________________________________________
void TG4RootGeometryManager::ConvertRootMedias()
{
/// Convert medias from TGeo into temporary objects in G3MedTable.

  TIter next(gGeoManager->GetListOfMedia());
  TGeoMedium* medium;
  while ((medium=(TGeoMedium*)next())) {
    Int_t kmed;
    Int_t isvol  = (Int_t) medium->GetParam(0);
    Int_t ifield = (Int_t) medium->GetParam(1);
    Double_t fieldm = medium->GetParam(2);
    Double_t tmaxfd = medium->GetParam(3);
    Double_t stemax = medium->GetParam(4);
    Double_t deemax = medium->GetParam(5);
    Double_t epsil  = medium->GetParam(6);
    Double_t stmin  = medium->GetParam(7);
    Double_t* ubuf  = 0;
    
    const G4Material* material = 0;
    // Get material from G4MaterialTable via its name
    G4String materialName = medium->GetMaterial()->GetName();
    material = G4Material::GetMaterial(materialName);
    
    // Check material
    if (!material) {
      G4String text = "ConvertRootMedias: Material ";
      text += medium->GetMaterial()->GetName();
      text += " not found in G4MaterialTable";
      TG4Globals::Exception(text); 
    }  
    
    Medium(kmed, medium->GetName(), material, isvol, ifield, fieldm, tmaxfd,
           stemax, deemax, epsil, stmin, ubuf, 0);
	   
    // fMediumIdMap[medium->GetId()] = kmed;
    
    // Reset the new Id to the TGeoMedium
    // to make it retrievable from user VMC application
    // (will not be needed when TVirtualMC::MediumId(const char* name)
    //  is available)
    medium->SetId(kmed); 
  } 
}    	         	            
    
//_____________________________________________________________________________
void TG4RootGeometryManager::FillMediumMap()
{
/// Map tracking medium IDs to volumes names

  const VGM::VolumeStore& volumes = fG4Factory->Volumes();
  for (G4int i=0; i<G4int(volumes.size()); i++) {
    // Get medium name
    VGM::IVolume* volume = volumes[i];
    G4String name  = volume->Name();
    G4String mediumName  = volume->MediumName();

    // Get TGeoMedium via name 
    // (to get index defined via MC)
    TGeoMedium* geoMedium = gGeoManager->GetMedium(mediumName.data());
    if ( ! geoMedium ) {
      G4String text = "    TG4RootGeometryManager::FillMediumMap: \n";
      text += "    Medium: ";
      text += mediumName;
      text += "  (of volume: ";
      text += name; 
      text += " not found in Root";
      TG4Globals::Exception(text); 
    }  
    G4int mediumId = geoMedium->GetId(); 
   
    // Map it to logical volume name
    fMediumMap->Add(name, mediumId);     
  }     
}    

//
// public methods
//

//_____________________________________________________________________________
void TG4RootGeometryManager::ImportRootGeometry()
{
/// Convert Root geometry to G4 geometry objects
/// and Root medias to Geant4 VMC objects.

  // Convert Root geometry
  ConvertRootGeometry();
 
  // Convert Root medias to G3Med table
  ConvertRootMedias();

  // Fill medium map
  FillMediumMap();
  
  // Delete VGM objects  delete fG4Factory;
  delete fG4Factory;
  fG4Factory = 0;
} 

#endif //USE_VGM   
