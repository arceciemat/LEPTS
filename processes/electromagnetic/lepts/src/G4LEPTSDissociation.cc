#include "G4LEPTSDissociation.hh"
#include "G4LEPTSDissociationModel.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSDissociation::G4LEPTSDissociation(const G4String& processName) 
  : G4VEmProcess( processName ),isInitialised(false)
{
  G4cout << "G4LEPTSDissociation constructor: " << processName << G4endl;
  //-  SetProcessName(processName);

} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSDissociation::~G4LEPTSDissociation() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool G4LEPTSDissociation::IsApplicable(const G4ParticleDefinition& particleDef) {
  G4cout << "G4LEPTSDissociation::IsApplicable" << G4endl;
  return( &particleDef == G4Electron::Electron() ||
	  &particleDef == G4Positron::Positron() );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSDissociation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel()) SetEmModel(new G4LEPTSDissociationModel);
    EmModel()->SetLowEnergyLimit(0.1*CLHEP::eV);
    EmModel()->SetHighEnergyLimit(15.*CLHEP::MeV);
    AddEmModel(1, EmModel());

  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4LEPTSDissociation::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << EmModel()->GetName() << " model"
    << G4endl;
}         

