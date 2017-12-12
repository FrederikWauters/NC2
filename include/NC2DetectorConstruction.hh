//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: NC2DetectorConstruction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file NC2DetectorConstruction.hh
/// \brief Definition of the NC2DetectorConstruction class

#ifndef NC2DetectorConstruction_h
#define NC2DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class NC2DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    NC2DetectorConstruction();
    virtual ~NC2DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    
    void Print();

  protected:
    G4LogicalVolume*  fScoringVolume;
    
    //
    //Parameters
    //
    G4double world_sizeXY;
    G4double world_sizeZ;
    G4double delta;
    
    G4double detectorDistance;
    
    //Ge detector 1
    G4double diameter_Ge1;
    G4double length_Ge1;
    G4double holeDiameter_Ge1;
    G4double holeDepth_Ge1;
    
    G4double backGapCup;
    G4double sideGapCup;
    G4double sideThicknessCup;
    G4double backThicknessCup;
    
    G4double sideThickness1;
    G4double diameterJacket1;
    G4double frontGap1;
    G4double frontThickness1;
    G4double jacketLength1;
    
    
    //Ge detector 2
    G4double diameter_Ge2;
    G4double length_Ge2;
    G4double holeDiameter_Ge2;
    G4double holeDepth_Ge2;
    
    G4double sideThickness2;
    G4double diameterJacket2;
    G4double frontGap2;
    G4double frontThickness2;
    G4double jacketLength2;
    
    //Ge detector 3
    G4double diameter_Ge3;
    G4double length_Ge3;
    G4double holeDiameter_Ge3;
    G4double holeDepth_Ge3;
    
    G4double sideThickness3;
    G4double diameterJacket3;
    G4double frontGap3;
    G4double frontThickness3;
    G4double jacketLength3;
    
    //dummy detector, catch ingoing and outgoing gamma's
    
    G4double dummyDiameter;
    G4double dummyLength;
    G4double dummyThickness;
 
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

