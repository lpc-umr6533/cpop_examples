# CPOP examples

This repository contains 4 CPOP examples:
- GeneratePopulation;
- UniformRadiation;
- NanoparticleRadiation;
- TargetedAlphaTherapy.

You need a valid CPOP installation to compile them,
see https://github.com/lpc-umr6533/cpop

## Build-Test script

A script, `build_test.sh`, is provided to:
- download/build/install Geant4;
- download/build/install CGAL;
- download/build/install CLHEP;
- download/build/install ROOT;
- download/build/install CPOP;
- build the examples;
- test the examples.

Everything is done in a subdirectory `dependencies` created by the script.
You can also follow the instruction below.

## Build steps

```sh
source "${CPOP_DIR}/CPOP/bin/CPOP.sh"
```

```sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Testing

### GeneratePopulation

```sh
cd example/GeneratePopulation
../../GeneratePopulation/generatePopulation --vis -f data/exampleConfig.cfg
```

You can see the generated cell population with `geomview` for example:
```sh
geomview data/exampleConfig.cfg.off
```

### UniformRadiation

```sh
cd example/UniformRadiation
../../UniformRadiation/uniformRadiation -m data/run.mac
```

Output are ROOT files.

### NanoparticleRadiation

```sh
cd example/NanoparticleRadiation
../../NanoparticleRadiation/nanoparticleRadiation -m data/run.mac
```

Output are ROOT files.

### TargetedAlphaTherapy

```sh
cd example/TargetedAlphaTherapy
../../TargetedAlphaTherapy/targetedAlphaTherapy -m data/run.mac
```

ROOT files are in the `output` directory.
