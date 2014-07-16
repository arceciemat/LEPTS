#ifndef G4LEPTSIonisation_h
#define G4LEPTSIonisation_h

#include "G4VEmProcess.hh"

class G4LEPTSIonisation : public G4VEmProcess { 
public:
  G4LEPTSIonisation(const G4String& processName ="G4LEPTSIonisation");
  ~G4LEPTSIonisation();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
 
private:
  G4bool       isInitialised;

};

#endif
