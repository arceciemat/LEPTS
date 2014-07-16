#include "G4LEPTSElectronicExcitationModel.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSElectronicExcitationModel::G4LEPTSElectronicExcitationModel(const G4String& modelName) 
  : G4VLEPTSModel( modelName )
{
  G4cout << "G4LEPTSElectronicExcitationModel constructor" << modelName << G4endl;
  theXSType = XSExcitation; 
} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSElectronicExcitationModel::~G4LEPTSElectronicExcitationModel() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSElectronicExcitationModel::Initialise(const G4ParticleDefinition* aParticle, 
                          const G4DataVector&)
{
  Init();
  BuildPhysicsTable( *aParticle );

  fParticleChangeForGamma = GetParticleChangeForGamma();

  LowestExcitationEnergy = 0;
  LowestNeutralDisociationEnergy = 0;


}


std::map<G4int,std::vector<G4double> > G4LEPTSElectronicExcitationModel::ReadIXS(G4String fileTXS, const G4Material* aMaterial)
{
  std::map<G4int,std::vector<G4double> > integralXS = G4VLEPTSModel::ReadIXS( fileTXS, aMaterial);

  if( integralXS.size() == 0 ) return integralXS;

  for (G4int jj=theNXSdat[aMaterial]; jj>=0; jj--) {
    if( integralXS[XSDissociation][jj] > 0.001) LowestExcitationEnergy  = integralXS[XSTotal][jj-1];
    if( integralXS[XSVibration][jj] > 0.001) LowestNeutralDisociationEnergy = integralXS[XSTotal][jj-1]*CLHEP::eV;
    //    if( txs[5][j] > 0.001) LowestExcitationEnergy         = txs[0][j-1];
    //  if( txs[6][j] > 0.001) LowestNeutralDisociationEnergy = txs[0][j-1]*CLHEP::eV;  
  }

  G4cout << "        LowestExcitationEnergy: " << LowestExcitationEnergy << G4endl
	 << "LowestNeutralDisociationEnergy: " << LowestNeutralDisociationEnergy/CLHEP::eV
	 << G4endl;
  
  return integralXS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4LEPTSElectronicExcitationModel::CrossSectionPerVolume(const G4Material* mate,
                                         const G4ParticleDefinition* aParticle,
                                         G4double kineticEnergy,
                                         G4double,
                                         G4double)
{
  return 1./GetMeanFreePath( mate, aParticle, kineticEnergy );

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSElectronicExcitationModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle,
                                 G4double,
                                 G4double)
{
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  G4double Edep=0;
  G4double Energylost=0;
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

  G4double eMin = 0.0;
  const G4Material* aMaterial = mateCuts->GetMaterial();
  G4double eMax = min(theIonisPot[aMaterial], P0KinEn);
  Energylost = SampleEnergyLoss(aMaterial, eMin, eMax);

  Edep = Energylost;

  G4ThreeVector P1Dir = SampleNewDirection(aMaterial, P0Dir, P0KinEn/CLHEP::eV, Energylost/CLHEP::eV);
  G4double P1KinEn = P0KinEn - Edep;

  fParticleChangeForGamma->ProposeMomentumDirection( P1Dir);
  fParticleChangeForGamma->SetProposedKineticEnergy( P1KinEn);
  fParticleChangeForGamma->ProposeLocalEnergyDeposit( Edep);

}
