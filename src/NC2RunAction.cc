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

#include <string>
#include <ctime>

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

  
  
  //set root file output
  char filename[256] = "out.root";
  treeFile = TFile::Open(filename, "recreate");   
  if(!treeFile || treeFile->IsZombie()){ printf("Could not open tree file %s!\n", filename); }
  else
  {
    std::cout << "**************************" << std::endl;
    std::cout << "writing trees to " << filename << std::endl;
  }  
  treeFile->cd(); 

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
  
  //set random engine seed
  time_t t = std::time(0);
  long int now = static_cast<long int> (t);
  CLHEP::HepRandom::setTheSeed(now);
  
  const NC2EventAction* constEventAction = static_cast<const NC2EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
  NC2EventAction* eventAction = const_cast<NC2EventAction*>(constEventAction);
  
  //define output
  tree = new TTree("EventTree","MC muonic X-ray events");
  TTree::SetBranchStyle(1);
  
  test_value = 2;
  
  //tree->Branch("test",&test_value,"test/I");
  tree->Branch("EInits",eventAction->GetEInits());
  tree->Branch("pxInits",eventAction->GetPxInits());
  tree->Branch("pyInits",eventAction->GetPyInits());
  tree->Branch("pzInits",eventAction->GetPzInits());
  tree->Branch("Levels",eventAction->GetLevels());
  tree->Branch("Edeps",eventAction->GetEnergyDepositions());
  
  runTree = new TTree("RunTree","Simulation config");

  runTree->Branch("nEvents",eventAction->GetNevents(),"nEvents/l");
  
  hEPrimary = new TH1D();
  
  hPPrimary = new TH3D();

}



void NC2RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  const NC2Run* b1Run = static_cast<const NC2Run*>(run);
  
  const NC2EventAction* constEventAction = static_cast<const NC2EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
  NC2EventAction* eventAction = const_cast<NC2EventAction*>(constEventAction);
  
  runTree->Fill();
  
  *hEPrimary = eventAction->GetPrimaryEnergyHistogram();
  hEPrimary->Write();
  
  *hPPrimary = eventAction->GetPrimaryMomentumHistogram();
  hPPrimary->Write();
  
  if(treeFile){
   treeFile->Write(0,TObject::kOverwrite); //https://root.cern.ch/root/roottalk/roottalk02/0899.html cycles and such
   treeFile->Close();
  }
}



