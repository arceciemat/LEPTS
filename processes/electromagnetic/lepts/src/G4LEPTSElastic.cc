#include "G4LEPTSElastic.hh"
#include "G4LEPTSElasticModel.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSElastic::G4LEPTSElastic(const G4String& processName) 
  : G4VEmProcess( processName ),isInitialised(false)
{
  G4cout << "G4LEPTSElastic constructor: " << processName << G4endl;
  //-  SetProcessName(processName);

} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSElastic::~G4LEPTSElastic() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool G4LEPTSElastic::IsApplicable(const G4ParticleDefinition& particleDef) {
  G4cout << "G4LEPTSElastic::IsApplicable" << G4endl;
  return( &particleDef == G4Electron::Electron() ||
	  &particleDef == G4Positron::Positron() );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSElastic::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel()) SetEmModel(new G4LEPTSElasticModel);
    EmModel()->SetLowEnergyLimit(0.1*CLHEP::eV);
    EmModel()->SetHighEnergyLimit(15.*CLHEP::MeV);
    AddEmModel(1, EmModel());

  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4LEPTSElastic::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << EmModel()->GetName() << " model"
    << G4endl;
}         

