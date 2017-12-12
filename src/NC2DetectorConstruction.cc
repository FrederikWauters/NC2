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


NC2DetectorConstruction::NC2DetectorConstruction(): G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ 
 //
 //Dimensions
 //
 
 world_sizeXY = 50.0*cm;
 world_sizeZ  = 50.0*cm;
 
 //from Stephanies typical loan pool specs
 diameter_Ge1 = 70*mm;
 length_Ge1 = 70.*mm;
 holeDiameter_Ge1 = 11*mm;
 holeDepth_Ge1 = 55*mm;
 
 diameter_Ge2 = 70*mm;
 length_Ge2 = 70.*mm;
 holeDiameter_Ge2 = 11*mm;
 holeDepth_Ge2 = 55*mm;
 
 diameter_Ge3 = 70*mm;
 length_Ge3 = 70.*mm;
 holeDiameter_Ge3 = 11*mm;
 holeDepth_Ge3 = 55*mm;
 
 delta = 0.1*mm; //for whenever a gap is needed
 
 //Cu cup
 backGapCup = 10.0*mm;
 sideGapCup = 1.0*mm;
 sideThicknessCup = 0.8*mm;
 backThicknessCup = 3.0*mm;

 
 //Jacket
 sideThickness1 = 2.0*mm;
 diameterJacket1 = diameter_Ge2 + 2*sideGapCup + 2*sideThicknessCup + +2*sideThickness1+10.0*mm;
 frontGap1 = 35.0*mm;
 frontThickness1 = 0.5*mm;
 jacketLength1 = length_Ge2+50.0*mm;
 
 sideThickness2 = 2.0*mm;
 diameterJacket2 = diameter_Ge2 + 2*sideGapCup + 2*sideThicknessCup + +2*sideThickness2+10.0*mm;
 frontGap2 = 35.0*mm;
 frontThickness2 = 0.5*mm;
 jacketLength2 = length_Ge2+50.0*mm;
 
 sideThickness3 = 2.0*mm;
 diameterJacket3 = diameter_Ge3 + 2*sideGapCup + 2*sideThicknessCup + +2*sideThickness3+10.0*mm;
 frontGap3 = 35.0*mm;
 frontThickness3 = 0.5*mm;
 jacketLength3 = length_Ge3+50.0*mm;
 
 //dummy detector
 dummyDiameter = diameterJacket1 + 20.0*mm;
 dummyLength = jacketLength1 + 20.0*mm;
 dummyThickness = 1.0*mm;
 
 detectorDistance = 13*cm + frontGap1;
 
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
  // Germanium volume
  //
  
  //Ge1  
  G4Tubs* outerGe1 = new G4Tubs("outerGe1",0.,diameter_Ge1/2.,length_Ge1/2.,0.,2*M_PI);
  G4Tubs* innerGe1 = new G4Tubs("innerGe1",0.,holeDiameter_Ge1/2.,length_Ge1/2.,0.,2*M_PI);
  G4ThreeVector zTransGe1(0, 0,-(length_Ge1-holeDepth_Ge1) );
  G4SubtractionSolid* solidGe1 = new G4SubtractionSolid("coaxGe1", outerGe1, innerGe1,0,zTransGe1);  
  G4LogicalVolume* logicGe1 = new G4LogicalVolume(solidGe1,ge_mat,"Germanium1");  
  G4ThreeVector posGe1 = G4ThreeVector(0, 0, -length_Ge1/2.-detectorDistance);
  G4RotationMatrix* rotGe1  = new G4RotationMatrix();
  rotGe1->rotateY(-90*deg);
  new G4PVPlacement(rotGe1, posGe1,   logicGe1,  "Germanium1",  logicWorld,  false,     0,  checkOverlaps);          //overlaps checking
 
  //Ge2
  G4Tubs* outerGe2 = new G4Tubs("outerGe2",0.,diameter_Ge2/2.,length_Ge2/2.,0.,2*M_PI);
  G4Tubs* innerGe2 = new G4Tubs("innerGe2",0.,holeDiameter_Ge2/2.,length_Ge2/2.,0.,2*M_PI);
  G4ThreeVector zTransGe2(0, 0,-(length_Ge2-holeDepth_Ge2) );
  G4SubtractionSolid* solidGe2 = new G4SubtractionSolid("coaxGe2", outerGe2, innerGe2,0,zTransGe2);
  G4LogicalVolume* logicGe2 = new G4LogicalVolume(solidGe2,ge_mat,"Germanium2");  
  G4ThreeVector posGe2 = G4ThreeVector(0, 0, -length_Ge2/2.-detectorDistance);
  G4RotationMatrix* rotGe2  = new G4RotationMatrix();
  rotGe2->rotateY(90*deg);
  new G4PVPlacement(rotGe2, posGe2,   logicGe2,  "Germanium2",  logicWorld,  false,     0,  checkOverlaps);
  
  //Ge3
  G4Tubs* outerGe3 = new G4Tubs("outerGe3",0.,diameter_Ge3/2.,length_Ge3/2.,0.,2*M_PI);
  G4Tubs* innerGe3 = new G4Tubs("innerGe3",0.,holeDiameter_Ge3/2.,length_Ge3/2.,0.,2*M_PI);
  G4ThreeVector zTransGe3(0, 0,-(length_Ge3-holeDepth_Ge3) );
  G4SubtractionSolid* solidGe3 = new G4SubtractionSolid("coaxGe3", outerGe3, innerGe3,0,zTransGe3);
  G4LogicalVolume* logicGe3 = new G4LogicalVolume(solidGe3,ge_mat,"Germanium3");  
  G4ThreeVector posGe3 = G4ThreeVector(0, 0, -length_Ge3/2.-detectorDistance);
  G4RotationMatrix* rotGe3  = new G4RotationMatrix();
  rotGe3->rotateY(45*deg);
  new G4PVPlacement(rotGe3, posGe3,   logicGe3,  "Germanium3",  logicWorld,  false,     0,  checkOverlaps);
  
  
  
  //
  //Cu jacket
  //
  
  //1
  G4double cupLength1 = (length_Ge1+backGapCup+backThicknessCup);
  G4Tubs* outerCup1 = new G4Tubs("outerCu1",0.,diameter_Ge1/2.+sideThicknessCup+sideGapCup,cupLength1/2.,0.,2*M_PI);
  G4Tubs* innerCup1 = new G4Tubs("innerCu1",0.,diameter_Ge1/2.+sideGapCup,(length_Ge1+backGapCup+delta)/2.,0.,2*M_PI);
  G4ThreeVector zTransCup1(0, 0,backThicknessCup/2.+delta/2.);
  G4SubtractionSolid* solidCup1 = new G4SubtractionSolid("solidCup1", outerCup1, innerCup1,0,zTransCup1);  
  G4LogicalVolume* logicCup1 = new G4LogicalVolume(solidCup1,cu_mat,"Cup1");  
  G4ThreeVector posCup1 = G4ThreeVector(0, 0, -cupLength1/2.-detectorDistance);
  new G4PVPlacement(rotGe1, posCup1,   logicCup1,  "Cup1",  logicWorld,  false,     0,  checkOverlaps); 
  
  //2
  G4double cupLength2 = (length_Ge2+backGapCup+backThicknessCup);
  G4Tubs* outerCup2 = new G4Tubs("outerCu2",0.,diameter_Ge2/2.+sideThicknessCup+sideGapCup,cupLength2/2.,0.,2*M_PI);
  G4Tubs* innerCup2 = new G4Tubs("innerCu2",0.,diameter_Ge2/2.+sideGapCup,(length_Ge2+backGapCup+delta)/2.,0.,2*M_PI);
  G4ThreeVector zTransCup2(0, 0,backThicknessCup/2.+delta/2.);
  G4SubtractionSolid* solidCup2 = new G4SubtractionSolid("solidCup2", outerCup2, innerCup2,0,zTransCup2);  
  G4LogicalVolume* logicCup2 = new G4LogicalVolume(solidCup2,cu_mat,"Cup2");  
  G4ThreeVector posCup2 = G4ThreeVector(0, 0, -cupLength2/2.-detectorDistance);
  new G4PVPlacement(rotGe2, posCup2,   logicCup2,  "Cup2",  logicWorld,  false,     0,  checkOverlaps); 
  
  //3
  G4double cupLength3 = (length_Ge3+backGapCup+backThicknessCup);
  G4Tubs* outerCup3 = new G4Tubs("outerCu3",0.,diameter_Ge3/2.+sideThicknessCup+sideGapCup,cupLength3/2.,0.,2*M_PI);
  G4Tubs* innerCup3 = new G4Tubs("innerCu3",0.,diameter_Ge3/2.+sideGapCup,(length_Ge3+backGapCup+delta)/2.,0.,2*M_PI);
  G4ThreeVector zTransCup3(0, 0,backThicknessCup/2.+delta/2.);
  G4SubtractionSolid* solidCup3 = new G4SubtractionSolid("solidCup3", outerCup3, innerCup3,0,zTransCup3);  
  G4LogicalVolume* logicCup3 = new G4LogicalVolume(solidCup3,cu_mat,"Cup3");  
  G4ThreeVector posCup3 = G4ThreeVector(0, 0, -cupLength3/2.-detectorDistance);
  new G4PVPlacement(rotGe3, posCup3,   logicCup3,  "Cup3",  logicWorld,  false,     0,  checkOverlaps);
  
  //
  //Outer Jacket
  //
  
  //Jacket1
  G4Tubs* outerJacket1 = new G4Tubs("outerJacket1",0.,diameterJacket1/2.,jacketLength1/2.,0.,2*M_PI);
  G4Tubs* innerJacket1 = new G4Tubs("innerJacket1",0.,diameterJacket1/2.-sideThickness1,jacketLength1/2.,0.,2*M_PI);
  G4ThreeVector zTransJacket1(0, 0,-frontThickness1);
  G4SubtractionSolid* solidJacket1 = new G4SubtractionSolid("solidJacket1", outerJacket1, innerJacket1,0,zTransJacket1);  
  G4LogicalVolume* logicJacket1 = new G4LogicalVolume(solidJacket1,alu_mat,"Jacket1");  
  G4ThreeVector posJacket1 = G4ThreeVector(0, 0, -jacketLength1/2.+frontThickness1+frontGap1-detectorDistance);
  new G4PVPlacement(rotGe1, posJacket1,   logicJacket1,  "Jacket1",  logicWorld,  false,     0,  checkOverlaps);
   
  //Jacket1
  G4Tubs* outerJacket2 = new G4Tubs("outerJacket2",0.,diameterJacket2/2.,jacketLength2/2.,0.,2*M_PI);
  G4Tubs* innerJacket2 = new G4Tubs("innerJacket2",0.,diameterJacket2/2.-sideThickness2,jacketLength1/2.,0.,2*M_PI);
  G4ThreeVector zTransJacket2(0, 0,-frontThickness2);
  G4SubtractionSolid* solidJacket2 = new G4SubtractionSolid("solidJacket2", outerJacket2, innerJacket2,0,zTransJacket2);  
  G4LogicalVolume* logicJacket2 = new G4LogicalVolume(solidJacket2,alu_mat,"Jacket2");  
  G4ThreeVector posJacket2 = G4ThreeVector(0, 0, -jacketLength1/2.+frontThickness1+frontGap2-detectorDistance);
  new G4PVPlacement(rotGe2, posJacket2,   logicJacket2,  "Jacket2",  logicWorld,  false,     0,  checkOverlaps);
  
  //Jacket2
  G4Tubs* outerJacket3 = new G4Tubs("outerJacket3",0.,diameterJacket3/2.,jacketLength3/2.,0.,2*M_PI);
  G4Tubs* innerJacket3 = new G4Tubs("innerJacket3",0.,diameterJacket3/2.-sideThickness3,jacketLength3/2.,0.,2*M_PI);
  G4ThreeVector zTransJacket3(0, 0,-frontThickness3);
  G4SubtractionSolid* solidJacket3 = new G4SubtractionSolid("solidJacket3", outerJacket3, innerJacket3,0,zTransJacket3);  
  G4LogicalVolume* logicJacket3 = new G4LogicalVolume(solidJacket3,alu_mat,"Jacket3");  
  G4ThreeVector posJacket3 = G4ThreeVector(0, 0, -jacketLength3/2.+frontThickness3+frontGap3-detectorDistance);
  new G4PVPlacement(rotGe3, posJacket3,   logicJacket3,  "Jacket3",  logicWorld,  false,     0,  checkOverlaps);
  
  //
  //Dummy detector
  //
  G4Tubs* outerDummy = new G4Tubs("outerDummy",0.,dummyDiameter/2.+dummyThickness,dummyLength/2.+dummyThickness,0.,2*M_PI);
  G4Tubs* innerDummy = new G4Tubs("innerDummy",0.,dummyDiameter/2.,dummyLength/2.,0.,2*M_PI);
  G4ThreeVector zTransDummy(0.,0.,0.);
  G4SubtractionSolid* solidDummy = new G4SubtractionSolid("solidDummy", outerDummy, innerDummy,0,zTransDummy);
  
  G4LogicalVolume* logicDummy = new G4LogicalVolume(solidDummy,world_mat,"Dummy");
  
  G4ThreeVector posDummy = G4ThreeVector(0, 0, -(dummyLength + dummyThickness)/2. + frontThickness1 + frontGap1 + ( dummyLength + dummyThickness - jacketLength1 )/2. );
  //G4ThreeVector posDummy = G4ThreeVector(0, 0, 0 );
  new G4PVPlacement(0,posDummy, logicDummy, "Dummy",  logicWorld,  false,     0, checkOverlaps);
  

  // Set Germanium as scoring volume
  //
  fScoringVolume = logicGe1;
  
  //
  //Set Sensitive Detectors
  //
  NC2GermaniumSD* geSD1 = new NC2GermaniumSD("GeSD1", "GeHitsCollection");
  SetSensitiveDetector("Germanium1",geSD1);
  
  NC2DummySD* dummySD = new NC2DummySD("DummySD", "DummyHitsCollection");
  SetSensitiveDetector("Dummy",dummySD);
  
  G4cout << " ************ Detector construction finished ************ " << G4endl;
  
  //
  //Set Vis attributes
  //

  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* geVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  geVisAtt->SetVisibility(true);
  geVisAtt->SetForceSolid(true);
  logicGe1->SetVisAttributes(geVisAtt);
  logicGe2->SetVisAttributes(geVisAtt);
  logicGe3->SetVisAttributes(geVisAtt);
  
  G4VisAttributes* cupVisAtt= new G4VisAttributes(G4Colour::Yellow());
  cupVisAtt->SetVisibility(true);
  cupVisAtt->SetForceWireframe(true);
  logicCup1->SetVisAttributes(cupVisAtt);
  logicCup2->SetVisAttributes(cupVisAtt);
  logicCup3->SetVisAttributes(cupVisAtt);
  
  G4VisAttributes* jacketVisAtt= new G4VisAttributes(G4Colour::Grey());
  jacketVisAtt->SetVisibility(true);
  jacketVisAtt->SetForceWireframe(true);
  logicJacket1->SetVisAttributes(jacketVisAtt);
  logicJacket2->SetVisAttributes(jacketVisAtt);
  logicJacket3->SetVisAttributes(jacketVisAtt);
  
  G4VisAttributes* dummyVisAtt= new G4VisAttributes(G4Colour::White());
  dummyVisAtt->SetVisibility(false);
  dummyVisAtt->SetForceWireframe(true);
  logicDummy->SetVisAttributes(dummyVisAtt);
  //
  //always return the physical World
  //
  return physWorld;
  
}

