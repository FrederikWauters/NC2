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
// $Id: NC2PrimaryGeneratorAction.cc 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file NC2PrimaryGeneratorAction.cc
/// \brief Implementation of the NC2PrimaryGeneratorAction class

#include "NC2PrimaryGeneratorAction.hh"
#include "NC2PrimaryGeneratorMessenger.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


//#include "G4UniformRand.hh"



NC2PrimaryGeneratorAction::NC2PrimaryGeneratorAction(NC2EventAction* ea): G4VUserPrimaryGeneratorAction(),  fParticleGun(0), eventAction(ea)
{
  G4int n_particle = 1;
  
  cosThetaLow = -1; //define a cone along the Z axis as a source
  cosThetaHigh = -0.985;
  //cosThetaHigh = -0.98;
  
  gammas.clear();
  entries = 0;
 

  fParticleGun  = new G4ParticleGun(n_particle);
  messenger = new NC2PrimaryGeneratorMessenger(this);
  
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle  = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
  defaultEnergy = 1.*MeV; //default energy
}



NC2PrimaryGeneratorAction::~NC2PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete messenger;
}



void NC2PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  //  G4LogicalVolume* envLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World"); //grab something from the detector construction
  //fParticleGun->SetParticlePosition(G4ThreeVector(0,0,200*mm));
  
  //Generate random direction
  G4double cosTheta = (cosThetaHigh-cosThetaLow)*G4UniformRand()+cosThetaLow;
  G4double phi       = 2*M_PI*G4UniformRand();
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  
  G4double px = cos(phi)*sinTheta ;
  G4double py = sinTheta*sin(phi);
  G4double pz = cosTheta;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
  

 //Get energy from input spectrum (set in macro)
  G4double init_energy;
  if(entries > 0)
  {
    init_energy = GetEnergy();    
  } 
  else { G4cout <<  " No gamma available from list, use default" << G4endl; init_energy = defaultEnergy;}
  
  fParticleGun->SetParticleEnergy(init_energy);
  eventAction->SetInitEnergy(init_energy); //save init energy in output file
  fParticleGun->GeneratePrimaryVertex(anEvent);
}





void NC2PrimaryGeneratorAction::SetNewEnergy(G4double value) 
{
  //always call this one first!
  Gamma gamma;
  gamma.energy = value;
  gamma.intensity = -1.;
  gamma.use=true;
  gammas.push_back(gamma);
  entries++;
}


void NC2PrimaryGeneratorAction::SetNewIntensity(G4double value) 
{
  G4cout << "entries : " << entries << G4endl;
  if(entries < 1) { G4cout << " ******** Set gamma energy first ********** " << G4endl; return; }
  if(gammas.at(entries-1).intensity>0) { G4cout << " *********** This intensity was already set, first set a new gamma energy !!! ******* " << G4endl; return; }
  else
  {
    gammas.at(entries-1).intensity=value;
  }
  
  G4cout << "Set intensity of " << value << G4endl;
}


void NC2PrimaryGeneratorAction::SetNewUse(G4bool value) 
{
  gammas.at(entries).use=value;
}
  
G4double NC2PrimaryGeneratorAction::GetEnergy()
{
  G4bool success = false;
  G4double energy = 0.;
  if (entries < 1) G4cout << " No gamma's to choose from! " << G4endl;
  while(!success)
  {
    G4int trial_a = int(trunc(G4UniformRand()*entries)); //random integer number between 0 and entries -1
    //G4cout << "trial_a " << trial_a << G4endl;
    if(gammas.at(trial_a).use = true)
    {
      G4double trial_b = G4UniformRand();
      if(gammas.at(trial_a).intensity > trial_b)
      {
        energy = gammas.at(trial_a).energy;
        success = true;
      }
    }
  }
  //G4cout << energy << G4endl;
  return energy;
}

void NC2PrimaryGeneratorAction::PrintGammas()
{
  G4cout << " **************************************** " << G4endl;
  G4cout << " ********** Print Gamma's *************** " << G4endl;
  G4cout << " **************************************** " << G4endl << G4endl;
  
  if( gammas.size() != entries || entries <1) { G4cout << " !!! unable to print gamma's !!! " << G4endl; return; }
  
  G4cout << " Energy (MeV)	intensity	use (1/0) " << G4endl;
  for(int i = 0; i <entries; i++)
  {
    G4cout << gammas.at(i).energy << "	" << gammas.at(i).intensity << "	" << gammas.at(i).use << G4endl;  
  }
  
  G4cout << G4endl << " ************************** " << G4endl << G4endl;
}
    
