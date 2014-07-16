#ifndef G4LEPTSAttachment_h
#define G4LEPTSAttachment_h 1

#include "G4VEmProcess.hh"

class G4LEPTSAttachment : public G4VEmProcess
{ 
public:
  G4LEPTSAttachment(const G4String& processName ="G4LEPTSAttachment");
  ~G4LEPTSAttachment();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};


#endif
