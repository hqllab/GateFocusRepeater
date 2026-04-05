/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/

#include "GateFocusArrayRepeater.hh"
#include "GateFocusArrayRepeaterMessenger.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4UnitsTable.hh"

#include "GateTools.hh"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

namespace
{
struct FocusPlacement
{
  G4ThreeVector center;
  G4ThreeVector target;
};

G4RotationMatrix BuildFocusRotation(const G4RotationMatrix& currentRotationMatrix,
                                    const G4ThreeVector& newPosition,
                                    const G4ThreeVector& targetPoint)
{
  const G4double pi = 3.141592653589793238462643383279502884197;
  const G4double tolerance = 1e-12;

  G4ThreeVector currentDirection = targetPoint - newPosition;
  if (currentDirection.mag2() <= tolerance)
    return currentRotationMatrix;

  currentDirection = currentDirection.unit();
  G4ThreeVector targetDirection(1., 0., 0.);

  G4double dotProduct = currentDirection.dot(targetDirection);
  dotProduct = std::max(-1.0, std::min(1.0, dotProduct));

  if (std::abs(newPosition.y()) < 0.0001 && std::abs(newPosition.z()) < 0.0001) {
    if (dotProduct < 0.) {
      G4RotationMatrix rotation;
      rotation.rotate(pi, G4ThreeVector(0., 1., 0.));
      return currentRotationMatrix * rotation;
    }
    return currentRotationMatrix;
  }

  G4ThreeVector axis = currentDirection.cross(targetDirection);
  if (axis.mag2() <= tolerance) {
    if (dotProduct < 0.) {
      G4RotationMatrix rotation;
      rotation.rotate(pi, G4ThreeVector(0., 1., 0.));
      return currentRotationMatrix * rotation;
    }
    return currentRotationMatrix;
  }

  G4double angle = std::acos(dotProduct);
  G4RotationMatrix rotation;
  rotation.rotate(angle, axis.unit());
  return currentRotationMatrix * rotation;
}
}

GateFocusArrayRepeater::GateFocusArrayRepeater(GateVVolume* itsObjectInserter,
                                               const G4String& itsName)
  : GateVGlobalPlacement(itsObjectInserter, itsName),
    m_placementsFilename("ArrayRepeater_placements.txt"),
    m_Messenger(0)
{
  m_Messenger = new GateFocusArrayRepeaterMessenger(this);
}

GateFocusArrayRepeater::~GateFocusArrayRepeater()
{
  delete m_Messenger;
}

void GateFocusArrayRepeater::PushMyPlacements(const G4RotationMatrix& currentRotationMatrix,
                                              const G4ThreeVector&,
                                              G4double)
{
  const G4String& filename = m_placementsFilename;
  std::ifstream inputFile(filename.c_str());
  if (!inputFile.is_open()) {
    G4cerr << "Error: Unable to open file: " << filename << G4endl;
    return;
  }

  std::string line;
  G4int pointCount = 0;
  if (std::getline(inputFile, line)) {
    std::istringstream lineStream(line);
    if (!(lineStream >> pointCount)) {
      G4cerr << "Error: Invalid format for point count in file." << G4endl;
      return;
    }
  }

  G4cout << "points num:" << pointCount << G4endl;
  if (pointCount <= 0) {
    G4cerr << "Error: Invalid point count in " << filename << G4endl;
    return;
  }

  std::vector<FocusPlacement> placements;
  placements.reserve(pointCount);
  for (G4int p = 0; p < pointCount; p++) {
    if (!std::getline(inputFile, line)) {
      G4cerr << "Error: Missing point " << p << " in file." << G4endl;
      return;
    }

    std::istringstream lineStream(line);
    G4double centerX, centerY, centerZ;
    G4double targetX, targetY, targetZ;
    if (!(lineStream >> centerX >> centerY >> centerZ >> targetX >> targetY >> targetZ)) {
      G4cerr << "Error: Invalid format for point " << p
             << " in file. Expected: centerX centerY centerZ targetX targetY targetZ"
             << G4endl;
      return;
    }
    FocusPlacement placement;
    placement.center = G4ThreeVector(centerX, centerY, centerZ);
    placement.target = G4ThreeVector(targetX, targetY, targetZ);
    placements.push_back(placement);
  }

  for (std::vector<FocusPlacement>::const_iterator placement = placements.begin();
       placement != placements.end(); ++placement) {
    G4RotationMatrix rotation =
      BuildFocusRotation(currentRotationMatrix, placement->center, placement->target);
    PushBackPlacement(rotation, placement->center);
  }
}

void GateFocusArrayRepeater::DescribeMyself(size_t indent)
{
  G4cout << GateTools::Indent(indent) << "Repetition type:         " << "focusArray" << "\n";
  G4cout << GateTools::Indent(indent) << "Placements file:         " << m_placementsFilename << "\n";
}
