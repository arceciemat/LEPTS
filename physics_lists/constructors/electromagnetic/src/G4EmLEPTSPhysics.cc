
#include "G4EmLEPTSPhysics.hh"

#include "G4PenelopeRayleighModel.hh"
#include "G4PenelopeComptonModel.hh"
#include "G4PenelopePhotoElectricModel.hh"
#include "G4PenelopeGammaConversionModel.hh"

#include "G4LivermoreRayleighModel.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4GammaConversion.hh"
#include "G4RayleighScattering.hh"
#include "G4ComptonScattering.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4OpRayleigh.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"

#include "G4LEPTSElastic.hh"
#include "G4LEPTSIonisation.hh"
#include "G4LEPTSElectronicExcitation.hh"
#include "G4LEPTSDissociation.hh"
#include "G4LEPTSVibExcitation.hh"
#include "G4LEPTSRotExcitation.hh"
#include "G4LEPTSAttachment.hh"
#include "G4LEPTSPositronium.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4EmLEPTSPhysics::G4EmLEPTSPhysics( const G4String& name)
 : G4VPhysicsConstructor(name)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4EmLEPTSPhysics::ConstructParticle()
{
  G4Gamma::GammaDefinition();
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
}

//....oooOO0OOooo.......oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4EmLEPTSPhysics::ConstructProcess()
{
  ConstructEMep();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4EmLEPTSPhysics::ConstructEMep()
{
  aParticleIterator->reset();

  while( (*aParticleIterator)() ){
    G4ParticleDefinition * particle = aParticleIterator->value();
    G4String               pName = particle->GetParticleName();
    G4ProcessManager     * manager = particle->GetProcessManager();
    
    if(pName=="e-") {
      manager->AddDiscreteProcess(new G4LEPTSElastic("e-_G4LEPTSElastic") );
      manager->AddDiscreteProcess(new G4LEPTSIonisation("e-_G4LEPTSIonisation") );
      manager->AddDiscreteProcess(new G4LEPTSElectronicExcitation("e-_G4LEPTSExcitElectr") );
      manager->AddDiscreteProcess(new G4LEPTSDissociation("e-_G4LEPTSDissocNeutr") );
      manager->AddDiscreteProcess(new G4LEPTSVibExcitation("e-_G4LEPTSExcitVibrat") );
      manager->AddDiscreteProcess(new G4LEPTSRotExcitation("e-_G4LEPTSExcitRotat") );

      manager->AddDiscreteProcess(new G4LEPTSAttachment("e-_G4LEPTSAttachment") );
    }
    else if(pName=="e+") {
      manager->AddDiscreteProcess(new G4LEPTSElastic("e+_G4LEPTSElastic") );
      manager->AddDiscreteProcess(new G4LEPTSIonisation("e+_G4LEPTSIonisation") );
      manager->AddDiscreteProcess(new G4LEPTSElectronicExcitation("e+_G4LEPTSExcitElectr") );
      manager->AddDiscreteProcess(new G4LEPTSDissociation("e+_G4LEPTSDissocNeutr") );
      manager->AddDiscreteProcess(new G4LEPTSVibExcitation("e+_G4LEPTSExcitVibrat") );
      manager->AddDiscreteProcess(new G4LEPTSRotExcitation("e+_G4LEPTSExcitRotat") );

      manager->AddDiscreteProcess(new G4LEPTSPositronium("e+_G4LEPTSPositronium") );
    }
    
    manager->DumpInfo();
  }
}

