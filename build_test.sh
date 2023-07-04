#!/usr/bin/bash

cores=7 # number of cores to use to build
root_dir="$(pwd)/dependencies"
g4_version=v11.1.1
cgal_version=v5.5.1
clhep_version=CLHEP_2_4_6_1
root_version=v6-26-10

die() {
	test -n "$@" && echo>&2 "$@"
	exit 1
}
test "$(git rev-parse --show-toplevel)" = "$(pwd)" || die "you must execute this file from the Git repository root"

mkdir -p "${root_dir}/src"

# Geant 4
install_geant4() {
	cd "${root_dir}/src"
	git clone --depth 1 --branch "${g4_version}" https://github.com/Geant4/geant4.git "geant4-${g4_version}"
	cd "geant4-${g4_version}"
	mkdir -p build
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-std=c++17 -DCMAKE_INSTALL_PREFIX="${root_dir}/geant4-${g4_version}" \
		-DGEANT4_INSTALL_DATA=ON -DGEANT4_INSTALL_DATADIR="${root_dir}/geant4-${g4_version}/data" \
		-DGEANT4_USE_QT=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_GDML=ON -DGEANT4_BUILD_MULTITHREADED=ON \
		..
	make -j"${cores}" install || die "Geant4 build failed"
}

# CGAL
install_cgal() {
	cd "${root_dir}/src"
	git clone --depth 1 --branch "${cgal_version}" https://github.com/CGAL/cgal.git "cgal-${cgal_version}"
	mkdir -p "cgal-${cgal_version}_build"
	cd "cgal-${cgal_version}_build"
	cmake -DCMAKE_BUILD_TYPE=Release -DCGAL_DONT_OVERRIDE_CMAKE_FLAGS=OFF -DCMAKE_INSTALL_PREFIX="${root_dir}/cgal-${cgal_version}" "../cgal-${cgal_version}"
	make -j"${cores}" install
}

# CLHEP
install_clhep() {
	cd "${root_dir}/src"
	git clone --depth 1 --branch "${clhep_version}" https://gitlab.cern.ch/CLHEP/CLHEP.git "clhep-${clhep_version}"
	mkdir -p "clhep-${clhep_version}_build"
	cd "clhep-${clhep_version}_build"
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${root_dir}/clhep-${clhep_version}" "../clhep-${clhep_version}"
	make -j"${cores}" install
}

# ROOT
install_root() {
	cd "${root_dir}/src"
	git clone --depth 1 --branch "${root_version}" https://github.com/root-project/root.git "root-${root_version}"
	mkdir -p "root-${root_version}_build"
	cd "root-${root_version}_build"
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${root_dir}/root-${root_version}" "../root-${root_version}"
	make -j"${cores}" install
}

# CPOP
install_cpop() {
	cd "${root_dir}/src"
	git clone https://github.com/lpc-umr6533/cpop.git
	cd cpop
	mkdir -p build
	cd build

	(
		source "${root_dir}/geant4-${g4_version}/bin/geant4.sh"
		source "${root_dir}/root-${root_version}/bin/thisroot.sh"
		cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${root_dir}/cpop" \
			-DCGAL_DIR="${root_dir}/cgal-${cgal_version}/lib/cmake/CGAL" \
			-DCLHEP_DIR="${root_dir}/clhep-${clhep_version}/lib/CLHEP-2.4.6.1" \
			-DOpenGL_GL_PREFERENCE=LEGACY \
			..
		make -j"${cores}" install || die "CPOP build failed"
	) || die
}

# Examples
build_examples() {
	cd "$(git rev-parse --show-toplevel)"
	mkdir -p build
	cd build
	(
		source "${root_dir}/cpop/CPOP/bin/CPOP.sh"
		cmake -DCMAKE_BUILD_TYPE=Release ..
		make -j"${cores}" || die "CPOP example build failed"
	) || die
}

# Tests
run_examples() {
	cd "$(git rev-parse --show-toplevel)/build/example"

	(
		source "${root_dir}/geant4-${g4_version}/bin/geant4.sh"
		source "${root_dir}/cpop/CPOP/bin/CPOP.sh"
		## Generate population
		cd GeneratePopulation
		# ../../GeneratePopulation/generatePopulation --vis -f data/exampleConfig.cfg || die "GeneratePopulation example failed"
		#:: view with geomview for example
		cd ..

		pwd
		## Uniform radiation
		cd UniformRadiation
		../../UniformRadiation/uniformRadiation -m data/run.mac || die "UniformRadiation example failed"
		cd ..

		## Nanoparticle radiation
		cd NanoparticleRadiation
		../../NanoparticleRadiation/nanoparticleRadiation -m data/run.mac || die "NanoparticleRadiation example failed"
		cd ..

		## Targeted alpha therapy
		cd TargetedAlphaTherapy
		../../TargetedAlphaTherapy/targetedAlphaTherapy -m data/run.mac || die "TargetedAlphaTherapy example failed"
		cd ..
	) || die

	echo "----------------------------------------"
	echo "All tests succeeded"
}

install_geant4
install_cgal
install_clhep
install_root
install_cpop
build_examples
run_examples
