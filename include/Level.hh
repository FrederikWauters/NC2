#ifndef LEVEL_H_
#define LEVEL_H_

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"
#include <map>

class Level 
{
  public:
  
    Level();
    Level(unsigned int n_val,unsigned int l_val,float j_val);
    Level(unsigned int n_val,unsigned int l_val,float j_val, std::string name_value);
    
    ~Level();
    
    void SetEnergy(G4double value) {E = value;};
    G4double  GetEnergy() { return E; };
    std::string GetName() {  return name; };
    
    void SetTransition(Level* level, float p); //gets pointer of original levels defined
    std::vector<Level*>* GetDaughterLevels() { return &levels; }
    float GetTotalStrength(); // adding all transition strengths
    
    unsigned int Get_n() { return n; }
    unsigned int Get_l() { return l; }
    float Get_j() { return j; }
    
    Level* GetTransition(G4double* e); //returns next level
    
        
  private:
  
    std::string name;
  
    unsigned int n;
    unsigned int l;
    float j;
    
    G4double E; //energy of level 
    
    std::map<Level*,float> transitions; //radiative transitions with transition probabilities
    std::vector<Level*> levels; //handy to keep track of the levels available
};

#endif
