#include <iostream>
#include <cmath> 

#include "G4PhysicalConstants.hh"
#include "Detector.hh"

using namespace std;

Detector::Detector()
{
  
} 

Detector::~Detector()
{
  
} 

Detector::Detector(G4LogicalVolume *vol,G4ThreeVector p, G4ThreeVector o)
{
  world = vol;
  position = p - o;
  
  double angle;
  
  //solving some border cases
  if( fabs(position.getX()) < 0.000001 ) angle = 0.;
  else if( fabs(position.getZ()) < 0.000001 ) angle  = atan( position.getX() / 0.000001 ) ;
  else angle  = atan( position.getX() / position.getZ() );
  
  if ( position.getZ() >= 0 ) yRot = -angle;
    else yRot = - angle  + pi;
    
  G4cout << "yRot " << yRot << G4endl; 
  
  if( fabs(position.r()) < 0.000001 ) angle = asin( position.getY() / 0.000001 );
  else angle = asin( position.getY() / position.r() );
  
  if ( position.getZ() >= 0 ) xRot = angle;
    else xRot = angle + pi ;
    
  G4cout << "xRot " << xRot << G4endl;
} 

