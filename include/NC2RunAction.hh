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
// $Id: NC2RunAction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file NC2RunAction.hh
/// \brief Definition of the NC2RunAction class

#ifndef NC2RunAction_h
#define NC2RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"


//#include "TH1F.h"
#include <vector>

class G4Run;
class G4LogicalVolume;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class NC2RunAction : public G4UserRunAction
{
  public:
    NC2RunAction();
    virtual ~NC2RunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    
    //void ClearVectors();
    //void SetInteractionPoint(G4ThreeVector vec);
    //void SetInteraction(G4int value) { interactions.push_back(value); };
    //void PrintVector();
    
     
    
  private:
    //std::vector<G4float> x_interactions; needs to be defines in RunActions
    //std::vector<G4float> y_interactions; 
    //std::vector<G4float> z_interactions; 
    //std::vector<G4int> interactions; 


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
