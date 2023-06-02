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
/// \file unitSection.hh
/// \brief Definition of the B6::UnitSection class

#ifndef B6_UNIT_SECTION_H
#define B6_UNIT_SECTION_H

#include <cReader/sectionreader.hh>
#include <string>

// How to create your own configuration reader to build a T object
/* 1) Declare a ConfigReader object
 * conf::ConfigReader<T>* reader = new conf::ConfigReader<T>();
 *
 * 2) Add sections to the reader
 * reader->addSection<Section1>();
 * reader->addSection<Section2>();
 *
 * 3) Parse your file and retrieve your object
 * T* myObject = reader->parse(configFile);
 *
 * 4) Do not forget to free memory when you do not need it anymore
 * delete reader;
 * delete myObject;
 */

// How to create custom sections
/* 1) Include #include <cReader/sectionreader.hh>
 *
 * 2) Create a template class which inherits from conf::SectionReader<T>
 *
 * 3) Overload the fill method. One section class should only read one section in the configuration file
 * We provide a load method to read parts of the configuration file.
 * If you want to read :
 * _int    : int myInt = this->template load<int>(sectionName, keyName);
 * _double : double myDouble = this->template load<double>(sectionName, keyName);
 * _string : string myString = this->template load<string>(sectionName, keyName);
 * _vector : std::vector<double> myVector = this->template load<std::vector<double>>(sectionName, keyName);
 *
 * Note : vector can contain any type that supports << to a stringstream (according to zupply documentation http://zhreshold.github.io/zupply/classzz_1_1cfg_1_1_value.html)
 *
 * 4) You can get the object you are building this->objToFill and then call whatever methods/function you want
 *
 */

 ///UnitSection class
 ///
 ///It contains the metric system information

namespace B6 {

template <typename T>
class UnitSection: public conf::SectionReader<T> {
public:
	void fill() override {
		const char sectionName[] = "UnitProperties";

		std::string metricSystem = this->template load<std::string>(sectionName, "metricSystem");

		this->objToFill->SetMetricSystem(metricSystem);
	}
};

}

#endif
