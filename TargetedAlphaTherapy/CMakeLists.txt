##########################################################
# Copyright (C): Henri Payno, Axel Delsol, Alexis Pereda #
# Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA  #
#                                                        #
# This software is distributed under the terms           #
# of the GNU Lesser General  Public Licence (LGPL)       #
# See LICENSE.md for further detais                      #
##########################################################
cmake_minimum_required(VERSION 3.7)

project(TargetedAlphaTherapy)
set(BINARY_NAME targetedAlphaTherapy)

set(ALL_SOURCE
	src/main.cc
	src/DetectorConstruction.cc
	src/DetectorConstructionMessenger.cc
)

set(ALL_HEADER
	include/DetectorConstruction.hh
	include/DetectorConstructionMessenger.hh
)

add_executable(${BINARY_NAME} ${ALL_SOURCE} ${ALL_HEADER})

target_include_directories(${BINARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(${BINARY_NAME} PUBLIC -Wall -pthread)
target_link_libraries(${BINARY_NAME} PUBLIC Platform_SMA Modeler)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/example/TargetedAlphaTherapy)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/example/TargetedAlphaTherapy/output)
