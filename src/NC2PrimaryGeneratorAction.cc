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

#include<iterator>
#include<cstdlib>


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
  G4ParticleDefinition* particle  = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
  defaultEnergy = 1.*MeV; //default energy
  levels.clear();
  SetLevels();
  PrintLevels();
  
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
  
  
  //1S  
  Level level1s(1,0,0.5,"1s");
  level1s.SetEnergy(0); // set 1s energy level to "0", instead of 
  levels[ "1s" ] = level1s;
  
  
  
  //2P levels
  Level level2p1(2,1,0.5,"2p_1/2");
  level2p1.SetEnergy(1595.42*keV);
  level2p1.SetTransition(&levels["1s"],1);
  levels[ level2p1.GetName() ] = level2p1;
  
  Level level2p2(2,1,1.5,"2p_3/2");
  level2p2.SetEnergy(1602.61*keV);
  level2p2.SetTransition(&levels["1s"],1);
  levels[ level2p2.GetName() ] = level2p2;
    
  
  //2S
  Level level2s(2,0,0.5,"2s");
  level2s.SetEnergy(1639.476*keV);
  level2s.SetTransition(&levels["2p_1/2"],1-0.0024); //Feyman and Chen
  level2s.SetTransition(&levels["1s"],0.0024);
  levels[ level2s.GetName() ] = level2s;
  
  
  
  //these transition strengths are preliminary, and have to come from either data, or Vogel's calculation
  
  //3P
  Level level3p1(3,1,0.5,"3p_1/2");
  level3p1.SetEnergy(1953.921*keV); 
  level3p1.SetTransition(&levels["1s"],0.1); //my miniball 2016 note
  level3p1.SetTransition(&levels["2s"],0.022);
  levels[ level3p1.GetName() ] = level3p1;
  
  Level level3p2(3,1,1.5,"3p_3/2");
  level3p2.SetEnergy(1956.013*keV);
  level3p2.SetTransition(&levels["1s"],0.1);
  level3p2.SetTransition(&levels["2s"],0.02);
  levels[ level3p2.GetName() ] = level3p2;
  
  //4P
  Level level4p1(4,1,0.5,"4p_1/2");
  level4p1.SetEnergy(2078.919*keV);
  level4p1.SetTransition(&levels["1s"],0.028);
  level4p1.SetTransition(&levels["2s"],0.0046);//https://muon.npl.washington.edu/elog/neutralcurrents/Plans+and+Analysis/11
  levels[ level4p1.GetName() ] = level4p1;
  
  Level level4p2(4,1,0.5,"4p_3/2");
  level4p2.SetEnergy(2079.794*keV);
  level4p2.SetTransition(&levels["1s"],0.028);
  level4p2.SetTransition(&levels["2s"],0.0046);
  levels[ level4p2.GetName() ] = level4p2;
  
  //5P
  Level level5p1(5,1,0.5,"5p_1/2");
  level5p1.SetEnergy(2136.6*keV);
  level5p1.SetTransition(&levels["1s"],0.017);
  level5p1.SetTransition(&levels["2s"],0.0036);
  levels[ level5p1.GetName() ] = level5p1;
  
  Level level5p2(5,1,1.5,"5p_3/2");
  level5p2.SetEnergy(2134.247*keV);
  level5p2.SetTransition(&levels["1s"],0.017);
  level5p2.SetTransition(&levels["2s"],0.0036);
  levels[ level5p2.GetName() ] = level5p2;
  
  //6P
  Level level6p1(6,1,0.5,"6p_1/2");
  level6p1.SetEnergy(2164.521*keV);
  level6p1.SetTransition(&levels["1s"],0.022);
  level6p1.SetTransition(&levels["2s"],0.0043);
  levels[ level6p1.GetName() ] = level6p1;
  
  Level level6p2(6,1,1.5,"6p_3/2");
  level6p2.SetEnergy(2164.779*keV);
  level6p2.SetTransition(&levels["1s"],0.022);
  level6p2.SetTransition(&levels["2s"],0.0043);
  levels[ level6p2.GetName() ] = level6p2;
  
  //7P
  Level level7p1(7,1,0.5,"6p_1/2");
  level7p1.SetEnergy(2184*keV);
  level7p1.SetTransition(&levels["1s"],0.021);
  level7p1.SetTransition(&levels["2s"],0.0034);
  levels[ level7p1.GetName() ] = level7p1;
  
  Level level7p2(7,1,1.5,"7p_3/2");//for consistency, current transition strength are per muon * 2 (as there are two finestates)
  level7p2.SetEnergy(2184*keV);
  level7p2.SetTransition(&levels["1s"],0.021);
  level7p2.SetTransition(&levels["2s"],0.0034);
  levels[ level7p2.GetName() ] = level7p2;
  
 
  //8P
  Level level8p1(8,1,0.5,"8p_1/2");
  level8p1.SetEnergy(2197*keV);
  level8p1.SetTransition(&levels["1s"],0.02);
  level8p1.SetTransition(&levels["2s"],0.003);
  levels[ level8p1.GetName() ] = level8p1;
  
  Level level8p2(8,1,1.5,"8p_3/2");//for consistency, current transition strength are per muon * 2 (as there are two finestates)
  level8p2.SetEnergy(2197*keV);
  level8p2.SetTransition(&levels["1s"],0.02);
  level8p2.SetTransition(&levels["2s"],0.003);
  levels[ level8p2.GetName() ] = level8p2;
  
  //9+P
  Level level9p1(8,1,0.5,"9p+1");
  level9p1.SetEnergy(2220*keV);
  level9p1.SetTransition(&levels["1s"],0.1);
  level9p1.SetTransition(&levels["2s"],0.018);
  levels[ level9p1.GetName() ] = level9p1;
  
  Level level9p2(8,1,1.5,"9p+2"); //for consistency, current transition strength are per muon * 2 (as there are two finestates)
  level9p2.SetEnergy(2220*keV);
  level9p2.SetTransition(&levels["1s"],0.1);
  level9p2.SetTransition(&levels["2s"],0.018);
  levels[ level9p2.GetName() ] = level9p2;


}

Level* NC2PrimaryGeneratorAction::GetSeedLevel()
{
  //method 1: MC selection of a p level
  int method = 1;
  
  bool level_found = false;

  Level* level;
  
  switch(method)
  {
    case 1:
      while(!level_found)
      {
        //select random level
        auto it = levels.begin();
        std::advance(it, std::rand() % levels.size() );
        level = &(it->second);

        if(level->Get_l() == 1)
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
    G4cout << x.second.GetName() << "	" << x.second.GetEnergy() << "	" << daughters->size() << G4endl; 
  
    for(unsigned int j = 0; j < daughters->size(); j++)
    {
      G4cout << " daughter n " << daughters->at(j)->Get_n() << "	daughter l " << daughters->at(j)->Get_n() << "	daughter name " << daughters->at(j)->GetName() << G4endl;
    }
  }
  
  G4cout << G4endl << " ************************** " << G4endl << G4endl;
}
    
