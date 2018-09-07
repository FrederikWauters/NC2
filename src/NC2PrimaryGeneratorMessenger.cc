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


  // energy gamma line command
  /*energyCmd = new G4UIcmdWithADoubleAndUnit("/generator/energy",this);
  energyCmd->SetGuidance("Set energy of gamma line");
  energyCmd->SetParameterName("Energy",false);
  energyCmd->SetRange("Energy>=0.");
  energyCmd->SetDefaultValue((G4double)0.*keV);
  energyCmd->AvailableForStates(G4State_Idle,G4State_Idle);
  
  // intensity of gamma line
  intensityCmd = new G4UIcmdWithADouble("/generator/intensity",this);
  intensityCmd->SetGuidance("Set intensity of gamma line");
  intensityCmd->SetGuidance("This command MUST be applied after /generator/energy");
  intensityCmd->SetParameterName("Intensity",false);
  intensityCmd->SetRange("Intensity>=0.");
  intensityCmd->SetDefaultValue((G4double)0.);
  intensityCmd->AvailableForStates(G4State_Idle,G4State_Idle);
  
  useCmd = new G4UIcmdWithABool("/generator/use",this);
  useCmd->SetGuidance("Use this line");
  useCmd->SetGuidance("  Choice : true(use), false(not use)");
  useCmd->SetGuidance("This command MUST be applied after /generator/energy");
  useCmd->SetParameterName("Choice",true);
  useCmd->SetDefaultValue((G4bool)true);
  useCmd->AvailableForStates(G4State_PreInit,G4State_Idle);*/
  

  
  printCmd = new G4UIcmdWithoutParameter("/generator/print",this);
  printCmd->SetGuidance("Print level's.");
  printCmd->AvailableForStates(G4State_Idle);
  
}

NC2PrimaryGeneratorMessenger::~NC2PrimaryGeneratorMessenger()
{
  /*delete energyCmd;
  delete intensityCmd;
  delete useCmd;*/
  delete printCmd;
}

void NC2PrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  /*if( command == energyCmd )
    { generator->SetNewEnergy(energyCmd->GetNewDoubleValue(newValue));}

  if( command == intensityCmd )
    { generator->SetNewIntensity(intensityCmd->GetNewDoubleValue(newValue));}

  if( command == useCmd )
    { generator->SetNewUse(useCmd->GetNewBoolValue(newValue));}*/
    
  if( command == printCmd )
    { generator->PrintLevels();}
}





