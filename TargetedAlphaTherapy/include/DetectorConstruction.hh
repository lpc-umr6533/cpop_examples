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
/// \file DetectorConstruction.hh
/// \brief Definition of the B9::DetectorConstruction class

#ifndef B9_DETECTOR_CONSTRUCTION_HH
#define B9_DETECTOR_CONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <Population.hh>

namespace cpop {

class Population;

}

namespace B9 {

class DetectorConstructionMessenger;

/// Detector construction class to define materials and geometry.

class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
	DetectorConstruction(const cpop::Population& population);

	G4VPhysicalVolume* Construct() override;

	[[nodiscard]] double getWorldSize() const;
	void setWorldSize(double value);

private:
	double fWorldSize;
	std::unique_ptr<DetectorConstructionMessenger> fMessenger;
	const cpop::Population* fPopulation;

};

}

#endif // DETECTORCONSTRUCTION_HH
