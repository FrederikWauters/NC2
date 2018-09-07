#ifndef GEDETECTOR_H_
#define GEDETECTOR_H_

#include "Detector.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "NC2GermaniumSD.hh"
#include <string>

// Germanium Detector construction class to define materials and geometry.

struct ge_config_t{  
      std::string name;
      //crystal
      G4double diameter;
      G4double length;
      G4double holeDiameter;
      G4double holeDepth;
      //Cup around the crystal
      G4double backGapCup;
      G4double sideGapCup;
      G4double sideThicknessCup;
      G4double backThicknessCup;
      //Jacket
      G4double frontGap;
      G4double sideGap;
      G4double sideThickness;
      G4double frontThickness;
      //Absorber
      G4double absorberThickness;
      
      //Position
      G4ThreeVector pos; //this one is relative to 'origin' of G4
      G4ThreeVector origin;
      
};


class GeDetector : public Detector
{
  public:
    GeDetector();
    GeDetector(G4LogicalVolume* vol, ge_config_t cf);
    ~GeDetector();
    
    void Place(G4bool checkOverlaps, G4bool visual);
    void SetSD(std::string name);

    ge_config_t GetConfig() {return config; }
    
  private:
    ge_config_t config;
    
    G4double jacketLength;
    G4double cupLength;
    
    G4LogicalVolume* logic_crystal;
    G4LogicalVolume* logic_cup;
    G4LogicalVolume* logic_jacket;
    G4LogicalVolume* logic_absorber;
    
    G4VPhysicalVolume* physical_crystal;
    G4VPhysicalVolume* physical_cup;
    G4VPhysicalVolume* physical_jacket;
    G4VPhysicalVolume* physical_absorber;
    
    NC2GermaniumSD* sensitive_detector;
    
    
  
};

#endif
