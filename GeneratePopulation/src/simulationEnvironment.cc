/*----------------------------------------------------
Copyright (C): Henri Payno, Axel Delsol, Alexis Pereda
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------------------------------------*/
#include "simulationEnvironment.hh"

#include <CellProperties.hh>
#include <G4MaterialPropertiesIndex.hh>
#include <UnitSystemManager.hh>

namespace B6 {

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimulationEnvironment::SetMetricSystem(const std::string& metric) {
  std::string input = metric;
  // transforms the input string to lowercase to be case insensitive
  std::transform(std::begin(input), std::end(input), std::begin(input), ::tolower);

  std::unordered_map<std::string, UnitSystemManager::eMetricSystem> const units{
    {"centimeter", UnitSystemManager::Centimeter},
    {"millimeter", UnitSystemManager::Millimeter},
    {"micrometer", UnitSystemManager::Micrometer},
    {"nanometer", UnitSystemManager::Nanometer},
  };

  if(units.count(metric) == 0) return; // error

  auto const unit = units.at(metric);
  fMetricSystem = UnitSystemManager::getInstance()->getMetricUnit(unit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimulationEnvironment::SetCellProperties(
  double minRadiusNucleus, double maxRadiusNucleus, double minRadiusMembrane,
  double maxRadiusMembrane, const std::string& cytoplasmMaterials, const std::string& nucleusMaterials
) {
  // cells weights range (not used for the simulation but needed to call automaticFill)
  double weightSystem = UnitSystemManager::getInstance()->getWeightUnit(UnitSystemManager::Nanogram);
  CellVariableAttribute<double> weights(1.0*weightSystem, 1.0*weightSystem);
  // membrane radius range
  CellVariableAttribute<double> membraneRadius(minRadiusMembrane*fMetricSystem, maxRadiusMembrane*fMetricSystem);
  // nucleus radius range
  CellVariableAttribute<double> nucleusRadius(minRadiusNucleus*fMetricSystem, maxRadiusNucleus*fMetricSystem);
  // cytoplasm material
  auto* cytoMat = ParseMaterial(cytoplasmMaterials.c_str());
  // nucleoplasm radius
  auto* nucleusMat = ParseMaterial(nucleusMaterials.c_str());
  // fill with those parameters for all life cycle.
  fCellProperties.automaticFill(membraneRadius, weights, nucleusRadius, BARYCENTER, cytoMat, nucleusMat);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimulationEnvironment::SetSpheroidProperties(double internalRadius, double externalRadius, int nbCell) {
  // setup the main environment
  Point_3 center(0., 0., 0.);
  // tell where the cells should be created (here in a spheroid)
  auto* subEnvSD = new SpheresSDelimitation(internalRadius*fMetricSystem, externalRadius*fMetricSystem, center);

  // environment to simulate
  delete fSimulatedEnv;
  fSimulatedEnv = new t_SimulatedSubEnv_3(&fEnv, "MySimulatedSubEnv", static_cast<t_SpatialDelimitation_3*>(subEnvSD));

  // generate cells
  CLHEP::MTwistEngine defaultEngine(1234567);
  RandomEngineManager::getInstance()->setEngine(&defaultEngine);

  std::map<LifeCycles::LifeCycle, double> rates = Utils::generateUniformLifeCycle();
  /// 3.1 get the distribution
  auto* distribution = DistributionFactory::getInstance()->getDistribution<double, Point_3, Vector_3>(Distribution::RANDOM);
  /// 3.2 distribute
  distribution->distribute(fSimulatedEnv, &fCellProperties, nbCell, rates);

  delete distribution;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimulationEnvironment::SetMeshProperties(int nOfFacetPerCell) {
  fNumberOfFacetPerCell = nOfFacetPerCell;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimulationEnvironment::SetForceProperties(double ratioToStableLength, double rigidity) {
  int error = 0;

  // get the generated cells
  auto* voronoiMesh = MeshFactory::getInstance()->create_3DMesh(&error, fSimulatedEnv, MeshTypes::Round_Cell_Tesselation, fNumberOfFacetPerCell);
  auto lCells = voronoiMesh->getCells();
  delete voronoiMesh;

  // apply elastic forces to each cells
  for(auto itCell = std::begin(lCells); itCell != std::end(lCells); ++itCell) {
    auto* elasForce = new t_ElasticForce_3(*itCell, rigidity, ratioToStableLength);
    (*itCell)->addForce(elasForce);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimulationEnvironment::SetSimulationProperties(
  double duration, int numberOfAgentToExecute, double displacementThreshold, double stepDuration
) {
  // set up platform
  fPlatform.setLayerToSimulate(fSimulatedEnv);
  fPlatform.setStepDuration(stepDuration);
  fPlatform.setDuration(duration);
  fPlatform.setDisplacementThreshold(displacementThreshold);
  fPlatform.limiteNbAgentToSimulate(numberOfAgentToExecute);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimulationEnvironment::StartSimulation() {
  /// 4.3 set the adapted spatial data structure permitting agent to know their neighbors)
  fSimulatedEnv->addSpatialDataStructure(new Delaunay_3D_SDS( " my spatial data structure"));
  fPlatform.startSimulation();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimulationEnvironment::SavePopulation(const char* filename) {
  IO::CPOP::save(static_cast<Writable*>(&fEnv), filename);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimulationEnvironment::ExportToVis(const char* filename) {
  int error = 0;
  auto* voronoiMesh = MeshFactory::getInstance()->create_3DMesh(&error, fSimulatedEnv, MeshTypes::Round_Cell_Tesselation, fNumberOfFacetPerCell);
  QString outputName = filename;
  voronoiMesh->exportToFile(outputName, MeshOutFormats::OFF);

  // delete mesh used
  delete voronoiMesh;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* SimulationEnvironment::ParseMaterial(char const* material) {
  G4Material* mat = nullptr;
  if(strcmp(material, "G4_WATER") == 0)
    mat = MaterialManager::getInstance()->getMaterial(material);
  else
    mat = MaterialManager::getInstance()->getDefaultMaterial();

  return mat;
}

}
