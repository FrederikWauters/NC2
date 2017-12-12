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
// $Id: NC2RunAction.cc 87359 2014-12-01 16:04:27Z gcosmo $
//
/// \file NC2RunAction.cc
/// \brief Implementation of the NC2RunAction class

#include "NC2RunAction.hh"
#include "NC2PrimaryGeneratorAction.hh"
#include "NC2DetectorConstruction.hh"
#include "NC2Run.hh"
#include "NC2Analysis.hh"
#include "NC2EventAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VNtupleManager.hh"


//#include "TFile.h"

NC2RunAction::NC2RunAction()
: G4UserRunAction()
{ 
  
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in B5Analysis.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("outfile");  

}



NC2RunAction::~NC2RunAction()
{}



G4Run* NC2RunAction::GenerateRun()
{

  return new NC2Run; 
}



void NC2RunAction::BeginOfRunAction(const G4Run*)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  
  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file 
  // The default file name is set in B5RunAction::B5RunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
  
  // Creating 1D histograms
  //analysisManager->CreateH1("hGermaniumEdep","Energy deposition in Ge detector", 4000, 0., 4000); //this does work with the newest root version
  // http://hypernews.slac.stanford.edu/HyperNews/geant4/get/analysis/474.html
  
  // Get event action
  const NC2EventAction* constEventAction = static_cast<const NC2EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
  NC2EventAction* eventAction = const_cast<NC2EventAction*>(constEventAction);
  
  analysisManager->CreateNtuple("Germanium", "Germanium detector hits");
  //basic Ge detector info
  analysisManager->CreateNtupleDColumn("Edep"); //0
  analysisManager->CreateNtupleDColumn("Einit");
  //multiplicity in the detector
  analysisManager->CreateNtupleIColumn("Interactions",eventAction->GetInteractions()); //2
  analysisManager->CreateNtupleFColumn("x_interactions",eventAction->GetXInteractions());
  analysisManager->CreateNtupleFColumn("y_interactions",eventAction->GetYInteractions());
  analysisManager->CreateNtupleFColumn("z_interactions",eventAction->GetZInteractions());
  
  //dummy detector hits
  analysisManager->CreateNtupleFColumn("E_Dummy",eventAction->GetEDummy()); //6
  analysisManager->CreateNtupleFColumn("x_Dummy",eventAction->GetXDummy());
  analysisManager->CreateNtupleFColumn("y_Dummy",eventAction->GetYDummy());
  analysisManager->CreateNtupleFColumn("z_Dummy",eventAction->GetZDummy());

  //do work now, but with some effort:  http://hypernews.slac.stanford.edu/HyperNews/geant4/get/analysis/473/1/1/1.html
 
  analysisManager->FinishNtuple();   
}



void NC2RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  const NC2Run* b1Run = static_cast<const NC2Run*>(run);
  
  // save histograms & ntuple
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();  
}


/*void NC2RunAction::ClearVectors()
{
  if(interactions.size()>0)
  {
    //for(int i = 0; i < interactions.size(); i++) G4cout << interactions.at(i) << G4endl;
  }

  x_interactions.clear();
  y_interactions.clear();
  z_interactions.clear();
  interactions.clear();

}*/


/*void NC2RunAction::SetInteractionPoint(G4ThreeVector vec)
{
  x_interactions.push_back(vec[0]);
  y_interactions.push_back(vec[1]);
  z_interactions.push_back(vec[2]);
}


void NC2RunAction::PrintVector()
{
  G4cout << "size vector : " << interactions.size() << G4endl;
  if(interactions.size() > 0) 
  {
    for(int i = 0; i < interactions.size(); i++) G4cout << " interactions.at("<<i<<") = " << interactions.at(i) << G4endl;
  }
}*/
