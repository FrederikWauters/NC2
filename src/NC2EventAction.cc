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

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"


NC2EventAction::NC2EventAction(NC2RunAction* ra): G4UserEventAction(),  fEdep(0.), fGeHCID(-1), fDummyHCID(-1)
{
  runAction = ra;
  initE = 0.*MeV;

} 


NC2EventAction::~NC2EventAction()
{}


void NC2EventAction::BeginOfEventAction(const G4Event*)
{    
 G4SDManager* sdManager = G4SDManager::GetSDMpointer();
 if (fGeHCID==-1) 
 {
      
      fGeHCID = sdManager->GetCollectionID("GeSD/GeHitsCollection");
 }
 if (fDummyHCID==-1) 
 {
      fDummyHCID = sdManager->GetCollectionID("DummySD/DummyHitsCollection");
 }
 
   //empty vectors
  //runAction->ClearVectors();
  interactions.clear();
  x_interactions.clear();
  y_interactions.clear();
  z_interactions.clear();
  e_dummy.clear();
  x_dummy.clear();
  y_dummy.clear(); 
  z_dummy.clear();
}


void NC2EventAction::EndOfEventAction(const G4Event* event)
{   
  
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if (!hce) 
  {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl; 
      G4Exception("NC2EventAction::EndOfEventAction()",  "Code001", JustWarning, msg);
      return;
  }   
  
 
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); 
 
  //*******************************
  //Process Germanium detector hits
  //*******************************
  
  // Get hits collections 
  NC2GermaniumHitsCollection* geHC = static_cast<NC2GermaniumHitsCollection*>(hce->GetHC(fGeHCID));
  if(!geHC) return;   // !!!!!!!!! For now, events with no Ge detector hit will be ignored!!!!!!!!!
  G4int n_hit = geHC->entries();
  
  if(n_hit > 0)
  {
    G4double eDep = 0.;
    for(int i=0; i<n_hit; i++)
    {
      NC2GermaniumHit* hit = (*geHC)[i];
      G4int trackID =  hit->GetTrackID();
      G4int parentID = hit->GetParentID();
      if(trackID>0) //the first "hit" is not really a hit. Not sure if this is a general thing, so I check in the track number
      {
        eDep += hit->GetEdep();
        if(trackID ==1) //follow the primary particle as It goes through the detector
        {
          //G4cout << " Set interaction point of type " << hit->GetProcessType() << G4endl;
          G4ThreeVector pos  = hit->GetPos();
          G4int type = hit->GetProcessType(); //check in SD code what is saved. For now, just EM interactions 13 is coulomb interactions
          interactions.push_back(type);
          x_interactions.push_back(pos.x());
          y_interactions.push_back(pos.y());
          z_interactions.push_back(pos.z());
        }
      }
    }
    analysisManager->FillNtupleDColumn(0, eDep);
    analysisManager->FillNtupleDColumn(1, initE);
  }  
   
  //******************
  //Process Dummy hits
  //******************
    
  //Dummy Hits collection
  NC2DummyHitsCollection* dummyHC = static_cast<NC2DummyHitsCollection*>(hce->GetHC(fDummyHCID)); //only gamma's are in the hits collection
    
  n_hit = dummyHC->entries(); 
  if(n_hit > 0)
  {
    for(int i=0; i<n_hit; i++)
    {
      NC2DummyHit* hit = (*dummyHC)[i];
      G4float energy = hit->GetEnergy();
      G4int trackID =  hit->GetTrackID();
      if(energy<initE-1. && trackID>0)
      {
        G4ThreeVector pos = hit->GetPos();
        e_dummy.push_back(energy);
        x_dummy.push_back(pos.x());
        y_dummy.push_back(pos.y());
        z_dummy.push_back(pos.z());
      }
    }
  }
  
  
  //***********************
  //* ADD ROW TO NTUPLE ***
  //***********************
 
  analysisManager->AddNtupleRow();  //fill only get set if there was a GeSD hit
    
  
}

