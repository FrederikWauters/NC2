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
// $Id: NC2PrimaryGeneratorAction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file NC2PrimaryGeneratorAction.hh
/// \brief Definition of the NC2PrimaryGeneratorAction class

#ifndef NC2PrimaryGeneratorAction_h
#define NC2PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "NC2EventAction.hh"
#include <vector>
	

class G4ParticleGun;
class G4Event;
class G4Box;
class G4GenericMessenger;
class NC2PrimaryGeneratorMessenger;


/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class NC2PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    NC2PrimaryGeneratorAction(NC2EventAction* ea);    
    virtual ~NC2PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
    
    G4double GetEnergy();
    


    void SetNewEnergy(G4double value) ;
    void SetNewIntensity(G4double value) ;
    void SetNewUse(G4bool value) ;
    
    struct Gamma {
    double energy;
    double intensity;
    bool use;
    };
    
    void PrintGammas();
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    
    int rand (void);

    NC2PrimaryGeneratorMessenger* messenger;
    NC2EventAction* eventAction;
    
    //source properties
    G4double cosThetaLow;
    G4double cosThetaHigh;
    
    std::vector<Gamma> gammas;
    G4int entries;
    G4double defaultEnergy;
    
    //Energies en intensities
    
//    G4double 2p1s[2];
//    G4double 3p1s[2];
//    G4double 4p1s[2];
//    G4double 5p1s[2];
//    G4double 6p1s[2];
//    G4double 7p1s[2];
//    G4double 8p1s[2];
//    G4double 9p1s[2];
//    G4double 10p1s[2];
//    G4double 11p1s[2];
//    G4double 12p1s[2];
//    G4double 13p1s[2];
//    G4double 14p1s[2];
//    G4double 15p1s[2];
//    
//    G4double 2s1s[2];
//    G4double 16p1s[2];
//    
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


