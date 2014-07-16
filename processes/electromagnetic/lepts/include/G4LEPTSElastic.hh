#ifndef G4LEPTSElastic_h
#define G4LEPTSElastic_h 1

#include "G4VEmProcess.hh"

class G4LEPTSElastic : public G4VEmProcess
{ 
public:
  G4LEPTSElastic(const G4String& processName ="G4LEPTSElastic");
  ~G4LEPTSElastic();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};


#endif
