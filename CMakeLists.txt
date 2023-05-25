cmake_minimum_required(VERSION 3.7)

project("CPOP examples")


set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/alexis/software/cpop/lib/cmake/CPOP)

option(WITH_GEANT_4 "Import Geant4 to be directly complient with G4" ON)
option(WITH_EXTERNAL_CLHEP "Use an external CLHEP build" ON)
option(WITH_GDML_EXPORT "Unable to be GDML compliant ?" ON)
option(CPOP_IMPORT_INTERNAL_GDML OFF)

find_package(CPOP CONFIG REQUIRED)
find_package(Geant4 REQUIRED)
find_package(CLHEP REQUIRED)
include(${Geant4_USE_FILE})

add_subdirectory(GeneratePopulation)
add_subdirectory(UniformRadiation)
add_subdirectory(NanoparticleRadiation)
add_subdirectory(TargetedAlphaTherapy)
