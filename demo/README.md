# `focusArray` example for GATE

This example demonstrates how to use the `focusArray` repeater provided by
[GateFocusRepeater](https://github.com/hqllab/GateFocusRepeater) to construct a
focused-hole SPECT collimator in GATE 9.4.1.

The example defines one conical air hole and repeats it at the positions listed
in `ArrayRepeater_placements.txt`. Each repeated hole is rotated toward its
corresponding target point. Using one common target produces a cone-beam
collimator; varying the target points along a line produces a fan-beam
collimator.

## Prerequisites

- GATE 9.4.1 built with the `focusArray` extension from this repository.
- Geant4 11.3.0 and the other dependencies required by GATE 9.4.1.
- A working GATE environment in which the `Gate` executable is available.
- ROOT if the ROOT output in `secondPart.mac` is enabled.

The repository must be configured and rebuilt after adding the extension:

```bash
cmake -S /path/to/GateFocusRepeater -B /path/to/GateFocusRepeater/build
cmake --build /path/to/GateFocusRepeater/build -j16
```

## Files in this example

| File | Purpose |
| --- | --- |
| `main.mac` | Defines the world, SPECT head, collimator, seed hole, crystal, phantom, physics, and visualization. |
| `ArrayRepeater_placements.txt` | Lists the center and target point of every repeated hole. |
| `secondPart.mac` | Defines the digitizer, output, acquisition timing, and starts the simulation. |
| `Defrise.mac` | Defines the seven radioactive disks of the Defrise phantom. |
| `GateMaterials.db` | Material database used by the macro. |
| `vis.mac` | OpenGL visualization settings. |
| `Verbose.mac` | GATE and Geant4 verbosity settings. |
| `MoveVisu.mac` | Optional visualization timing commands. |
| `run.sh` | Example SLURM/Nix launcher used in the original environment. |
| `output/` | Output directory for projection and ROOT files. |


## Run the example

Run GATE from this directory so that all relative paths resolve correctly:

```bash
cd /path/to/GateFocusRepeater/demo
Gate main.mac
```

The provided `run.sh` is specific to a SLURM cluster and a Nix environment. It
can be used directly only if the referenced Nix configuration is available:

```bash
sbatch run.sh
```

For a quick geometry check, keep visualization enabled in `main.mac`. For a
batch simulation, comment out `/control/execute vis.mac` or replace the
visualization configuration with `/vis/disable`.

The example writes projection files and a ROOT file under `output/`. Create the
directory before running if it does not already exist:

```bash
mkdir -p output
```

## Define the collimator and one seed hole

The collimator is a tungsten box whose thickness is its X length:

```text
/gate/SPECThead/daughters/name collimator
/gate/SPECThead/daughters/insert box
/gate/collimator/geometry/setXLength 4. cm
/gate/collimator/geometry/setYLength 50. cm
/gate/collimator/geometry/setZLength 50. cm
/gate/collimator/setMaterial Tungsten
```

A conical air volume is then created as a daughter of the collimator:

```text
/gate/collimator/daughters/name hole
/gate/collimator/daughters/insert cone
/gate/hole/geometry/setRmin1 0. cm
/gate/hole/geometry/setRmin2 0. cm
/gate/hole/geometry/setRmax1 0.08 cm
/gate/hole/geometry/setRmax2 0.085 cm
/gate/hole/geometry/setHeight 5.2 cm
/gate/hole/placement/setRotationAxis 0 1 0
/gate/hole/placement/setRotationAngle 90 deg
/gate/hole/setMaterial Air
```

GATE cones are initially aligned with their local Z axis. The 90-degree
rotation about Y aligns the seed hole with X, which is the collimator-thickness
direction used by this example.

### Important: the hole length is determined by the collimator thickness

`focusArray` controls the number, position, and orientation of the repeated
holes, but it does **not** set their length. The repeated holes inherit the
geometry of the seed `hole`. Therefore, set
`/gate/hole/geometry/setHeight` according to the collimator thickness defined by
`/gate/collimator/geometry/setXLength`.

For a straight hole, the required length is approximately the collimator
thickness. For an oblique hole at angle `theta` from the thickness direction,
the path through a slab of thickness `T` is approximately:

```text
hole length = T / cos(theta)
```

The `hole` height should be long enough for the most oblique repeated hole, with
a small margin to ensure that every repeated air hole fully crosses the
tungsten. In this example the collimator is 4 cm thick and
`/gate/hole/geometry/setHeight` is 5.2 cm. If the collimator thickness or
maximum focusing angle is changed, recalculate and increase the `hole` height
as needed. Because every repeated hole is a copy of this `hole` geometry,
changing this one parameter changes the length of all repeated air holes.

## Apply the `focusArray` repeater

The repeater is attached to the seed hole with two commands:

```text
/gate/hole/repeaters/insert focusArray
/gate/hole/focusArray/setPlacementsFilename ArrayRepeater_placements.txt
```

The number of holes is taken from the placement file. The legacy
`setRepeatNumberX`, `setRepeatNumberY`, and `setRepeatNumberZ` commands do not
control the number of copies in the current `focusArray` implementation.

Paths may be absolute or relative. Relative paths are resolved from the working
directory in which GATE is started, which is why the example should be run from
`demo/`.

## Placement-file format

The placement file is plain text:

```text
N
centerX centerY centerZ targetX targetY targetZ
centerX centerY centerZ targetX targetY targetZ
...
```

- `N` is the number of holes and must equal the number of following data rows.
- `centerX centerY centerZ` is the reference center of a hole.
- `targetX targetY targetZ` is the point toward which that hole is aimed.
- All six coordinates are interpreted in millimetres by the current
  implementation.
- Coordinates are expressed in the local coordinate system of the mother
  volume (`collimator` in this example).

A minimal cone-beam layout, in which all holes share one focal point, is:

```text
4
20.0 -100.0 -100.0 -470.0 0.0 0.0
20.0  -93.8 -100.0 -470.0 0.0 0.0
20.0 -100.0  -93.8 -470.0 0.0 0.0
20.0  -93.8  -93.8 -470.0 0.0 0.0
```

A minimal fan-beam layout varies the target along one axis:

```text
4
20.0 -100.0 -100.0 -470.0 -30.0 0.0
20.0  -93.8 -100.0 -470.0 -10.0 0.0
20.0 -100.0  -93.8 -470.0  10.0 0.0
20.0  -93.8  -93.8 -470.0  30.0 0.0
```

The current implementation projects each supplied center onto the local plane
`X = 0` before placing the repeated volume, and then rotates the volume toward
the supplied target. Consequently, the center and target coordinates must use
the same local coordinate convention as the example.

## Geometry coverage and definition order

This example relies on a coverage rule: when a volume is defined and another
volume is subsequently defined at the same location, the later definition is
the one present at that location. The tungsten `collimator` is therefore
defined first, followed by its air `hole`; the air volume covers the coincident
tungsten region and forms the aperture. Every copy created by `focusArray`
inherits the `Air` material of the seed hole.

In short, for this collimator the intended order is:

```text
Tungsten collimator first -> Air holes second
```

Definition order is significant. Reversing it would not produce the intended
air channels. This rule should be used deliberately for the parent/daughter
construction shown here; avoid accidental overlaps between unrelated or
sibling volumes, because such geometry is ambiguous and may produce navigation
errors.

## What the complete example simulates

The geometry in `main.mac` contains:

- a 4 cm thick tungsten focused-hole collimator;
- a 0.95 cm thick NaI crystal behind the collimator;
- an air cylinder used as the phantom container;
- seven thin cylindrical Tc-99m-like, 140 keV sources defined in
  `Defrise.mac`;
- a rotating phantom that supplies angular projections.

`secondPart.mac` applies 10% energy resolution at 140 keV, 3 mm spatial
resolution, and a 126--154 keV energy window. It writes a 256 x 256 projection
with 2.1866 mm pixels in the YZ plane.

The default activity and acquisition time are intentionally very small, so the
example can be used as a fast smoke test. Increase `ACTIVITY`, `PROJTIME`, and
the acquisition timing only after the geometry has been visually checked.

## Common modifications

### Change the collimator thickness

Update both the collimator thickness and the `hole` height:

```text
/gate/collimator/geometry/setXLength NEW_THICKNESS
/gate/hole/geometry/setHeight NEW_HOLE_LENGTH
```

Choose `NEW_HOLE_LENGTH` from the new thickness and maximum hole angle, as
described above.

### Change the hole diameter or taper

Edit `setRmax1` and `setRmax2`. Equal values create a cylindrical hole; unequal
values create a tapered hole.

### Change cone-beam to fan-beam focusing

Keep one common target for all rows to obtain cone-beam focusing. Vary the
target coordinates along a line to obtain fan-beam focusing. No change to the
two `focusArray` macro commands is required.

### Change the number of holes

Add or remove placement rows and update the first line of the placement file.
No repeat-count command is required in the macro.

## Troubleshooting

- **`focusArray` is not recognized:** rebuild GATE from this repository and
  verify that the new repeater sources were compiled.
- **The placement file cannot be opened:** run GATE from `demo/` or use an
  absolute path in `setPlacementsFilename`.
- **The holes point in the wrong direction:** verify the local-axis convention,
  the seed-hole 90-degree rotation, and the order of center and target values.
- **An air hole does not fully penetrate the tungsten collimator:** the air hole
  is too short. Increase `/gate/hole/geometry/setHeight`, using the collimator
  thickness and maximum focusing angle to determine the required hole length.
- **GATE reports overlaps or navigation errors:** check the placement
  coordinates and ensure that unintended sibling volumes do not overlap.
- **No output is created:** verify that `output/` exists and is writable.
