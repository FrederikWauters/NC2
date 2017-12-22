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

//
//Detector construction for Germanium detector with Compton veto
//

#include "NC2DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"


#include "G4SDManager.hh"
#include "NC2GermaniumSD.hh"
#include "NC2DummySD.hh"




NC2DetectorConstruction::NC2DetectorConstruction(): G4VUserDetectorConstruction()
{ 
  
 //
 //Dimensions
 //
 
 world_sizeXY = 100.0*cm;
 world_sizeZ  = 100.0*cm;
 
  // germanium detector parameters
  
  //survey results
 /* PunktS1 -1.117 94.560 9557.515
PunktS2 70.610 58.285 9564.941
PunktS3 97.267 -11.483 9557.375
PunktS4 57.587 -84.839 9552.423
PunktS5 -9.787 -97.016 9556.683
PunktS6 -75.010 -80.888 9552.919
PunktS7 -107.477 -9.624 9574.415
PunktS8 -92.412 63.811 9581.075
PunktS9 121.574 -9.747 9460.619
PunktS10 -121.375 -5.159 9488.936
PunktS11 -36.730 -12.126 9597.484
S1L1 43.344 88.234 9567.282
S1L2 -13.315 117.797 9519.786
S1L3 -33.423 77.644 9585.485
Zyl 2.083 0.467 9440.597*/

G4ThreeVector origin(2.083*mm, 0.467*mm, 9440.597*mm);

G4ThreeVector ge2_pos(70.610*mm, 58.285*mm, 9564.941*mm);
G4ThreeVector ge3_pos(97.267*mm, -11.483*mm, 9564.941*mm);
G4ThreeVector ge4_pos(57.587*mm, -84.839*mm, 9552.423*mm);
G4ThreeVector ge5_pos(-9.787*mm, -97.016*mm, 9556.683*mm);
G4ThreeVector ge6_pos(-75.010*mm, -80.888*mm, 9552.919*mm);
G4ThreeVector ge7_pos(-107.477*mm,-9.624*mm, 9574.415*mm);
G4ThreeVector ge8_pos(-92.412*mm, 63.811*mm, 9581.075*mm);




               //name  //diameter  //length  //bore diam  //bore depth   <--- crystal
  ge2_conf = { "Ge2",  70*mm,      70.*mm,   11*mm,       55*mm,
	       //backgap  //side gap  //side thickness  //back thickness  <--- cup
	       10.0*mm,   0.1*mm,     0.8*mm,           3.0*mm,
	       //front gap  //side gap  //side thickness  //front thickness  <--- Jacket
	       35.0*mm,     2.0*mm,     1.0*mm,            0.5*mm,
	       //absorber thickness
	       0.8*mm,
	       //position  //origin
	       ge2_pos,    origin
              };
	      
  ge3_conf = ge2_conf;
  ge3_conf.name = "Ge3";
  ge3_conf.pos = ge3_pos;
  
  ge4_conf = ge2_conf;
  ge4_conf.name = "Ge4";
  ge4_conf.pos = ge4_pos;
  
  ge5_conf = ge2_conf;
  ge5_conf.name = "Ge5";
  ge5_conf.pos = ge5_pos;
  
  ge6_conf = ge2_conf;
  ge6_conf.name = "Ge6";
  ge6_conf.pos = ge6_pos;
  
  ge7_conf = ge2_conf;
  ge7_conf.name = "Ge7";
  ge7_conf.pos = ge7_pos;
  
  ge8_conf = ge2_conf;
  ge8_conf.name = "Ge8";
  ge8_conf.pos = ge8_pos;
  

 
  //source
  sourceThickness = 0.7*mm;
  sourceDiameter = 35.0*mm;
}



NC2DetectorConstruction::~NC2DetectorConstruction()
{ }



