
![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/OpenGATE/Gate?logo=github)
[![CI](https://github.com/OpenGATE/Gate/actions/workflows/main.yml/badge.svg)](https://github.com/OpenGATE/Gate/actions/workflows/main.yml)
[![Read the Docs](https://img.shields.io/readthedocs/opengate?logo=read-the-docs&style=plastic)](https://opengate.readthedocs.io/)
[![CDash](https://img.shields.io/badge/CDash-passing-green?style=plastic)](https://my.cdash.org/index.php?project=GATE)
[![GitHub issues](https://img.shields.io/github/issues-raw/OpenGATE/Gate?logo=github&style=plastic)](https://github.com/OpenGATE/Gate/issues)
[![Mailing List](https://img.shields.io/badge/mailing%20list-subscribe-blue?logo=minutemailer&style=plastic)](https://listserv.in2p3.fr/cgi-bin/wa?A0=OPENGATE-L)
[![Docker](https://img.shields.io/docker/automated/opengatecollaboration/gate?style=plastic)](https://hub.docker.com/r/opengatecollaboration/gate)
[![Website](https://img.shields.io/badge/website-OpenGateCollaboration-blue?style=plastic)](http://www.opengatecollaboration.org/)
[![GitHub](https://img.shields.io/github/license/OpenGATE/Gate?style=plastic)](https://github.com/OpenGATE/Gate/blob/develop/LICENSE.md)

https://github.com/OpenGATE/Gate is the public repository of the GATE software. 

More details : http://www.opengatecollaboration.org

Documentation : https://opengate.readthedocs.io

* The stable version is tag 9.4.1: https://github.com/OpenGATE/Gate/tree/v9.4.1 with Geant4 11.3.0
* The current development version is branch 'develop' (default): http://github.com/OpenGATE/Gate/tree/develop
* [Benchmarks](https://github.com/OpenGATE/GateBenchmarks)
* [Examples](https://github.com/OpenGATE/GateContrib)
* [Exercises](https://davidsarrut.pages.in2p3.fr/gate-exercices-site)

References collaboration papers:

* 2021 http://www.ncbi.nlm.nih.gov/pubmed/33770774
* 2014 http://www.ncbi.nlm.nih.gov/pubmed/24877844
* 2011 http://www.ncbi.nlm.nih.gov/pubmed/21248393
* 2004 http://www.ncbi.nlm.nih.gov/pubmed/15552416



# Focus Array Extension for GATE 9.4.1

## Overview

This repository contains a custom extension to GATE 9.4.1 for modeling a focused repeating geometry, developed for converging-hole collimator simulations and optimization.

The main addition is a new object repeater called `focusArray`. Unlike the standard `cubicArray` repeater, which generates copies on a regular Cartesian grid, `focusArray` reads user-defined copy positions from a text file and rotates each copy so that it points toward a common focus.

This implementation was used to simulate and optimize a converging-hole collimator scheme.

## What Was Added

The following new files were added:

- `source/geometry/include/GateFocusArrayRepeater.hh`
- `source/geometry/include/GateFocusArrayRepeaterMessenger.hh`
- `source/geometry/src/GateFocusArrayRepeater.cc`
- `source/geometry/src/GateFocusArrayRepeaterMessenger.cc`

## What Was Modified

The following existing files were modified:

- `source/geometry/src/GateObjectRepeaterListMessenger.cc`
  Registered the new repeater type `focusArray` so that it can be inserted from GATE macro files.

## Main Idea

The custom repeater works as follows:

1. A placement file is read from disk.
2. The file provides:
   - the focus position in the `x-y` plane,
   - the number of placements,
   - the list of copy coordinates.
3. Each copy is translated to its prescribed position.
4. Each copy is rotated so that its local forward axis points toward the focus.

## Build Instructions

After modifying the source tree, re-run CMake so that the new files are included in the build:

```bash
cmake -S /path/to/Gate-9.4.1 -B /path/to/Gate-9.4.1/build
cmake --build /path/to/Gate-9.4.1/build -j16
```
## How to Use `focusArray`

### Step 1: Insert the repeater in a macro

Minimal example:

```text
/gate/crystal/repeaters/insert focusArray
/gate/crystal/focusArray/setPlacementsFilename /absolute/path/to/FocusArray_placements.txt
```

If you prefer a custom repeater name:

```text
/gate/crystal/repeaters/name focusArrayRepeater
/gate/crystal/repeaters/insert focusArray
/gate/crystal/focusArrayRepeater/setPlacementsFilename /absolute/path/to/FocusArray_placements.txt
```

## Placement File Format

The placement file must have the following structure:

```text
focusX focusY
N
x1 y1 z1
x2 y2 z2
x3 y3 z3
...
```

### Meaning of each line

- Line 1: focus position in the `x-y` plane
  - `focusX focusY`
  - the current implementation assumes `focusZ = 0`

- Line 2: number of placements
  - `N`

- Line 3 onward: one copy position per line
  - `x y z`

Example:

```text
-470 0
4
0 10 10
0 -10 10
0 10 -10
0 -10 -10
```

## Important Behavior

### Actual number of copies

The actual number of copies is determined by the placement file, not by:

- `setRepeatNumberX`
- `setRepeatNumberY`
- `setRepeatNumberZ`

In the current implementation:

- the code reads the point count from the file,
- it places all points listed in the file,
- if `Nx * Ny * Nz` differs from the file count, only a warning is printed.

### Path handling

`setPlacementsFilename` accepts either:

- an absolute path, or
- a relative path

Absolute paths are strongly recommended for reproducibility.

### Orientation convention

Each copy is rotated so that it points toward the common focus. The current implementation aligns the local forward direction with the global `+x` reference used in the custom rotation routine.

## Recommended Minimal Workflow

1. Prepare the focused geometry coordinates in a text file.
2. Insert `focusArray` in the volume that should be repeated.
3. Set the placement filename in the macro.
4. Initialize and run GATE as usual.

Example:

```text
/gate/crystal/repeaters/insert focusArray
/gate/crystal/focusArray/setPlacementsFilename /home/user/data/FocusArray_placements.txt
/gate/run/initialize
```

## Example Use Case

This extension is intended for geometries where repeated detector or collimator elements must:

- be placed at arbitrary coordinates,
- share a common geometric focus,
- represent converging-hole or other focused collimator arrangements.

## Current Limitations

- The focus is read as `focusX focusY`, with `focusZ` fixed to `0`.
- The implementation uses an internal projection toward `xSlice = 0`.
- `setRepeatVector` and `autoCenter` are kept for interface compatibility, but the actual positions are driven by the placement file.
- This implementation is a custom extension and is not part of the official GATE release.

## Summary of User-Facing Commands

For a volume named `crystal`, the most useful commands are:

```text
/gate/crystal/repeaters/insert focusArray
/gate/crystal/focusArray/setPlacementsFilename /absolute/path/to/FocusArray_placements.txt
```

If a custom repeater name is used:

```text
/gate/crystal/repeaters/name focusArrayRepeater
/gate/crystal/repeaters/insert focusArray
/gate/crystal/focusArrayRepeater/setPlacementsFilename /absolute/path/to/FocusArray_placements.txt
```

## Citation / Reuse

If you use this custom extension in publications, please describe it as a user-added GATE repeater for focused array placement and cite the corresponding methodological paper or repository associated with your study.

