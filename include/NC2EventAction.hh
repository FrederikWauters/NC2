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
// $Id: NC2EventAction.hh 75216 2013-10-29 16:08:11Z gcosmo $
//
/// \file NC2EventAction.hh
/// \brief Definition of the NC2EventAction class

#ifndef NC2EventAction_h
#define NC2EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "NC2RunAction.hh"

/// Event action class
///

class NC2EventAction : public G4UserEventAction
{
  public:
    NC2EventAction(NC2RunAction* ra);
    virtual ~NC2EventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void AddEdep(G4double edep) { fEdep += edep; }
    void SetInitEnergy(G4double value) { initE = value; }
    
    std::vector<G4int>& GetInteractions() { return interactions; }
    std::vector<G4float>& GetXInteractions() { return x_interactions; }
    std::vector<G4float>& GetYInteractions() { return y_interactions; }
    std::vector<G4float>& GetZInteractions() { return z_interactions; }
    
    std::vector<G4float>& GetEDummy() { return e_dummy; }
    std::vector<G4float>& GetXDummy() { return x_dummy; }
    std::vector<G4float>& GetYDummy() { return y_dummy; }
    std::vector<G4float>& GetZDummy() { return z_dummy; }

  private:
    G4double  fEdep;
    //hit collection ID's
    G4int fGeHCID;
    G4int fDummyHCID;
    NC2RunAction* runAction;
    G4double initE; //set from primary generator
    std::vector<G4int> interactions; //set to fill tree
    std::vector<G4float> x_interactions;
    std::vector<G4float> y_interactions; 
    std::vector<G4float> z_interactions;
    
 
    std::vector<G4float> e_dummy;
    std::vector<G4float> x_dummy;
    std::vector<G4float> y_dummy; 
    std::vector<G4float> z_dummy;
    
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
