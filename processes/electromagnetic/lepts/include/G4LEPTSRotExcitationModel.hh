#ifndef G4LEPTSRotExcitationModel_h
#define G4LEPTSRotExcitationModel_h
#include "G4VLEPTSModel.hh"
#include "G4ParticleChangeForGamma.hh"

class G4LEPTSRotExcitationModel : public G4VLEPTSModel 
{ 
public:
  G4LEPTSRotExcitationModel(const G4String& modelName ="G4LEPTSRotExcitationModel");
  ~G4LEPTSRotExcitationModel();

  virtual void Initialise(const G4ParticleDefinition*, 
                          const G4DataVector&);

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);

 // main method to compute cross section per Volume
  virtual G4double CrossSectionPerVolume(const G4Material*,
                                         const G4ParticleDefinition*,
                                         G4double kineticEnergy,
                                         G4double cutEnergy = 0.0,
                                         G4double maxEnergy = DBL_MAX);

private:
  G4ParticleChangeForGamma* fParticleChangeForGamma;
};

#endif
