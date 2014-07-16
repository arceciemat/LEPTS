#include "G4LEPTSIonisation.hh"
#include "G4LEPTSIonisationModel.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// constructor
//G4LEPTSIonisation::G4LEPTSIonisation(const G4String& processName) : vLeptsProcess(processName) {
G4LEPTSIonisation::G4LEPTSIonisation(const G4String& processName) 
  : G4VEmProcess( processName ),isInitialised(false)
{
  G4cout << "G4LEPTSIonisation constructor: " << processName << G4endl;

} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSIonisation::~G4LEPTSIonisation() {
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool G4LEPTSIonisation::IsApplicable(const G4ParticleDefinition& particleDef) {
  G4cout << "G4LEPTSIonisation::IsApplicable" << G4endl;
  return( &particleDef == G4Electron::Electron() ||
	  &particleDef == G4Positron::Positron() );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSIonisation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel()) SetEmModel(new G4LEPTSIonisationModel);
    EmModel()->SetLowEnergyLimit(0.1*CLHEP::eV);
    EmModel()->SetHighEnergyLimit(15.*CLHEP::MeV);
    AddEmModel(1, EmModel());

  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4LEPTSIonisation::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << EmModel()->GetName() << " model"
    << G4endl;
}         

