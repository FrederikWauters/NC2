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

#include <vector>

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "GeDetector.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class NC2DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    NC2DetectorConstruction();
    virtual ~NC2DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    
    void Print();
    unsigned int NGeDetectors(){ return germanium_detectors.size(); };
    std::vector<std::string> GetGeDetectorNames();
    std::vector<GeDetector*>* GetGeDetectors() { return &germanium_detectors; };

  protected:
    
    //world
    G4double world_sizeXY;
    G4double world_sizeZ;
    G4double delta;
    
    //germanium detectors
    std::vector<GeDetector* > germanium_detectors;
    ge_config_t ge1_conf,ge2_conf,ge3_conf,ge4_conf,ge5_conf,ge6_conf,ge7_conf,ge8_conf,ge9_conf,ge10_conf;

    //source
    G4double sourceThickness;
    G4double sourceDiameter;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

