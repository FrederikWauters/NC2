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

#include "utils.hh"

#include<iterator>
#include<cstdlib>
#include <math.h>


//#include "G4UniformRand.hh"



NC2PrimaryGeneratorAction::NC2PrimaryGeneratorAction(NC2EventAction* ea): G4VUserPrimaryGeneratorAction(),  fParticleGun(0), eventAction(ea)
{
  G4int n_particle = 1;
  
  cosThetaLow = -0.36; //the detectors are sitting in the XY plane
  cosThetaHigh = 0.36;
  //cosThetaHigh = -0.98;
  
  //cosThetaLow=-1.;
  //cosThetaHigh=1.;
  
  fParticleGun  = new G4ParticleGun(n_particle);
  messenger = new NC2PrimaryGeneratorMessenger(this);
  
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  xray  = particleTable->FindParticle(particleName="gamma");
  electron  = particleTable->FindParticle(particleName="electron");
  neutron  = particleTable->FindParticle(particleName="neutron");
  fParticleGun->SetParticleDefinition(xray);
  defaultEnergy = 1.*MeV; //default energy
  initialN = 6;
  muon_lifetime = 2197.*ns;
  zinc_lifetime = 160.*ns;
  muon_mass = 105.65*MeV;
  levels.clear();
  SetLevels();
  PrintLevels();
  decayFlag = false;
  
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
  
  //generate xrays
  
  fParticleGun->SetParticleDefinition(xray);
  
  //Get Seed Level
  Level* level = GetSeedLevel();
  
  std::vector<G4double> energies;
  
  eventAction->ClearInitLevels();
  eventAction->AddLevel(level->GetName());
  
  while(level->Get_n() > 1)
  {
    G4double energy;
    level = level->GetTransition(&energy);
    eventAction->AddLevel(level->GetName());
    energies.push_back(energy);
  }

  eventAction->ClearInitEnergies();
  eventAction->ClearInitPs();
  
  
  for(const auto &e : energies )
  {
    SetRandomDirection(fParticleGun);
    fParticleGun->SetParticleEnergy(e);
    eventAction->AddInitEnergy(e); //save init energy in output file
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  
  
  //generate michel decay or neutron emission
  if(decayFlag)
  {
    G4double energy;
    G4double decayTime = GetDecayTime();
    G4double neutronBR = (1/zinc_lifetime - 1/muon_lifetime) / (1/muon_lifetime);
    if(G4UniformRand() < neutronBR)
    {
      fParticleGun->SetParticleDefinition(neutron);
      energy = GetMichelElectronEnergy();
    }
    else
    {
      fParticleGun->SetParticleDefinition(electron);
      energy = GetNeutronEnergy();
    }
    
  }
  
  
}


void NC2PrimaryGeneratorAction::SetLevels() //hard coding for now, not super elegenant
{

  //levels can`t be a vector, as it will chance the address of the members when adding new ones
  //if I have cross-referenced before, things get messed up
  //
  //one solution is std::unique_ptr : auto level1s = std::uniuqe_ptr<Level>( new Level(1,0,0.5,"1s") );
  //  levels.emplace_back( std::move(level1s) );
  //   level2p1.SetTransition(level1s.get(), 1);
  //
  // or a map should also work. see table in https://en.cppreference.com/w/cpp/container
  
  int nLevels = SetLevelsFromParser(&levels); //

}

Level* NC2PrimaryGeneratorAction::GetSeedLevel()
{
  //method 1: MC selection of a p level
  int method = 1;
  
  bool level_found = false;

  Level* level;
  
  switch(method)
  {
    //lacks some efficiency, but fairly simple
    case 1:
      while(!level_found)
      {
        //select random level
        auto it = levels.begin();
        std::advance(it, std::rand() % levels.size() );
        level = &(it->second);

        if(level->Get_n() == initialN)
        {
          G4double trial_b = G4UniformRand(); //assuming the transition strength are < 1 and proportional to each other
          if( trial_b < level->GetTotalStrength() )
          level_found = true;
        } 
      }
      break;
      
    default: 
      level = NULL; 
      G4cout << " no seed level selection method selected " << G4endl; 
      break;
  }

  
  return level;
}

G4ThreeVector NC2PrimaryGeneratorAction::SetRandomDirection(G4ParticleGun* gun)
{

  //Generate random direction
  G4double cosTheta = (cosThetaHigh-cosThetaLow)*G4UniformRand()+cosThetaLow; 
  G4double phi      = 2*M_PI*G4UniformRand();
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  
  G4double px = cos(phi)*sinTheta ;
  G4double py = sinTheta*sin(phi);
  G4double pz = cosTheta;
    
  eventAction->AddInitPx(px);
  eventAction->AddInitPy(py);
  eventAction->AddInitPz(pz);
  
  gun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
  
}
    
void NC2PrimaryGeneratorAction::PrintLevels()
{
  G4cout << " **************************************** " << G4endl;
  G4cout << " ********** Print Levels's *************** " << G4endl;
  G4cout << " **************************************** " << G4endl << G4endl;
  
  if( levels.size() <1) { G4cout << " !!! unable to print gamma's !!! " << G4endl; return; }
  
  G4cout << " Level	Energy	daughter levels" << G4endl;
  for(auto& x : levels)
  {
    std::vector<Level*>* daughters = x.second.GetDaughterLevels();
    G4cout << x.second.GetName() << " with " << daughters->size() << " daughters " << G4endl ; 
  
    for(unsigned int i = 0; i < daughters->size(); i++)
    {
      //G4cout << std::hex << &(y.first) << " ";
      //G4cout << " daughter name " << y.first->GetName() << " transition energy " << y.second.energy << G4endl;
      G4cout << " daughter name " << daughters->at(i)->GetName() << " with address " << std::hex << daughters->at(i) <<  G4endl;
    }
  }
  
  G4cout << G4endl << " ************************** " << G4endl << G4endl;
}

G4double NC2PrimaryGeneratorAction::GetDecayTime()
{
  G4bool found = false;
  G4double time = 0.*ns;
  while(!found)
  {
    G4double randomTime = G4UniformRand()*5000.*ns; //choose random time 
    G4double exponent = randomTime / zinc_lifetime;
    double result = exp(exponent);
    if(G4UniformRand() < result) 
    {
      found = true;
      time = randomTime;
    }
  }
  return time;  
}

G4double NC2PrimaryGeneratorAction::GetMichelElectronEnergy()
{
  G4bool found = false;
  G4double energy;
  while(!found)
  {
    G4double x = G4UniformRand();
    if(G4UniformRand() < 3*x*x - 2*x*x*x )
    {
      found = true;
      energy = x*muon_mass*0.5;
    }
  }
  return energy;
}

G4double NC2PrimaryGeneratorAction::GetNeutronEnergy()
{
  return 10.*MeV;
}

    
