#include "PhysicsListMessenger.hh"

#include "PhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

//---------------------------------------------------------------------------

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
  :fPPhysicsList(pPhys)
{   
  fPhysDir = new G4UIdirectory("/simpleG4MC/physics/");
  fPhysDir->SetGuidance("physics control.");

  fPListCmd = new G4UIcmdWithAString("/simpleG4MC/physics/addPhysics",this);
  fPListCmd->SetGuidance("Add physics list (standard_opt3, QGSP_BIC_EMY, QGSP_BIS_HP, QGSP_BERT_HP, FTFP_BERT_HP");
}

//---------------------------------------------------------------------------

PhysicsListMessenger::~PhysicsListMessenger()
{;}

//---------------------------------------------------------------------------

void PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{
  if( command == fPListCmd )
   { fPPhysicsList->AddPhysicsList(newValue);}
}

//---------------------------------------------------------------------------

