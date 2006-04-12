// $Id: TG4IntMap.cxx,v 1.3 2004/11/10 11:39:28 brun Exp $
// Category: global
//
// Class TG4IntMap
// ---------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4IntMap.h"
#include "TG4Globals.h"

#include "iomanip"
#include "globals.hh"

//_____________________________________________________________________________
TG4IntMap::TG4IntMap(){
//
}

//_____________________________________________________________________________
TG4IntMap::TG4IntMap(const TG4IntMap& right) {
//
  TG4Globals::Exception("TG4IntMap is protected from copying.");
}  

//_____________________________________________________________________________
TG4IntMap::~TG4IntMap() {
//
}

//
// operators
//

//_____________________________________________________________________________
TG4IntMap& TG4IntMap::operator=(const TG4IntMap& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception("TG4IntMap is protected from assigning.");
    
  return *this;  
}    
          
//
// private methods
//

//_____________________________________________________________________________
G4bool TG4IntMap::IsDefined(const G4String& first)
{
/// Return true if the first is already in the map.

  MapIterator i = fMap.find(first);
  if (i == fMap.end()) 
    return false;
  else                 
    return true;
}

//
// public methods
//

//_____________________________________________________________________________
G4bool TG4IntMap::Add(const G4String& first, G4int second)
{  
/// Add pair (name, int number) to the map.

  if (!IsDefined(first)) {
    // insert into map 
    // only in case it is not yet here
    fMap[first] = second;
    return true;
  }
  return false;
}

//_____________________________________________________________________________
G4int TG4IntMap::GetSecond(const G4String& name, G4bool warn)
{
/// Get second name associated with given name.

  MapIterator i = fMap.find(name);
  if (i == fMap.end()) {
    if (warn) {
      G4String text = "   TG4IntMap::GetSecond: ";
      text = text + name + " is not defined.";
      TG4Globals::Warning(text);
    }  
    return 0;
  }  
  else {                
    return (*i).second;
  }  
}
//_____________________________________________________________________________
G4int TG4IntMap::GetSize() const
{
/// Get map size.

  return fMap.size();
}  

//_____________________________________________________________________________
void TG4IntMap::PrintAll() const
{
/// Dump the whole map.

  if (fMap.size()) {
    G4cout << "Dump of TG4IntMap - " << fMap.size() << " entries:" << G4endl;
    G4int counter = 0;
    for (MapConstIterator i=fMap.begin(); i != fMap.end(); i++) {
      const G4String& first  = (*i).first;
      G4int second = (*i).second;
      G4cout << "Map element " << std::setw(3) << counter++ << "   " 
             << first << "   " << second << G4endl;
    }
  }
}

//_____________________________________________________________________________
void TG4IntMap::Clear() 
{
/// Clear the map.

  if (fMap.size()>0) fMap.clear();
}  
