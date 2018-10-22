#include <iostream>
#include <cmath> 
#include <algorithm>


#include "Level.hh"
#include "Randomize.hh"

using namespace std;

Level::Level(unsigned int n_val,unsigned int l_val,float j_val)
{
  n = n_val;
  l = l_val;
  j = j_val;
  name = "";
} 

Level::Level(unsigned int n_val,unsigned int l_val,float j_val, std::string name_value)
{
  n = n_val;
  l = l_val;
  j = j_val;
  name = name_value;
} 

Level::Level()
{
  n = 0;
  l = 0;
  j = 0;
  name = "";
}

Level::~Level()
{
  transitions.clear();
  levels.clear();
} 

void Level::SetTransition(Level* level, float p, G4double e)
{
  if( std::find(levels.begin(),levels.end(),level) == levels.end() )
  {
    transition_param_t param;
    param.strength = p;
    param.energy = e;
    transitions[level] = param;
    levels.push_back( level );
    G4cout << " level " << level->GetName() << " added to " << name << " with address " << std::hex << level << G4endl;
  }
  else
  {
    G4cout << "Level " << level->GetName() << "already added " << G4endl;
  }

 // for (auto const& element : input_map) {
 //   retval.push_back(element.first);
 // }

}

Level* Level::GetTransition(G4double* transition_energy)
{
  bool level_found = false;
  if(levels.size() < 1 )
  {
    G4cout << " no levels available !!! " << G4endl;
    return NULL;
  }
  
  Level* level_out;
  
  //G4cout << " At level " << name << G4endl;
  //G4cout << "Levels available: " << levels.size() << G4endl;
 /* for(const auto &x : levels)
  {
    //G4cout << levels.at(0)->GetName() << G4endl;
  }*/
  
  float total_strength = GetTotalStrength(); //sum of the entire transition strength of all daughter levels
  
  while(!level_found)
  {
    G4int trial_a = int(trunc(G4UniformRand()*levels.size()));
    G4double trial_b = G4UniformRand();    
    if( transitions[levels.at(trial_a)].strength > trial_b )
    {
      *transition_energy = transitions[levels.at(trial_a)].energy;
      level_out =  levels.at(trial_a); 
      level_found = true;
    }
  }
  
  //G4cout << " (Level::GetTransition) transition found, next level " << level_out->GetName() << G4endl;
  
  return level_out;
  
}

float Level::GetTotalStrength()
{
  float sum = 0.;
  for (const auto& x : transitions)
  {
    sum += x.second.strength;  
  }
  return sum;
}


