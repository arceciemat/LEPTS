#ifndef G4LEPTSRotExcitation_h
#define G4LEPTSRotExcitation_h 1

#include "G4VEmProcess.hh"

class G4LEPTSRotExcitation : public G4VEmProcess
{ 
public:
  G4LEPTSRotExcitation(const G4String& processName ="G4LEPTSRotExcitation");
  ~G4LEPTSRotExcitation();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};


#endif
