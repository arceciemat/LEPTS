#include "G4LEPTSAttachment.hh"
#include "G4LEPTSAttachmentModel.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSAttachment::G4LEPTSAttachment(const G4String& processName) 
  : G4VEmProcess( processName ),isInitialised(false)
{
  G4cout << "G4LEPTSAttachment constructor: " << processName << G4endl;
  //-  SetProcessName(processName);

} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSAttachment::~G4LEPTSAttachment() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool G4LEPTSAttachment::IsApplicable(const G4ParticleDefinition& particleDef) {
  G4cout << "G4LEPTSAttachment::IsApplicable" << G4endl;
  return( &particleDef == G4Electron::Electron() );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSAttachment::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel()) SetEmModel(new G4LEPTSAttachmentModel);
    EmModel()->SetLowEnergyLimit(0.1*CLHEP::eV);
    EmModel()->SetHighEnergyLimit(15.*CLHEP::MeV);
    AddEmModel(1, EmModel());

  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4LEPTSAttachment::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << EmModel()->GetName() << " model"
    << G4endl;
}         

