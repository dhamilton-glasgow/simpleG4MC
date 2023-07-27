#include "DetectorConstruction.hh"
#include "VirtualDetectorSD.hh"
#include "RealDetectorSD.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4VisAttributes.hh"
#include "G4String.hh"
#include "globals.hh"

#include <fstream>

using namespace CLHEP;
using namespace std;

//---------------------------------------------------------------------------

DetectorConstruction::DetectorConstruction()
{
  fNistManager  = G4NistManager::Instance();
}

//---------------------------------------------------------------------------

DetectorConstruction::~DetectorConstruction() 
{
}

//---------------------------------------------------------------------------
G4VPhysicalVolume* DetectorConstruction::Construct()
{ 

  //---------------------------------------------------------------------------
  // Target dimensions and material
  G4Material* Target_Material      = fNistManager->FindOrBuildMaterial("G4_lH2");
  G4Material* TargetCell_Material  = fNistManager->FindOrBuildMaterial("G4_Al");
  
  G4double Target_Length          = 10. *cm;
  G4double Target_Radius          = 25.*mm;
  G4double Target_WindowThickness = 0.1*mm;

  //---------------------------------------------------------------------------
  // Detector 1 dimensions and material
  G4double det1_X = 200.*mm; 
  G4double det1_Y = 200.*mm; 
  G4double det1_Z = 200.*mm;
  
  G4Material* det1_material = fNistManager->FindOrBuildMaterial("G4_PbWO4");

  // Detector position
  G4double det1_theta = -25.5 *deg; 
  G4double det1_dist  = 250. *cm; 

  //---------------------------------------------------------------------------
  // Detector 2 dimensions and material
  G4double det2_X = 400.*mm; 
  G4double det2_Y = 400.*mm; 
  G4double det2_Z = 400.*mm;
  
  G4Material* det2_material = fNistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  // Detector position
  G4double det2_theta = 15.5 *deg; 
  G4double det2_dist  = 150. *cm; 

  //---------------------------------------------------------------------------

  G4int SDcount = 1;

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
  //---------------------------------------------------------------------------
  // Create experimental hall
  //---------------------------------------------------------------------------
  
  G4Box* expHall_box           = new G4Box("expHall_box",
					   10.75 *m, 10.75 *m, 10.75 *m );
  

  fexpHall_log = new G4LogicalVolume(expHall_box,
				     fNistManager->FindOrBuildMaterial("G4_AIR"),  
				     "expHall_log", 0, 0, 0);
  
  fExpHall                     = new G4PVPlacement(0, G4ThreeVector(),
						   fexpHall_log, "expHall", 0, false, 0);

  //---------------------------------------------------------------------------
  // Create Target Cell & Target
  //---------------------------------------------------------------------------
  
  G4Tubs* TargetCell_solid = new G4Tubs("TargetCell_solid",           
					0., Target_Radius+Target_WindowThickness, (0.5*Target_Length)+Target_WindowThickness, 0.,twopi); 

  G4LogicalVolume* TargetCell_logic = new G4LogicalVolume(TargetCell_solid, TargetCell_Material, "TargetCell_log");   

  new G4PVPlacement(0, G4ThreeVector(), TargetCell_logic, "TargetCell", fexpHall_log, false, 0 );  

  //---------------------------------------------------------------------------

  G4Tubs* Target_solid = new G4Tubs("Target_solid",         
				    0., Target_Radius, 0.5*Target_Length, 0.,twopi); 

  G4LogicalVolume* Target_logic = new G4LogicalVolume(Target_solid, Target_Material, "Target_logic");  
  
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.5*Target_WindowThickness), Target_logic, "Target", TargetCell_logic, false, 0 ); 

  //--------------------------------------------------------------------------- 
  // Create detector1
  //--------------------------------------------------------------------------- 
  
  G4Box* det1_solid = new G4Box("det1_solid", 0.5*det1_X, 0.5*det1_Y, 0.5*det1_Z);
  
  G4LogicalVolume* det1_log = new G4LogicalVolume( det1_solid, det1_material, "pbwo4_log");

  G4double det1_xprime       = -det1_dist * std::sin(det1_theta); 
  G4double det1_zprime       = det1_dist * std::cos(det1_theta); 
  G4RotationMatrix* det1_rm  = new G4RotationMatrix(); 
  det1_rm->rotateY(det1_theta); 

  fDetVol[SDcount] = new G4PVPlacement(det1_rm, G4ThreeVector(det1_xprime,0.,det1_zprime), 
				       det1_log, "DET1", fexpHall_log, false, SDcount);

  SDcount++;

  //--------------------------------------------------------------------------- 
  // Create detector2
  //--------------------------------------------------------------------------- 
  
  G4Box* det2_solid = new G4Box("det2_solid", 0.5*det2_X, 0.5*det2_Y, 0.5*det2_Z);
  
  G4LogicalVolume* det2_log = new G4LogicalVolume( det2_solid, det2_material, "det2_log");

  G4double det2_xprime       = -det2_dist * std::sin(det2_theta); 
  G4double det2_zprime       = det2_dist * std::cos(det2_theta); 
  G4RotationMatrix* det2_rm  = new G4RotationMatrix(); 
  det2_rm->rotateY(det2_theta); 

  fDetVol[SDcount] = new G4PVPlacement(det2_rm, G4ThreeVector(det2_xprime,0.,det2_zprime), 
				       det2_log, "DET2", fexpHall_log, false, SDcount);

  SDcount++;

  //---------------------------------------------------------------------------
  // Set Logical Attributes
  //---------------------------------------------------------------------------

  // Senstive detector
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  
  fVirtualDetectorSD = new VirtualDetectorSD("VirtualDetectorSD", fNSD );
  SDman->AddNewDetector( fVirtualDetectorSD );
  det1_log->SetSensitiveDetector( fVirtualDetectorSD );
  det2_log->SetSensitiveDetector( fVirtualDetectorSD );

  fRealDetectorSD = new RealDetectorSD("RealDetectorSD", fNSD );
  SDman->AddNewDetector( fRealDetectorSD );
  det1_log->SetSensitiveDetector( fRealDetectorSD );
  det2_log->SetSensitiveDetector( fRealDetectorSD );
  
  // Visualisation
  fexpHall_log->SetVisAttributes(G4VisAttributes::Invisible);

  //---------------------------------------------------------------------------

  return fExpHall;

}

//---------------------------------------------------------------------------

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//---------------------------------------------------------------------------