G4VPhysicalVolume* NC2DetectorConstruction::Construct()
{  
  G4cout << " ********* Buidling the detectors ********* " << G4endl;
  //*********************************
  //*******  Materials  *************
  //*********************************
  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* ge_mat = nist->FindOrBuildMaterial("G4_Ge");
  G4Material* cu_mat = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* alu_mat = nist->FindOrBuildMaterial("G4_Al");
  G4Material* zn_mat = nist->FindOrBuildMaterial("G4_Zn");
  
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;
  

  //*********************************
  //*******  Make Stuff *************
  //*********************************

  //     
  // World
  //
  G4Box* solidWorld =  new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ); 
  
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Germanium detectors
  //
   

  GeDetector* Ge2 = new GeDetector(logicWorld,ge2_conf);    germanium_detectors.push_back(Ge2);
  GeDetector* Ge3 = new GeDetector(logicWorld,ge3_conf);    germanium_detectors.push_back(Ge3);
  GeDetector* Ge4 = new GeDetector(logicWorld,ge4_conf);    germanium_detectors.push_back(Ge4);
  GeDetector* Ge5 = new GeDetector(logicWorld,ge5_conf);    germanium_detectors.push_back(Ge5);
  GeDetector* Ge6 = new GeDetector(logicWorld,ge6_conf);    germanium_detectors.push_back(Ge6);
  GeDetector* Ge7 = new GeDetector(logicWorld,ge7_conf);    germanium_detectors.push_back(Ge7);
  //GeDetector* Ge8 = new GeDetector(logicWorld,ge8_conf);    germanium_detectors.push_back(Ge8);
  
  G4bool visualize = true;
  for(auto &detector: germanium_detectors)
  {
    detector->Place(checkOverlaps,visualize);
  }
  //Ge2->Place(checkOverlaps,visualize);
  
  G4Tubs* solidSource = new G4Tubs("solidSource",0.,sourceDiameter/2.,sourceThickness/2.,0.,2*M_PI);
  G4LogicalVolume* logicSource = new G4LogicalVolume(solidSource,zn_mat,"logicsource");
  G4ThreeVector sourcePosition(0,0,0);
  new G4PVPlacement(0, sourcePosition,   logicSource,  "Source",  logicWorld,  false,     0,  checkOverlaps); 

  //
  //Set Vis attributes
  //

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  
  G4VisAttributes* sourceVisAtt= new G4VisAttributes(G4Colour::Cyan());
  sourceVisAtt->SetVisibility(true);
  sourceVisAtt->SetForceSolid(true);
  logicSource->SetVisAttributes(sourceVisAtt);

  /*logicDummy->SetVisAttributes(dummyVisAtt);*/
  //
  //always return the physical World
  //
  return physWorld;
  
}

void NC2DetectorConstruction::Print()
{
    G4cout << "------------------------------------------------------------" << G4endl;
    G4cout << "----------------- Ge detector configuration ----------------" << G4endl;
    G4cout << "------------------------------------------------------------" << G4endl << G4endl;
    
    for(auto &detector: germanium_detectors)
    {
      G4cout	<<	"	name			=	"	<<	detector->GetConfig().name		<<	G4endl;
      G4cout	<<	"	diameter		=	"	<<	detector->GetConfig().diameter		<<	G4endl;
      G4cout	<<	"	length			=	"	<<	detector->GetConfig().length		<<	G4endl;
      G4cout	<<	"	holeDiameter		=	"	<<	detector->GetConfig().holeDiameter		<<	G4endl;
      G4cout	<<	"	holeDepth		=	"	<<	detector->GetConfig().holeDepth		<<	G4endl;
      G4cout	<<	"	backGapCup		=	"	<<	detector->GetConfig().backGapCup		<<	G4endl;
      G4cout	<<	"	sideGapCup		=	"	<<	detector->GetConfig().sideGapCup		<<	G4endl;
      G4cout	<<	"	sideThicknessCup	=	"	<<	detector->GetConfig().sideThicknessCup		<<	G4endl;
      G4cout	<<	"	backThicknessCup	=	"	<<	detector->GetConfig().backThicknessCup		<<	G4endl;
      G4cout	<<	"	frontGap		=	"	<<	detector->GetConfig().frontGap		<<	G4endl;
      G4cout	<<	"	sideGap			=	"	<<	detector->GetConfig().sideGap		<<	G4endl;
      G4cout	<<	"	sideThickness		=	"	<<	detector->GetConfig().sideThickness		<<	G4endl;
      G4cout	<<	"	frontThickness		=	"	<<	detector->GetConfig().frontThickness		<<	G4endl;
      G4cout	<<	"	absorberThicknes	=	"	<<	detector->GetConfig().absorberThickness		<<	G4endl;

    }
    
    

}





