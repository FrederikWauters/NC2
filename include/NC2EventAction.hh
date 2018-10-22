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


#include <map>
#include <utility>
#include <TH1D.h>
#include <TH3D.h>

/// Event action class
///

class NC2EventAction : public G4UserEventAction
{
  public:
    NC2EventAction();
    virtual ~NC2EventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

   
    void ClearInitEnergies() { initEs.clear(); };
    void ClearInitLevels() { levels.clear(); };
    void AddInitEnergy(G4double value) { initEs.push_back(value); };
    void AddLevel(std::string value) { levels.push_back(value); };
    
    std::map<std::string, G4double>* GetGermaniumEnergies() { return &germaniumEnergy; };
    std::map<std::string, G4double>* GetGermaniumTimes() { return &germaniumTime; };
    //std::pair<G4int,G4int>* GetGermaniumHits() { return &test_pair; };
    std::vector<G4double>* GetEInits() {return &initEs; };
    std::vector<std::string>* GetLevels() { return &levels; };
    unsigned long int* GetNevents() { return &nEvents; };
    TH1D GetPrimaryEnergyHistogram() { return *hEPrimary; };
    TH3D GetPrimaryMomentumHistogram() { return *hPPrimary; };
    
    G4bool* GetPileUpFlag() { return &pileUpFlag; };
    
    std::vector<G4double>* GetPxInits() {return &initPxs; };
    std::vector<G4double>* GetPyInits() {return &initPys; };
    std::vector<G4double>* GetPzInits() {return &initPzs; };
    
    void AddInitPx(G4double value) { initPxs.push_back(value); };
    void AddInitPy(G4double value) { initPys.push_back(value); };
    void AddInitPz(G4double value) { initPzs.push_back(value); };
    void ClearInitPs() { initPxs.clear(); initPys.clear(); initPzs.clear(); };
    

  private:
  
    void AddGermaniumHit(std::string detector, G4double e, G4double t);
  
    NC2RunAction* runAction;
        
    //hit collection ID's
    std::map<std::string, G4int> geHCIDs;
    
    //detector hit info
    std::map<std::string, G4double > germaniumEnergy;
    std::map<std::string, G4double > germaniumTime;
    G4bool pileUpFlag;
    
    std::pair<G4int,G4int> test_pair;
    
    //event info
    std::vector<G4double> initEs; //set from primary generator
    std::vector<G4double> initPxs;
    std::vector<G4double> initPys;
    std::vector<G4double> initPzs;
    TH1D* hEPrimary; // histogram primary photons
    TH3D* hPPrimary;
        
    std::vector<std::string> levels; //levels during the cascade
    
    unsigned long int nEvents;
    
   
    
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
