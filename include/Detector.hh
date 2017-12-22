#ifndef DETECTOR_H_
#define DETECTOR_H_


/// Parent Detector construction class to define materials and geometry.

#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

class Detector 
{
  public:
    Detector();
    Detector(G4LogicalVolume* vol, G4ThreeVector p, G4ThreeVector o);
    virtual ~Detector();
    
    virtual void Place(G4bool checkOverlaps, G4bool visual){};
    
  protected:
  
    // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* ge_mat = nist->FindOrBuildMaterial("G4_Ge");
  G4Material* cu_mat = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* alu_mat = nist->FindOrBuildMaterial("G4_Al");
  
  G4LogicalVolume* world;
  G4ThreeVector position; //this one is relative to (0,0,0) of G4
  double yRot;
  double xRot;
  
};

#endif
