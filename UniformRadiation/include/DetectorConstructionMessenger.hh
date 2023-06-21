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
//
/// \file DetectorConstructionMessenger.hh
/// \brief Definition of the B7::DetectorConstructionMessenger class

#ifndef B7_DETECTOR_CONSTRUCTION_MESSENGER_HH
#define B7_DETECTOR_CONSTRUCTION_MESSENGER_HH

#include <G4UImessenger.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

namespace B7 {

class DetectorConstruction;

/// Detector construction messenger class to interact with the geometry
/// information via a .mac file

class DetectorConstructionMessenger: public G4UImessenger
{
public:
    DetectorConstructionMessenger(DetectorConstruction* det);

    void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
    DetectorConstruction* fDetector;
    G4UIcmdWithADoubleAndUnit fSizeCmd;
};

}

#endif
