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
// $Id: NC2GermaniumSD.cc 87359 2014-12-01 16:04:27Z gcosmo $
//
/// \file NC2GermaniumSD.cc
/// \brief Implementation of the NC2GermaniumSD class

/*
00044 enum G4EmProcessSubType 
00045 { 
00046   fCoulombScattering = 1, 
00047   fIonisation = 2, 
00048   fBremsstrahlung = 3, 
00049   fPairProdByCharged = 4,
00050   fAnnihilation = 5, 
00051   fAnnihilationToMuMu = 6,
00052   fAnnihilationToHadrons = 7,
00053   fNuclearStopping = 8,
00054 
00055   fMultipleScattering = 10, 
00056   
00057   fRayleigh = 11,
00058   fPhotoElectricEffect = 12,
00059   fComptonScattering = 13,
00060   fGammaConversion = 14,
00061   fGammaConversionToMuMu = 15,
00062  
00063   fCerenkov = 21,
00064   fScintillation = 22,
00065   fSynchrotronRadiation = 23,
00066   fTransitionRadiation = 24  
00067 };
00068 


enum G4ProcessType

Enumerator:
    fNotDefined 	
    fTransportation 	
    fElectromagnetic 	
    fOptical 	
    fHadronic 	
    fPhotolepton_hadron 	
    fDecay 	
    fGeneral 	
    fParameterisation 	
    fUserDefined 	
    
    
*/
#include "NC2GermaniumSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4EmProcessSubType.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NC2GermaniumSD::NC2GermaniumSD(const G4String& name,const G4String& hitsCollectionName) : G4VSensitiveDetector(name), fHitsCollection(0)
{
  collectionName.insert(hitsCollectionName);
  G4cout << " construct NC2GermaniumSD::NC2GermaniumSD " << G4endl << G4endl; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NC2GermaniumSD::~NC2GermaniumSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NC2GermaniumSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new NC2GermaniumHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  fHitsCollection->insert(new NC2GermaniumHit());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NC2GermaniumSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{  
  
  //Select on process type. Non EM interaction are of no interest (with just a incident gamma, that will transportation, when I add neutrons .. well, maybe trickier)
  const G4VProcess* process = step->GetPostStepPoint()->GetProcessDefinedStep();

  if ( process->GetProcessType()!=fElectromagnetic ) return false;    
  


  //Make hit
  NC2GermaniumHit* newHit = new NC2GermaniumHit();
  
  //Get info
  newHit->SetProcessType(process->GetProcessSubType());
  newHit->SetParentID(step->GetTrack()->GetParentID());    
  newHit->SetTrackID(step->GetTrack()->GetTrackID());
  newHit->SetEdep(step->GetTotalEnergyDeposit());
  newHit->SetPos(step->GetPostStepPoint()->GetPosition());
  newHit->SetEnergy(step->GetPreStepPoint()->GetKineticEnergy());
  
  // Add Hit
  fHitsCollection->insert( newHit );
  

      
  return true;
}



void NC2GermaniumSD::EndOfEvent(G4HCofThisEvent*)
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


