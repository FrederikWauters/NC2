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
// $Id: NC2EventAction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file NC2EventAction.cc
/// \brief Implementation of the NC2EventAction class

#include "NC2EventAction.hh"
#include "NC2Run.hh"
#include "NC2GermaniumHit.hh"
#include "NC2DummyHit.hh"
#include "NC2Analysis.hh"
#include "NC2DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"


NC2EventAction::NC2EventAction(): G4UserEventAction()
{
  //runAction = ra;
  geHCIDs.clear();
    germaniumEnergy.clear();
  germaniumTime.clear();
  pileUpFlag = false;
  
  
  const NC2RunAction* constRunAction = static_cast<const NC2RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction());
  runAction = const_cast<NC2RunAction* > (constRunAction);
  
  nEvents = 0;
  
  test_pair = std::make_pair(1,2);
  
  hEPrimary = new TH1D("hEPrimary","Primary photon energies for the entire MC run; Energy (keV)",10000,0,10000);
  hPPrimary = new TH3D("hPPrimary","Primart photon momentum for entire MC run; px;py;pz",200,-1,1,200,-1,1,200,-1,1);
  
} 


NC2EventAction::~NC2EventAction()
{}


void NC2EventAction::BeginOfEventAction(const G4Event*)
{    
 
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();

  if(geHCIDs.size() < 1)
  {
    const NC2DetectorConstruction* constDetectorConstruction = static_cast<const NC2DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    NC2DetectorConstruction* detectorConstruction = const_cast<NC2DetectorConstruction*>(constDetectorConstruction);
    std::vector<std::string> geDetNames = detectorConstruction->GetGeDetectorNames();
  
    for(auto &name: geDetNames)
    {
      std::string sd_name = name + "SD" + "/" + name + "HitsCollection";
      int id = sdManager->GetCollectionID(sd_name);
      geHCIDs[name] = id;
      G4cout << name << " hc id = " << id << G4endl;
    }
    
  }
 
  //empty vector and maps
  //initEs.clear(); //called after prim event generator, so don`t clear here
  germaniumEnergy.clear();
  germaniumTime.clear();
  pileUpFlag = false;


}


void NC2EventAction::EndOfEventAction(const G4Event* event)
{   
  //G4cout << "end of event , initEs size : " << initEs.size() << G4endl; 
  
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if (!hce) 
  {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl; 
      G4Exception("NC2EventAction::EndOfEventAction()",  "Code001", JustWarning, msg);
      return;
  } 
  
  //*******************************
  //Process Germanium detector hits
  //*******************************
  
  bool fill = false;
  
  const NC2DetectorConstruction* constDetectorConstruction = static_cast<const NC2DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  NC2DetectorConstruction* detectorConstruction = const_cast<NC2DetectorConstruction*>(constDetectorConstruction);
  std::vector<std::string> geDetNames = detectorConstruction->GetGeDetectorNames();
 
  for(auto &name: geDetNames)
  {
    int hcID = geHCIDs[name];
    NC2GermaniumHitsCollection* geHC = static_cast<NC2GermaniumHitsCollection*>(hce->GetHC(hcID));
    G4int n_hit = geHC->entries();
    
    G4double eDep = 0.;
    G4double time = 0.;
    G4double prevTime = 0.;
    G4double eMax = 0.*MeV;
    for(int i=0; i<n_hit; i++)
    {
      NC2GermaniumHit* hit = (*geHC)[i];
      G4int trackID =  hit->GetTrackID();
      G4int parentID = hit->GetParentID();
      if(trackID>0) //the first "hit" is not really a hit. Not sure if this is a general thing, so I check in the track number
      {
        eDep += hit->GetEdep();
        fill = true;
        if( ( (time-prevTime) > 100.*ns || (time-prevTime) < 100.*ns ) && hit->GetEdep() > 10.*keV ) pileUpFlag = true; //hard code pile-up flag for now, with minimum energy requirement
        if( hit->GetEdep() > 10.*keV ) prevTime = time; 
        if( hit->GetEdep() > eMax ) { eMax=hit->GetEdep();     time = hit->GetTime(); } //take time of highest eDep
      }      
    }
    
    AddGermaniumHit(name,eDep,time);
  }  
  
  
  if(fill) 
  {
    //G4cout << "initEs size : " << initEs.size() << G4endl;
    //G4cout << "energyDepositions size : " << energyDepositions.size() << G4endl;
    //G4cout << " test value " << runAction->GetTestValue() << G4endl;
    runAction->FillTree(); //don`t write event with no ge hits
  }
  
  for(const auto &e: initEs )
  {
    double e_keV = (double)e*1000.;
    hEPrimary->Fill(e_keV);
  }
  
  for(unsigned int iP = 0; iP < initPxs.size(); iP++)
  {
    hPPrimary->Fill(initPxs.at(iP),initPys.at(iP),initPzs.at(iP));
  }

  nEvents++;
  return;
}  
                   
void NC2EventAction::AddGermaniumHit(std::string detector, G4double e, G4double t)
{

  germaniumEnergy[detector] = e;
  germaniumTime[detector] = t;
  
  return;  
} 
  


