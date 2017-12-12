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
// $Id: NC2ActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file NC2ActionInitialization.cc
/// \brief Implementation of the NC2ActionInitialization class

#include "NC2ActionInitialization.hh"
#include "NC2PrimaryGeneratorAction.hh"
#include "NC2RunAction.hh"
#include "NC2EventAction.hh"
#include "NC2SteppingAction.hh"
//#include "NC2PrimaryGeneratorMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NC2ActionInitialization::NC2ActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NC2ActionInitialization::~NC2ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NC2ActionInitialization::BuildForMaster() const
{
  SetUserAction(new NC2RunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NC2ActionInitialization::Build() const
{

  //NC2PrimaryGeneratorAction* primaryGenerator = new NC2PrimaryGeneratorAction();
  //SetUserAction(primaryGenerator);
//  SetUserAction(new NC2PrimaryGeneratorMessenger(primaryGenerator));
  
  NC2RunAction* runaction = new NC2RunAction;
  SetUserAction(new NC2RunAction);
  
  NC2EventAction* eventAction = new NC2EventAction(runaction);
  SetUserAction(eventAction);
  
  NC2PrimaryGeneratorAction* primaryGenerator = new NC2PrimaryGeneratorAction(eventAction);
  SetUserAction(primaryGenerator);
  
  SetUserAction(new NC2SteppingAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
