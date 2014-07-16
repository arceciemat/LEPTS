#include "G4VLEPTSModel.hh"

#include "CLHEP/Units/SystemOfUnits.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VLEPTSModel::G4VLEPTSModel(const G4String& modelName) : G4VEmModel(modelName),isInitialised(false) 
{
  G4cout << "baseclass G4VLEPTSModel::constructor" << G4endl;
  theMeanFreePathTable=NULL;

  theNumbBinTable=100;

  verboseLevel = 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VLEPTSModel::~G4VLEPTSModel() 
{

  if(theMeanFreePathTable) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4VLEPTSModel::Init() 
{
  theLowestEnergyLimit = 0.1*CLHEP::eV;
  theHighestEnergyLimit = 10.0*CLHEP::keV;
  //t  theHighestEnergyLimit = 15.0*CLHEP::MeV;
  SetLowEnergyLimit(theLowestEnergyLimit);
  SetHighEnergyLimit(theHighestEnergyLimit);
  theNumbBinTable = 100;

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double G4VLEPTSModel::GetMeanFreePath(const G4Material* aMaterial,
			       const G4ParticleDefinition* ,
			       G4double kineticEnergy )
{

  G4double MeanFreePath;
  G4bool isOutRange ;

  if( verboseLevel >= 3 ) G4cout << aMaterial->GetIndex() << " G4VLEPTSModel::GetMeanFreePath " << kineticEnergy << " > " << theHighestEnergyLimit << " < " << theLowestEnergyLimit << G4endl;
  if (kineticEnergy > theHighestEnergyLimit || kineticEnergy < theLowestEnergyLimit)
    MeanFreePath = DBL_MAX;
  else
    MeanFreePath = (*theMeanFreePathTable)(aMaterial->GetIndex())->
                                       GetValue(kineticEnergy, isOutRange);

  return MeanFreePath;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4VLEPTSModel::BuildPhysicsTable(const G4ParticleDefinition& aParticleType) 
{
  //CHECK IF PATH VARIABLE IS DEFINED
  char* path = getenv("G4LEPTSDATA");
  if( !path ) {
    G4Exception("G4VLEPTSModel",
		"",
		FatalException,
		"variable G4LEPTSDATA not defined");
  }

  // Build microscopic cross section table and mean free path table
   
  G4String aParticleName = aParticleType.GetParticleName();

  if (theMeanFreePathTable) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }
  
  theMeanFreePathTable = new G4PhysicsTable(G4Material::GetNumberOfMaterials());

  //LOOP TO MATERIALS IN GEOMETRY 
  const G4MaterialTable * materialTable = G4Material::GetMaterialTable() ;
  std::vector<G4Material*>::const_iterator matite;
  for( matite = materialTable->begin(); matite != materialTable->end(); matite++ ) {
    const G4Material * aMaterial = (*matite);
    G4String mateName = aMaterial->GetName();

    //READ INTEGRAL CROSS SECTION FOR THIS MATERIAL
    string baseName = string(path) + "/" + mateName + "." + aParticleName;
    string fnIXS  = baseName + ".IXS.dat";
    
    std::map< G4int, std::vector<G4double> > integralXS = ReadIXS(fnIXS, aMaterial);
    if( verboseLevel >= -2 ) G4cout << GetName() << " : " << theXSType << " " << mateName << " INTEGRALXS " << integralXS.size() << G4endl;

    if( integralXS.size() == 0 ) {
      G4cerr << " Integral cross sections will be set to 0. for material " << mateName << G4endl;
      G4PhysicsLogVector* ptrVector = new G4PhysicsLogVector(theLowestEnergyLimit, theHighestEnergyLimit, 2);
      ptrVector->PutValue(0, DBL_MAX);
      ptrVector->PutValue(1, DBL_MAX);

      unsigned int matIdx = aMaterial->GetIndex();
      theMeanFreePathTable->insertAt( matIdx , ptrVector ) ;

    } else {
    
      if( verboseLevel >= 2 ) {
	std::map< G4int, std::vector<G4double> >::const_iterator itei;
	for( itei = integralXS.begin(); itei != integralXS.end(); itei++ ){
	  G4cout << GetName() << " : " << (*itei).first << " INTEGRALXS NDATA " << (*itei).second.size() << G4endl;
	}
      }
      
      BuildMeanFreePathTable( aMaterial, integralXS );

      string fnDXS = baseName + ".DXS.dat";
      string fnRMT = baseName + ".RMT.dat";
      string fnEloss = baseName + ".Eloss.dat";
      string fnEloss2 = baseName + ".Eloss2.dat";
      
      theDiffXS[aMaterial] = new G4LEPTSDiffXS(fnDXS);
      if( !theDiffXS[aMaterial]->IsFileFound() ) {
	G4Exception("G4VLEPTSModel::BuildPhysicsTable",
		    "",
		    FatalException,
		    (G4String("File not found :" + fnDXS).c_str()));
      }

      theRMTDistr[aMaterial] = new G4LEPTSDistribution(fnRMT);

      theElostDistr[aMaterial] = new G4LEPTSDistribution(fnEloss);
      if( !theElostDistr[aMaterial]->IsFileFound() ) {
	G4Exception("G4VLEPTSModel::BuildPhysicsTable",
		    "",
		    FatalException,
		    (G4String("File not found :" + fnEloss).c_str()));
      }
      theElostDistr2[aMaterial] = new G4LEPTSDistribution(fnEloss2);
    }

  }
  
}

void G4VLEPTSModel::BuildMeanFreePathTable( const G4Material* aMaterial, std::map< G4int, std::vector<G4double> >& integralXS )
{
  G4double LowEdgeEnergy, fValue;

  //BUILD MEAN FREE PATH TABLE FROM INTEGRAL CROSS SECTION
  unsigned int matIdx = aMaterial->GetIndex();
  G4PhysicsLogVector* ptrVector = new G4PhysicsLogVector(theLowestEnergyLimit, theHighestEnergyLimit, theNumbBinTable);
  
  for (G4int ii=0; ii < theNumbBinTable; ii++) {
    LowEdgeEnergy = ptrVector->GetLowEdgeEnergy(ii);
    if( verboseLevel >= 2 ) G4cout << GetName() << " " << ii << " LowEdgeEnergy " << LowEdgeEnergy << " > " << theLowestEnergyLimit << " < " << theHighestEnergyLimit << G4endl;
    //-      fValue = ComputeMFP(LowEdgeEnergy, material, aParticleName);
    fValue = 0.;
    if( LowEdgeEnergy >= theLowestEnergyLimit && 
	LowEdgeEnergy <= theHighestEnergyLimit) { 
      const G4double* NbOfAtomsPerVolume = aMaterial->GetVecNbOfAtomsPerVolume();
      
      G4double SIGMA = 0. ;
      for ( size_t elm=0 ; elm < aMaterial->GetNumberOfElements() ; elm++ ) {
	G4double crossSection = 0.;
	
	G4double eVEnergy = LowEdgeEnergy/CLHEP::eV;
	
	//-	   if( verboseLevel >= 2 )  G4cout << " eVEnergy " << eVEnergy << " LowEdgeE " << LowEdgeEnergy << " " << integralXS[XSEnergy][1] << G4endl;
	
	if(eVEnergy < integralXS[XSEnergy][1] ) {
	  crossSection = 0.;
	} else {
	  G4int Bin = 0; // locate bin                                                                           
	  G4double aa, bb;
	  for( G4int jj=1; jj<theNXSdat[aMaterial]; jj++) {  // Extrapolate for E > Emax !!!                               
	    if( verboseLevel >= 3 ) G4cout << " GET BIN " << jj << " "<< eVEnergy << " > " << integralXS[XSEnergy][jj] << G4endl; 
	    if( eVEnergy > integralXS[XSEnergy][jj]) {
	      Bin = jj;
	    } else {
	      break;
	    }
	  }
	  aa = integralXS[XSEnergy][Bin];
	  bb = integralXS[XSEnergy][Bin+1];
	  
	  crossSection = (integralXS[theXSType][Bin] + (integralXS[theXSType][Bin+1]-integralXS[theXSType][Bin])/(bb-aa)*(eVEnergy-aa) ) * 1.e-16*CLHEP::cm2;
	  
	  if( verboseLevel >= 3 ) G4cout << " crossSection " <<  crossSection << " " <<integralXS[theXSType][Bin] << " + " << (integralXS[theXSType][Bin+1]-integralXS[theXSType][Bin]) << " / " << (bb-aa) << " *" << (eVEnergy-aa) << " * " << 1.e-16*CLHEP::cm2 << G4endl;;
	  
	  SIGMA += NbOfAtomsPerVolume[elm] * crossSection;
	  if( verboseLevel >= 2 ) G4cout << GetName() << " " << elm << " ADDING SIGMA " << SIGMA << " NAtoms " << NbOfAtomsPerVolume[elm] 
					 << " Bin " << Bin << " TOTAL " << aa << " " << bb 
					 << " XS " << integralXS[theXSType][Bin]  << " " << integralXS[theXSType][Bin+1] << G4endl;
	}
	
      }
      
      fValue = SIGMA > DBL_MIN ? 1./SIGMA : DBL_MAX;
    }
    
    ptrVector->PutValue(ii, fValue);
    if( verboseLevel >= 2 ) G4cout << GetName() << " BUILDXS " << ii << " : " << LowEdgeEnergy << " = " << fValue << G4endl;
  }
  
  theMeanFreePathTable->insertAt( matIdx , ptrVector ) ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double G4VLEPTSModel::SampleAngle(const G4Material* aMaterial, G4double e, G4double el) 
{
  G4double x;

  if( e < 10001) {
    x = theDiffXS[aMaterial]->SampleAngleMT(e, el);
  }
  else {
    G4double Ei = e;                                       //incidente
    G4double Ed = e -el;                                   //dispersado
      
    G4double Pi = sqrt( pow( (Ei/27.2/137),2) +2*Ei/27.2); //incidente
    G4double Pd = sqrt( pow( (Ed/27.2/137),2) +2*Ed/27.2); //dispersado

    G4double Kmin = Pi - Pd;
    G4double Kmax = Pi + Pd;

    G4double KR = theRMTDistr[aMaterial]->Sample(Kmin, Kmax);          //sorteo mom. transf.
      
    G4double co = (Pi*Pi + Pd*Pd - KR*KR) / (2*Pi*Pd);    //cos ang. disp.
    if( co > 1. ) co = 1.;
    x = acos(co); //*360/twopi;                         //ang. dispers.
  }
  return(x);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ThreeVector G4VLEPTSModel::SampleNewDirection(const G4Material* aMaterial, G4ThreeVector P0Dir, G4double e, G4double el) {
  
  G4double x = SampleAngle(aMaterial, e, el);

  G4double cosTeta = cos(x); //*twopi/360.0);
  G4double sinTeta = sqrt(1.0-cosTeta*cosTeta);
  G4double Phi     = CLHEP::twopi * G4UniformRand() ;
  G4double dirx    = sinTeta*cos(Phi) , diry = sinTeta*sin(Phi) , dirz = cosTeta ;

  G4ThreeVector P1Dir(dirx, diry, dirz);
#ifdef DEBUG_LEPTS
  if( verboseLevel >= 2 ) G4cout << " G4VLEPTSModel::SampleNewDirection " <<P1Dir << G4endl; 
#endif
  P1Dir.rotateUz(P0Dir);
#ifdef DEBUG_LEPTS
  if( verboseLevel >= 2 ) G4cout << " G4VLEPTSModel::SampleNewDirection rotated " <<P1Dir << " " << P0Dir <<  G4endl; 
#endif

  return(P1Dir);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ThreeVector G4VLEPTSModel::SampleNewDirection(G4ThreeVector P0Dir, G4double x) 
{
  G4double cosTeta = cos(x); //*twopi/360.0);
  G4double sinTeta = sqrt(1.0-cosTeta*cosTeta);
  G4double Phi     = CLHEP::twopi * G4UniformRand() ;
  G4double dirx    = sinTeta*cos(Phi) , diry = sinTeta*sin(Phi) , dirz = cosTeta ;

  G4ThreeVector P1Dir( dirx,diry,dirz );
  P1Dir.rotateUz(P0Dir);

  return(P1Dir);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double G4VLEPTSModel::SampleEnergyLoss(const G4Material* aMaterial, G4double eMin, G4double eMax) 
{
  G4double el;
  if(eMax < theEnergySeparator[aMaterial])
    el = theElostDistr[aMaterial]->Sample(eMin/CLHEP::eV, eMax/CLHEP::eV)*CLHEP::eV;
  else {
    el = theElostDistr2[aMaterial]->Sample(eMin/CLHEP::eV, eMax/CLHEP::eV)*CLHEP::eV;
  }

#ifdef DEBUG_LEPTS
  if( verboseLevel >= 2 ) G4cout << "SampleEnergyLoss/eV " << el/CLHEP::eV << " theEnergySeparator/eV  " << theEnergySeparator[aMaterial]/CLHEP::eV << " eMax/eV " << eMax/CLHEP::eV << " "
	 << " " << GetName() << G4endl;
#endif
  return el;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::map< G4int, std::vector<G4double> > G4VLEPTSModel::ReadIXS(G4String fnIXS, const G4Material* aMaterial ) 
{
  std::map< G4int, std::vector<G4double> > integralXS; // process type - energy
  //G4cout << "fnIXS (" << fnIXS << ")" << G4endl;

  float fcu = 1; // factor conv. units: 1 -> 1e-16*cm2
  std::ifstream fin(fnIXS);
  if (!fin.is_open()) {
    G4Exception("G4VLEPTSModel::ReadIXS",
		"",
		JustWarning,
		(G4String("File not found: ")+ fnIXS).c_str());
    return integralXS;
  }

  G4double EnergySeparator, IonisPot, IonisPotInt, MolecularMass;

  fin >> EnergySeparator >> IonisPot >> IonisPotInt;

  theEnergySeparator[aMaterial] = EnergySeparator / CLHEP::eV;
  theIonisPot[aMaterial] = IonisPot / CLHEP::eV;
  theIonisPotInt[aMaterial] = IonisPotInt / CLHEP::eV;
  theMolecularMass[aMaterial] = aMaterial->GetMassOfMolecule()*CLHEP::Avogadro;

  if( verboseLevel >= 1) G4cout << " IonisPot: " << IonisPot/CLHEP::eV << " eV " 
				<< " IonisPotInt: " << IonisPotInt/CLHEP::eV << " eV" 
				<< " MolecularMass " << MolecularMass/(CLHEP::g/CLHEP::mole) << " g/mole" << G4endl;

  G4int nXSdat, nXSsub;
  fin >> nXSdat >> nXSsub >> fcu;
  G4cout << "Read IXS   (" << fnIXS << ")\t nXSdat: " << nXSdat
	 << " nXSsub: "  << nXSsub << G4endl;
  theNXSdat[aMaterial] = nXSdat;
  theNXSsub[aMaterial] = nXSsub;

  G4double xsdat;
  for (G4int ip=0; ip<=nXSsub; ip++) {   
    integralXS[ip].push_back(0.);
  }
  for (G4int ie=1; ie<=nXSdat; ie++) {
    for (G4int ip=0; ip<=nXSsub; ip++) {   
      fin >> xsdat;
      integralXS[ip].push_back(xsdat*fcu);
      if( verboseLevel >= 3 )  G4cout << GetName() << " FILL IXS " << ip << " " << ie << " = " << integralXS[ip][ie] << " " << xsdat*fcu << G4endl; 
    }
  }
  fin.close();

  return integralXS;
}

