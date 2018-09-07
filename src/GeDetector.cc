#include <iostream>

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"


#include "G4SubtractionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"

#include "GeDetector.hh"

using namespace std;

GeDetector::GeDetector()
{
  G4cout << "Dummy constructor Ge detector " << G4endl;
} 

GeDetector::GeDetector(G4LogicalVolume *vol,ge_config_t cf) : Detector(vol,cf.pos,cf.origin)
{
  config = cf;
  G4cout << "Constructing Ge detector " << config.name << G4endl;
  
  
  
  //Crystal
  G4Tubs* outer = new G4Tubs("outer",0.,config.diameter/2.,config.length/2.,0.,2*M_PI);
  G4Tubs* inner = new G4Tubs("inner",0.,config.holeDiameter/2.,config.length/2.,0.,2*M_PI);
  G4ThreeVector zTrans(0, 0,(config.length-config.holeDepth) );
  G4SubtractionSolid* solid = new G4SubtractionSolid("coax", outer, inner,0,zTrans);
  string logic_name = config.name;
  logic_name.append("_logic");
  logic_crystal = new G4LogicalVolume(solid,ge_mat,logic_name);
  
  //Cup
  cupLength = config.length+config.backGapCup+config.backThicknessCup;
  G4double innerRadiusCup = config.diameter/2.+config.sideGapCup;
  G4Tubs* outerCup = new G4Tubs("outerCup",0.,innerRadiusCup+config.sideThicknessCup,cupLength/2.,0.,2*M_PI);
  G4Tubs* innerCup = new G4Tubs("innerCup",0.,innerRadiusCup,cupLength/2.,0.,2*M_PI);
  G4ThreeVector zTransCup(0, 0,-config.backThicknessCup);
  G4SubtractionSolid* solidCup = new G4SubtractionSolid("solidCup", outerCup, innerCup,0,zTransCup);
  string logic_name_cup = config.name;
  logic_name_cup.append("_cup_logic");
  logic_cup = new G4LogicalVolume(solidCup,alu_mat,logic_name_cup);
  
  //Outer Jacket
  jacketLength = config.length+config.frontGap+config.frontThickness;
  G4double innerRadiusJacket = innerRadiusCup + config.sideThicknessCup + config.sideGap;
  G4Tubs* outerJacket = new G4Tubs("outerJacket",0.,innerRadiusJacket + config.sideThickness,jacketLength/2.,0.,2*M_PI);
  G4Tubs* innerJacket = new G4Tubs("innerJacket",0.,innerRadiusJacket,jacketLength/2.,0.,2*M_PI);
  G4ThreeVector zTransJacket(0, 0,config.frontThickness);
  G4SubtractionSolid* solidJacket = new G4SubtractionSolid("solidJacket",outerJacket, innerJacket,0,zTransJacket);
  string logic_name_jacket = config.name;
  logic_name_jacket.append("_jacket_logic");
  logic_jacket = new G4LogicalVolume(solidJacket,alu_mat,logic_name_jacket);
  
  //absorber
  G4Tubs* solidAbsorber = new G4Tubs("solidAbsorber",0.,config.diameter/2.,config.absorberThickness/2.,0.,2*M_PI);
  string logic_name_absorber = config.name;
  logic_name_absorber.append("_absorber_logic");
  logic_absorber = new G4LogicalVolume(solidAbsorber,world_mat,logic_name_absorber);
  
} 

GeDetector::~GeDetector()
{
  
}

void GeDetector::Place(G4bool checkOverlaps, G4bool visual)
{
  
  G4cout << "Placing detector  " << config.name << G4endl;
  
  G4RotationMatrix* rotation = new G4RotationMatrix();
  rotation->rotateY(yRot*rad);
  rotation->rotateX(xRot*rad);
  
  G4ThreeVector volume_position;
  G4ThreeVector volume_position_save;
  G4double r = position.r();
  
  volume_position = position * ( 1 + ( config.frontThickness + config.frontGap + config.length/2.)/r );
  physical_crystal = new G4PVPlacement(rotation,volume_position, logic_crystal, "Crystal",  world,  false,     0, checkOverlaps);
 // G4cout << "Placing detector crystal " << G4endl;
  //G4cout << "position : " << volume_position << G4endl;
  
  volume_position = position * ( 1 + ( cupLength*0.5 + config.frontThickness + config.frontGap)/r );
  physical_cup = new G4PVPlacement(rotation,volume_position, logic_cup, "Cup",  world,  false,     0, checkOverlaps);
 // G4cout << "Placing detector cup" << G4endl;
 // G4cout << "position : " << volume_position << G4endl;
  
  volume_position = position * ( 1 + (jacketLength*0.5)/r );
  volume_position_save = volume_position;
  physical_jacket = new G4PVPlacement(rotation,volume_position, logic_jacket, "Jacket",  world,  false,     0, checkOverlaps);
  //G4cout << "Placing detector jacket" << G4endl;
 // G4cout << "jacket length" << jacketLength << G4endl;
  //G4cout << "position : " << volume_position << G4endl;
  
  volume_position = position * ( 1 - (0.5*config.absorberThickness)/r );
  physical_absorber = new G4PVPlacement(rotation,volume_position, logic_absorber, "Absorber",  world,  false,     0, checkOverlaps);
 // G4cout << "Placing absorber " << G4endl;
 // G4cout << "position : " << volume_position << G4endl;
 // G4cout << "distance to jacket : " << (volume_position-volume_position_save).getR() << G4endl;
  
;
  
  
  if(visual)
  {
    G4VisAttributes* jacketVisAtt= new G4VisAttributes(G4Colour::Grey());
    jacketVisAtt->SetVisibility(true);
    jacketVisAtt->SetForceWireframe(true);
    logic_jacket->SetVisAttributes(jacketVisAtt);
    
    G4VisAttributes* geVisAtt = new G4VisAttributes(G4Colour::Red());
    geVisAtt->SetVisibility(true);
    geVisAtt->SetForceSolid(true);
    logic_crystal->SetVisAttributes(geVisAtt);
    
    G4VisAttributes* absorderVisAtt = new G4VisAttributes(G4Colour::Yellow());
    absorderVisAtt->SetVisibility(true);
    absorderVisAtt->SetForceSolid(true);
    logic_absorber->SetVisAttributes(absorderVisAtt);
  }
  
  return;
}

void GeDetector::SetSD(std::string name)
{

  std::string sd_name = name + "SD" ;
  std::string hc_name = name + "HitsCollection" ;    
    
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  sensitive_detector = new NC2GermaniumSD(sd_name,hc_name);
  SDman->AddNewDetector( sensitive_detector );
  logic_crystal->SetSensitiveDetector(sensitive_detector);
  return;
}

