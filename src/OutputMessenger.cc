#include "OutputMessenger.hh"
#include "OutputManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "globals.hh"

//---------------------------------------------------------------------------

OutputMessenger::OutputMessenger(OutputManager* outMana)
:fOutputManager(outMana)
{
  fOutputDir = new G4UIdirectory("/simpleG4MC/output/");
  fOutputDir->SetGuidance("Output control");

  fOutFileCmd = new G4UIcmdWithAString("/simpleG4MC/output/setOutputFile",this);
  fOutFileCmd->SetGuidance("Set the full name and path of the output file");
} 

//---------------------------------------------------------------------------

OutputMessenger::~OutputMessenger()
{;}

//---------------------------------------------------------------------------

void OutputMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if(command == fOutFileCmd)
    {fOutputManager->SetOutFileName(newValue.data());}
}

//---------------------------------------------------------------------------
