#ifndef G4LEPTSElectronicExcitation_h
#define G4LEPTSElectronicExcitation_h 1

#include "G4VEmProcess.hh"

class G4LEPTSElectronicExcitation : public G4VEmProcess
{ 
public:
  G4LEPTSElectronicExcitation(const G4String& processName ="G4LEPTSElectronicExcitation");
  ~G4LEPTSElectronicExcitation();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};


#endif
