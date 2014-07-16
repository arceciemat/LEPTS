#include "G4LEPTSPositronium.hh"
#include "G4LEPTSPositroniumModel.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSPositronium::G4LEPTSPositronium(const G4String& processName) 
  : G4VEmProcess( processName ),isInitialised(false)
{
  G4cout << "G4LEPTSPositronium constructor: " << processName << G4endl;
  //-  SetProcessName(processName);

} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSPositronium::~G4LEPTSPositronium() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool G4LEPTSPositronium::IsApplicable(const G4ParticleDefinition& particleDef) {
  G4cout << "G4LEPTSPositronium::IsApplicable" << G4endl;
  return( &particleDef == G4Positron::Positron() );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSPositronium::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel()) SetEmModel(new G4LEPTSPositroniumModel);
    EmModel()->SetLowEnergyLimit(0.1*CLHEP::eV);
    EmModel()->SetHighEnergyLimit(15.*CLHEP::MeV);
    AddEmModel(1, EmModel());

  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4LEPTSPositronium::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << EmModel()->GetName() << " model"
    << G4endl;
}         

