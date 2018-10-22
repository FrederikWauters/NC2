#include "NC2PrimaryGeneratorMessenger.hh"
#include "NC2PrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"

NC2PrimaryGeneratorMessenger::NC2PrimaryGeneratorMessenger(NC2PrimaryGeneratorAction * gen) : generator(gen)

{
  gunDirectory = new G4UIdirectory("/generator/");
  gunDirectory->SetGuidance("particle gun control");

  // intensity of gamma line
  startLevelCmd = new G4UIcmdWithAnInteger("/generator/startlevel",this);
  startLevelCmd->SetGuidance("Set initial n of the cascade");
  startLevelCmd->SetParameterName("InitialN",false);
  startLevelCmd->SetRange("InitialN>=1");
  startLevelCmd->SetDefaultValue((G4int)6);
  startLevelCmd->AvailableForStates(G4State_Idle,G4State_Idle);
  
 
  
  decayCmd = new G4UIcmdWithABool("/generator/decay",this);
  decayCmd->SetGuidance("Use this line");
  decayCmd->SetGuidance("  Choice : true(use), false(not use)");
  decayCmd->SetParameterName("Choice",true);
  decayCmd->SetDefaultValue((G4bool)false);
  decayCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  

  
  printCmd = new G4UIcmdWithoutParameter("/generator/print",this);
  printCmd->SetGuidance("Print level's.");
  printCmd->AvailableForStates(G4State_Idle);
  
}

NC2PrimaryGeneratorMessenger::~NC2PrimaryGeneratorMessenger()
{
  delete printCmd;
  delete decayCmd;
  delete startLevelCmd;
}

void NC2PrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{

  if(command == startLevelCmd)  { generator->SetInitialN(startLevelCmd->GetNewIntValue(newValue)); }
  
  if(command == decayCmd)  { generator->SetDecayFlag(decayCmd->GetNewBoolValue(newValue)); }
    
  if( command == printCmd )
    { generator->PrintLevels();}
}





