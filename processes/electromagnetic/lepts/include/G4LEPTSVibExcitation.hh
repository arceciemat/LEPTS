#ifndef G4LEPTSVibExcitation_h
#define G4LEPTSVibExcitation_h 1

#include "G4VEmProcess.hh"

class G4LEPTSVibExcitation : public G4VEmProcess
{ 
public:
  G4LEPTSVibExcitation(const G4String& processName ="G4LEPTSVibExcitation");
  ~G4LEPTSVibExcitation();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};


#endif
