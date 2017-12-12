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
// $Id: NC2DummyHit.cc 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file NC2DummyHit.cc
/// \brief Implementation of the NC2DummyHit class

#include "NC2DummyHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<NC2DummyHit>* NC2DummyHitAllocator=0;


NC2DummyHit::NC2DummyHit() 
  :    G4VHit(),   fTrackID(-1),   fParentID(-1),   fPos(G4ThreeVector(0,0,0)), fE(0.)
{}



NC2DummyHit::~NC2DummyHit() {}



NC2DummyHit::NC2DummyHit(const NC2DummyHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fParentID  = right.fParentID;
  fPos       = right.fPos;
  fE = right.fE;
}



const NC2DummyHit& NC2DummyHit::operator=(const NC2DummyHit& right)
{
  fTrackID   = right.fTrackID;
  fParentID   = right.fParentID;
  fPos       = right.fPos;
  fE = right.fE;

  return *this;
}



G4int NC2DummyHit::operator==(const NC2DummyHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}



void NC2DummyHit::Draw()
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

void NC2DummyHit::Print()
{
  G4cout << G4endl
     << "  trackID: " << fTrackID  
     << "  parentID: " << fParentID
     << "  Position: "  << std::setw(5) << G4BestUnit( fPos,"Length")
     << "  Energy: " << std::setw(5) << G4BestUnit (fE,"Energy")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
