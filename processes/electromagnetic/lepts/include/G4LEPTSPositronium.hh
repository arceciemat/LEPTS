#ifndef G4LEPTSPositronium_h
#define G4LEPTSPositronium_h 1

#include "G4VEmProcess.hh"

class G4LEPTSPositronium : public G4VEmProcess
{ 
public:
  G4LEPTSPositronium(const G4String& processName ="G4LEPTSPositronium");
  ~G4LEPTSPositronium();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};


#endif
