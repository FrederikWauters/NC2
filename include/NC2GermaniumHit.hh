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
// $Id: NC2GermaniumHit.hh 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file NC2GermaniumHit.hh
/// \brief Definition of the NC2GermaniumHit class

#ifndef NC2GermaniumHit_h
#define NC2GermaniumHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, energy deposit,
/// and position of particles in a selected volume:
/// - fTrackID, fEdep, fPos

class NC2GermaniumHit : public G4VHit
{
  public:
    NC2GermaniumHit();
    NC2GermaniumHit(const NC2GermaniumHit&);
    virtual ~NC2GermaniumHit();

    // operators
    const NC2GermaniumHit& operator=(const NC2GermaniumHit&);
    G4int operator==(const NC2GermaniumHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int id)      { fTrackID = id; };
    void SetParentID (G4int id)      { fParentID = id; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetEdep     (G4double value)   { fEdep = value; };
    void SetProcessType (G4int type) {fProcessType = type; };
    void SetEnergy (G4double value) { fE = value; };
    void SetTime (G4double value) { fTime = value; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetParentID() const     { return fParentID; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    G4int GetProcessType() const { return fProcessType; };
    G4double GetEnergy() const { return fE; };
    G4double GetTime() const { return fTime; };

  private:

      G4int         fTrackID;
      G4int         fParentID;
      G4double      fEdep;
      G4ThreeVector fPos;
      G4int	    fProcessType;
      G4double      fE;
      G4double      fTime;
};



typedef G4THitsCollection<NC2GermaniumHit> NC2GermaniumHitsCollection;

extern G4ThreadLocal G4Allocator<NC2GermaniumHit>* NC2GermaniumHitAllocator;



inline void* NC2GermaniumHit::operator new(size_t)
{
  if(!NC2GermaniumHitAllocator)    NC2GermaniumHitAllocator = new G4Allocator<NC2GermaniumHit>;
  return (void *) NC2GermaniumHitAllocator->MallocSingle();
}



inline void NC2GermaniumHit::operator delete(void *hit)
{
  NC2GermaniumHitAllocator->FreeSingle((NC2GermaniumHit*) hit);
}



#endif
