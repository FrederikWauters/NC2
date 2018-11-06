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



NC2PrimaryGeneratorAction::NC2PrimaryGeneratorAction(NC2EventAction* ea): G4VUserPrimaryGeneratorAction(),  fParticleGun(0), eventAction(ea), engine(random_device()), dis_0_1(0,1), dis_0_14(0,14)
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
  electron  = particleTable->FindParticle(particleName="e-");
  neutron  = particleTable->FindParticle(particleName="neutron");
  fParticleGun->SetParticleDefinition(xray);
  defaultEnergy = 1.*MeV; //default energy
  initialN = 15;
  muon_lifetime = 2197.*ns;
  zinc_lifetime = 160.*ns;
  muon_mass = 105.65*MeV;
  levels.clear();
  SetLevels();
  start_levels.clear();
  PrintLevels();
  decayFlag = true;
  
  l_names = Get_l_names();
  
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

  //clean truth info  
  eventAction->ClearInitLevels();
  eventAction->ClearPIDs();
  eventAction->ClearInitEnergies();
  eventAction->ClearInitPs();
  
  //Get Seed Level
  Level* level = GetSeedLevel();
  
  std::vector<G4double> energies;
  

  
  eventAction->AddLevel(level->GetName());
  
  while(level->Get_n() > 1)
  {
    G4double energy;
    G4bool is_radiative;
    level = level->GetTransition(&energy,&is_radiative);
    eventAction->AddLevel(level->GetName());
    if(is_radiative) energies.push_back(energy);
  }

  eventAction->ClearInitEnergies();
  eventAction->ClearInitPs();
  
  
  for(const auto &e : energies )
  {
    SetRandomDirection(fParticleGun);
    fParticleGun->SetParticleEnergy(e);
    eventAction->AddInitEnergy(e); //save init energy in output file
    eventAction->AddPID(1);
    fParticleGun->SetParticleTime(0.*ns);
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  

  //generate michel decay or neutron emission
  if(decayFlag)
  {
    G4double energy;
    G4double decayTime = GetDecayTime();
    //G4cout << "decayTime " << decayTime << G4endl;
    fParticleGun->SetParticleTime(decayTime*ns);
    G4double neutronBR = ( 1/zinc_lifetime - 1/muon_lifetime) / (1/zinc_lifetime);
    //G4cout << "neutronBR " <<  neutronBR << G4endl;
    if(G4UniformRand() < neutronBR)
    {
      fParticleGun->SetParticleDefinition(neutron);
      eventAction->AddPID(13);
      energy = GetNeutronEnergy();
    }
    else
    {
      fParticleGun->SetParticleDefinition(electron);
      eventAction->AddPID(3);
      energy = GetMichelElectronEnergy();
    }
    SetRandomDirection(fParticleGun);
    fParticleGun->SetParticleEnergy(energy);
    eventAction->AddInitEnergy(energy);
    fParticleGun->GeneratePrimaryVertex(anEvent); 
  
  }
  
  
}


int NC2PrimaryGeneratorAction::SetLevels() //hard coding for now, not super elegenant
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
  return nLevels;

}

Level* NC2PrimaryGeneratorAction::GetSeedLevel()
{
  //method 1: MC selection of a p level
  int method = 3;
  
  bool level_found = false;

  Level* level;
  
  //select start levels
  if(start_levels.size() < 1)
  {
    start_strength = 0.;          
    for(auto& x : levels)
    {
      Level* l = &(x.second);
      if(l->Get_n() == initialN)
      {
        start_levels.push_back(l);
        start_strength += l->GetTotalStrength();
      }
    }
    printf("Start levels initialized\n");
  }
  
  switch(method)
  {
    //lacks some efficiency, but fairly simple
    case 1:
      while(!level_found)
      {
        //select random level
        auto it = levels.begin();
        std::advance(it, std::rand() % levels.size() ); //rand() is apparently not a great thing to use https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful
        level = &(it->second);

        if(level->Get_n() == initialN)
        {
          G4double trial_b = G4UniformRand(); //assuming the transition strength are < 1 and proportional to each other
          if( trial_b < level->GetTotalStrength() )
          level_found = true;
        } 
      }
      break;
      
    case 2:
      
      while(!level_found)
      {
        //select random level
        G4int trial_a = int(trunc(G4UniformRand()*start_levels.size()));
        level = start_levels.at(trial_a);
        if(G4UniformRand()*start_strength < level->GetTotalStrength() ) level_found = true;
      }
      break;
      
    case 3:
      //use 2l+1 distribution
      //initialN;
      while(!level_found)
      {
        bool l_found = false;
        int l;
        while(l_found !=true)
        {
          if(initialN <= 15)
          {
            int l_trial = dis_0_14(engine);
            double trial = G4UniformRand()*(2*(initialN-1)+1);
            if(l_trial < initialN-1 and trial < (2*l_trial+1) )
            {
              l = l_trial;
              l_found = true;
            }
          }          
          else
          {
            G4cout << "This method does not work for initial N = " << initialN << G4endl;
            break;
          }
        }
        
        bool j_found = false;
        std::string j;
        
        while(j_found != true)
        {
          if( l_found==0 )
          {
            j = "1/2";
            j_found = true;
            continue;            
          }
          
          //chose sign
          int delta_spin;
          if(dis_0_1(engine) == 0) delta_spin = -1;
            else delta_spin = +1;
          
          if(delta_spin == +1)
          {
            j = std::to_string(l+delta_spin) + "/2";
            j_found = true;
            continue;
          }
          float trial_j = float(l) + delta_spin*0.5;
          float max_j = float(l) + 0.5;
          if( G4UniformRand()*(2*max_j+1) < (2*trial_j+1) )
          {
            j_found = true;
            j = std::to_string(l+delta_spin) + "/2";
            continue;
          }     
        }
        

        std::string key = std::to_string(initialN) + l_names[l] + "_" + j;
        
        if(levels.find(key) != levels.end() )
        {
          level = &(levels[key]);
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
  
  return G4ThreeVector(px,py,pz);
  
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
    G4double exponent = -randomTime / zinc_lifetime;
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
  //taken from "The nuclear physics of muon capture", Measday and references therein
  
  double high_energy_fraction=0.1;
  double theta = 1.;
  double T = 8.;
  double minE = 0.5;
  double maxE = 15.;
  
  G4bool energy_found = false;
  G4double energy;
  
  while(energy_found != true )
  {
    double e = minE + G4UniformRand()*(maxE-minE);
    double trial_value = G4UniformRand();
    
    double y = pow(e,(5./11))*exp(-e/theta)+high_energy_fraction*exp(-e/T);
    
    if(y<1 && trial_value <y)
    {
      energy_found = true;
      energy = e*MeV;
    }
    
  }
  
  return energy;
}

    
