//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"

#include <stdexcept>

#include <G4Material.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>

#include <G4VPhysicalVolume.hh>

namespace B9 {

DetectorConstruction::DetectorConstruction(const cpop::Population &population):
	fWorldSize(-1*CLHEP::micrometer),
	fMessenger(std::make_unique<DetectorConstructionMessenger>(this)),
	fPopulation(&population)
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
	if(this->getWorldSize() == -1*CLHEP::micrometer)
		throw std::runtime_error("World size not set. Please use /detector/size in your macro file.");

	G4Material* lWater = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER", false, false);

	auto* solidWorld = new G4Box("sWorld", this->getWorldSize(), this->getWorldSize(), this->getWorldSize());
	auto* logicWorld = new G4LogicalVolume( solidWorld, lWater, "LV_World", nullptr, nullptr, nullptr);

	auto world = new G4PVPlacement(  G4Transform3D(),// no rotation
															 logicWorld,     // its logical volume
															 "PV_World",     // its name
															 nullptr,        // its mother  volume
															 false,          // no boolean operations
															 0,              // copy number
															 false);         // surface overlaps

	return world;
}

double DetectorConstruction::getWorldSize() const
{
	return fWorldSize;
}

void DetectorConstruction::setWorldSize(double value)
{
	fWorldSize = value;
}

}
