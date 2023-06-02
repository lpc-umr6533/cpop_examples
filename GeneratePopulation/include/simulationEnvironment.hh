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
/// \file simulationEnvironment.hh
/// \brief Definition of the B6::SimulationEnvironment class

#ifndef B6_SIMULATION_ENVIRONMENT_H
#define B6_SIMULATION_ENVIRONMENT_H

#include <algorithm>
#include <string>

#include <CellFactory.hh>     // needed to call the mesh factory, creating the mesh
#include <Cell_Utils.hh>      // used for the getNearestAndFarthestPoints function
#include <DistributionFactory.hh> // used to distribute cell inside the sub environment
#include <ElasticForce.hh>      // The type of force we want to apply
#include <MASPlatform.hh>     // THe platform used to manage agent ( cell ) execution
#include <File_CPOP_Data.hh>    // CPOP tools for saving files
#include <MeshFactory.hh>     // used to get the reuested mesh
#include <RandomEngineManager.hh> // manager used to generate Random numbers
#include <SimpleSpheroidalCell.hh>  // the basic cell we want to use
#include <SpheresSDelimitation.hh>  // the spatial delimitation we are using
#include <UnitSystemManager.hh>   // used to have the requested unitn

#include <CLHEP/Random/MTwistEngine.h>

#include <map>
#include <set>

///SimulationEnvironment class
///
///It gets all the parameters given as an input, calls the needed functions
///to generate the cell population, and saves it.

namespace B6 {

using namespace Settings::nCell;
using namespace Settings::nEnvironment;

class SimulationEnvironment {
public:
  using t_ElasticForce_3 = ElasticForce<double, Point_3, Vector_3>;

private:
  double fMetricSystem{1.0};
  RoundCellProperties fCellProperties;

  // Spheroid properties
  t_Environment_3 fEnv{"main environment"};
  // automatically deleted
  t_SimulatedSubEnv_3* fSimulatedEnv{nullptr};

  // Mesh properties
  int fNumberOfFacetPerCell{50};

  // Simulation properties
  MASPlatform fPlatform;

public:
  // Setter used by the xxxSection class
  void SetMetricSystem(const std::string& metric);
  void SetCellProperties(
    double minRadiusNucleus, double maxRadiusNucleus, double minRadiusMembrane,
    double maxRadiusMembrane, const std::string& cytoplasmMaterials, const std::string& nucleusMaterials
  );
  void SetSpheroidProperties(double internalRadius, double externalRadius, int nbCell);
  void SetMeshProperties(int nOfFacetPerCell);
  void SetForceProperties(double ratioToStableLength, double rigidity);
  void SetSimulationProperties(double duration, int numberOfAgentToExecute, double displacementThreshold, double stepDuration);

  // start the simulation
  void StartSimulation();

  // save the population
  void SavePopulation(const char* filename);

  // export to off format to visualise the population
  void ExportToVis(const char* filename);

private:
  static G4Material* ParseMaterial(const char* material);
};

}

#endif
