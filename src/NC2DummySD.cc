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
// $Id: NC2DummySD.cc 87359 2014-12-01 16:04:27Z gcosmo $
//
/// \file NC2DummySD.cc
/// \brief Implementation of the NC2DummySD class


#include "NC2DummySD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4EmProcessSubType.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NC2DummySD::NC2DummySD(const G4String& name,const G4String& hitsCollectionName) : G4VSensitiveDetector(name), fHitsCollection(0)
{
  collectionName.insert(hitsCollectionName);
  G4cout << " construct NC2DummySD::NC2DummySD " << G4endl << G4endl; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NC2DummySD::~NC2DummySD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NC2DummySD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new NC2DummyHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  fHitsCollection->insert(new NC2DummyHit());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NC2DummySD::ProcessHits(G4Step* step, G4TouchableHistory*)
{  
  //Get particle type, we only want gammas
  if(step->GetTrack()->GetDefinition()->GetParticleName()!="gamma") return false;
  //Make hit
  NC2DummyHit* newHit = new NC2DummyHit();
     
  //Get info;
  newHit->SetParentID(step->GetTrack()->GetParentID());
  newHit->SetTrackID(step->GetTrack()->GetTrackID());
  newHit->SetPos(step->GetPostStepPoint()->GetPosition());
  newHit->SetEnergy(step->GetPreStepPoint()->GetKineticEnergy());
  
  // Add Hit
  fHitsCollection->insert( newHit );
      
  return true;
}



void NC2DummySD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) 
  { 
     G4int nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     for( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}


