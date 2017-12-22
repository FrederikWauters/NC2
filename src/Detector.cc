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
  
  double angle = atan( position.getX() / position.getZ() ) ;
  if ( position.getZ() > 0 ) yRot = -angle;
    else yRot = - angle  + pi; 
    
  angle = asin( position.getY() / position.r() );
  if ( position.getZ() > 0 ) xRot = angle;
    else xRot = angle + pi ;
    
    
} 

