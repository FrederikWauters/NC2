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
// $Id: NC2GermaniumHit.cc 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file NC2GermaniumHit.cc
/// \brief Implementation of the NC2GermaniumHit class

#include "NC2GermaniumHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<NC2GermaniumHit>* NC2GermaniumHitAllocator=0;


NC2GermaniumHit::NC2GermaniumHit() 
  :    G4VHit(),   fTrackID(-1),   fParentID(-1),   fEdep(0.),   fPos(G4ThreeVector()), fProcessType(-1), fE(0.), fTime(0.)
{}



NC2GermaniumHit::~NC2GermaniumHit() {}



NC2GermaniumHit::NC2GermaniumHit(const NC2GermaniumHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fParentID  = right.fParentID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fProcessType = right.fProcessType;
  fE = right.fE;
  fTime = right.fTime;
}



const NC2GermaniumHit& NC2GermaniumHit::operator=(const NC2GermaniumHit& right)
{
  fTrackID   = right.fTrackID;
  fParentID   = right.fParentID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fProcessType = right.fProcessType;
  fE = right.fE;
  fTime = right.fTime;

  return *this;
}



G4int NC2GermaniumHit::operator==(const NC2GermaniumHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}



void NC2GermaniumHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NC2GermaniumHit::Print()
{
  G4cout << G4endl
     << "  trackID: " << fTrackID  
     << "  parentID: " << fParentID
     << "  Process type: " << fProcessType
     << "  Edep: "  << std::setw(5) << G4BestUnit(fEdep,"Energy")
     << "  Position: "  << std::setw(5) << G4BestUnit( fPos,"Length")
     << "  Energy: " << std::setw(5) << G4BestUnit (fE,"Energy")
     << "  Time: " << std::setw(5) << G4BestUnit(fTime,"Time")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