void NC2DetectorConstruction::Print()
{
    G4cout
    << G4endl 
    << "------------------------------------------------------------" << G4endl;
    
    G4cout << " ******** Germanium Detector Parameters *********** " << G4endl;
    G4cout << "  diameter_Ge1 : " << diameter_Ge1 << G4endl;
    G4cout << "  length_Ge1 : " << length_Ge1 << G4endl;
    G4cout << "  holeDiameter_Ge1 : " <<  holeDiameter_Ge1 << G4endl;
    G4cout << "  holeDepth_Ge1 " << holeDepth_Ge1 << G4endl << G4endl;
    
    G4cout << "  diameter_Ge2 : " << diameter_Ge1 << G4endl;
    G4cout << "  length_Ge2 : " << length_Ge1 << G4endl;
    G4cout << "  holeDiameter_Ge2 : " <<  holeDiameter_Ge1 << G4endl;
    G4cout << "  holeDepth_Ge2 " << holeDepth_Ge1 << G4endl << G4endl;
    
    G4cout << "  diameter_Ge3 : " << diameter_Ge1 << G4endl;
    G4cout << "  length_Ge3 : " << length_Ge1 << G4endl;
    G4cout << "  holeDiameter_Ge3 : " <<  holeDiameter_Ge1 << G4endl;
    G4cout << "  holeDepth_Ge3 " << holeDepth_Ge1 << G4endl << G4endl;
    
    G4cout << "  backGapCup : " << backGapCup << G4endl;
    G4cout << "  sideGapCup : " << sideGapCup << G4endl;
    G4cout << "  sideThicknessCup : " <<  sideThicknessCup << G4endl;
    G4cout << "  backThicknessCup " << backThicknessCup << G4endl << G4endl;
    
    G4cout << "  sideThickness1 : " << sideThickness1 << G4endl;
    G4cout << "  diameterJacket1 : " << diameterJacket1 << G4endl;
    G4cout << "  frontGap1 : " <<  frontGap1 << G4endl;
    G4cout << "  jacketLength1 : " <<  jacketLength1 << G4endl;
    G4cout << "  frontThickness1 " << frontThickness1 << G4endl << G4endl;
    
    G4cout << " ******** Dummy Detector Parameters *********** " << G4endl;
    G4cout << " dummyThickness : " << dummyThickness << G4endl;
    G4cout << "  dummyDiameter : " <<  dummyDiameter << G4endl;
    G4cout << "  dummyLength : " << dummyLength << G4endl << G4endl;
    

    


}



