#ifndef __PHYSICS__
#define __PHYSICS__

#include "G4VUserPhysicsList.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
// Geant461: #include "g4std/iomanip"
#include "G4Decay.hh"
#include "G4UserSpecialCuts.hh"
#include "G4StepLimiter.hh"
#include "globals.hh"




class LeptsPhysicsList : public G4VUserPhysicsList{
 public:
  LeptsPhysicsList():G4VUserPhysicsList(){defaultCutValue=2*mm;};
  virtual ~LeptsPhysicsList(){};

  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void SetCuts();


 protected:
  // Physics
  virtual void ConstructGeneral();
  virtual void ConstructEMGamma();
  virtual void ConstructEMep();
};


#endif
