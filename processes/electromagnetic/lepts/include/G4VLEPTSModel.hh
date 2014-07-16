#ifndef G4VLEPTSModel_hh
#define G4VLEPTSModel_hh

#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4VEmModel.hh"
#include "G4PhysicsTable.hh"
#include "G4PhysicsLogVector.hh"
#include "G4Element.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4Step.hh"

#include "G4EnergyLossTables.hh"
#include "G4UnitsTable.hh"
#include <iostream> 
using namespace std;


#include "G4LEPTSDiffXS.hh"
#include "G4LEPTSDistribution.hh"
#include "G4ForceCondition.hh"
#include <map>
#include <vector>

enum XSType {XSEnergy, 
	     XSTotal, XSElastic, XSInelastic,
	     XSIonisation, XSExcitation, XSDissociation,
	     XSVibration, XSAttachment, XSRotation, XSPositronium
};


class G4VLEPTSModel : public G4VEmModel
{

public: 
  
  G4VLEPTSModel(const G4String& processName);
  ~G4VLEPTSModel();
  
  void BuildPhysicsTable(const G4ParticleDefinition& aParticleType); //G4ParticleWithCuts::SetCut()
  G4double GetMeanFreePath(const G4Material* mate,
			   const G4ParticleDefinition* aParticle,
			   G4double kineticEnergy );
  
  G4ThreeVector SampleNewDirection(const G4Material* aMaterial, G4ThreeVector Dir, G4double e, G4double el);
  G4double SampleAngle(const G4Material* aMaterial, G4double e, G4double el);
  G4ThreeVector SampleNewDirection(G4ThreeVector Dir, G4double ang);
  
  G4VLEPTSModel& operator=(const G4VLEPTSModel &right); //hide assignment operator
  G4VLEPTSModel(const G4VLEPTSModel& );
  
protected:
  void Init();
  virtual std::map<G4int,std::vector<G4double> > ReadIXS(G4String fileName, const G4Material* aMaterial);
  G4double SampleEnergyLoss(const G4Material* aMaterial, G4double eMin, G4double eMax);
  void BuildMeanFreePathTable( const G4Material* aMaterial, std::map< G4int, std::vector<G4double> >& integralXS );
  
protected:
  G4PhysicsTable * theMeanFreePathTable;
  
  G4double theLowestEnergyLimit;
  G4double theHighestEnergyLimit;
  G4int    theNumbBinTable;
  
  std::map<const G4Material*, G4double > theIonisPot;
  std::map<const G4Material*, G4double > theIonisPotInt;
  std::map<const G4Material*, G4double > theEnergySeparator;
  std::map<const G4Material*, G4double > theMolecularMass;

  std::map<const G4Material*, G4LEPTSDiffXS*> theDiffXS;
  std::map<const G4Material*, G4LEPTSDistribution*> theRMTDistr;

  std::map<const G4Material*, G4LEPTSDistribution*> theElostDistr;
  std::map<const G4Material*, G4LEPTSDistribution*> theElostDistr2;

  std::map<const G4Material*, G4int> theNXSdat;
  std::map<const G4Material*, G4int> theNXSsub;
  
  G4bool isInitialised;
  XSType theXSType;

  G4int verboseLevel;
};


#endif // G4VLEPTSModel_hh
