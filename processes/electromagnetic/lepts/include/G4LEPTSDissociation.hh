#ifndef G4LEPTSDissociation_h
#define G4LEPTSDissociation_h 1

#include "G4VEmProcess.hh"

class G4LEPTSDissociation : public G4VEmProcess
{ 
public:
  G4LEPTSDissociation(const G4String& processName ="G4LEPTSDissociation");
  ~G4LEPTSDissociation();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};


#endif
