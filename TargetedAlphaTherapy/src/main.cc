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

#define G4MULTITHREADED // TODO hack
#ifdef G4MULTITHREADED
#include <G4MTRunManager.hh>
#else
#include <G4RunManager.hh>
#endif

#include <cReader/zupply.hpp>
#include <Population.hh>
#include <PhysicsList.hh>
#include <ActionInitialization.hh>

#include "DetectorConstruction.hh"

#include <G4UImanager.hh>
#include <Randomize.hh>

#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif

#ifdef G4UI_USE
#include <G4UIExecutive.hh>
#endif

#include <RandomEngineManager.hh>

#include <ctime>
#include <chrono>
#include <memory>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
	auto start = std::chrono::high_resolution_clock::now();

	CLHEP::MTwistEngine defaultEngine(123456);
	G4Random::setTheEngine(&defaultEngine);

	CLHEP::MTwistEngine defaultEngineCPOP(456123);
	RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

	// Command line arguments
	// First we add an argument parser to add parameters
	zz::cfg::ArgParser parser;

#ifdef G4MULTITHREADED
	// Get the spectrum file. Specify option -t nbThread or --thread nbThread
	int nThreads = 1;
	parser.add_opt_value('t', "thread", nThreads, 0, "number of threads", "int");
#endif

	// Get the macro file. Specify option -m <fileName> or --macro <filename>
	std::string macro;
	parser.add_opt_value('m', "macro", macro, std::string("input_filename.mac"), "macro file", "file").require();

	parser.parse(argc, argv);

	// check errors
	if(parser.count_error() > 0)
	{
		std::cout << parser.get_error() << std::endl;
		std::cout << parser.get_help() << std::endl;
		return 1;
	}


	// Construct the default run manager
	//
#ifdef G4MULTITHREADED
	auto* runManager = new G4MTRunManager;
	if(nThreads > 0) {
		runManager->SetNumberOfThreads(nThreads);
	}
#else
	auto* runManager = new G4RunManager;
#endif

	// Create a population

	std::ofstream id_cell_file;
	id_cell_file.open("IDCell.txt");

	cpop::Population population;
	population.messenger().BuildCommands("/cpop");

	// Set mandatory initialization classes
	//

	// Set the geometry ie a box filled with G4_WATER
	auto* detector = new B9::DetectorConstruction(population);
	runManager->SetUserInitialization(detector);

	// Set the physics list
	auto* physicsList = new cpop::PhysicsList;
	physicsList->messenger().BuildCommands("/cpop/physics");
	runManager->SetUserInitialization(physicsList);

	// Set custom action to extract informations from the simulation
	auto* actionInitialisation = new cpop::ActionInitialization(population);
	runManager->SetUserInitialization(actionInitialisation);


	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	G4String command = "/control/execute ";
	UImanager->ApplyCommand(command+macro);

	id_cell_file.close();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted
	// in the main() program
	delete runManager;

	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
