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
                                               const G4String& itsName,
                                               G4int itsRepeatNumberX,
                                               G4int itsRepeatNumberY,
                                               G4int itsRepeatNumberZ,
                                               const G4ThreeVector& itsRepeatVector,
                                               G4bool itsFlagAutoCenter)
  : GateVGlobalPlacement(itsObjectInserter, itsName),
    m_repeatVector(itsRepeatVector),
    m_flagAutoCenter(itsFlagAutoCenter),
    m_placementsFilename("ArrayRepeater_placements.txt"),
    m_Messenger(0)
{
  m_repeatNumber[0] = itsRepeatNumberX;
  m_repeatNumber[1] = itsRepeatNumberY;
  m_repeatNumber[2] = itsRepeatNumberZ;

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
  const G4int nx = m_repeatNumber[0];
  const G4int ny = m_repeatNumber[1];
  const G4int nz = m_repeatNumber[2];
  const G4int configuredCount = nx * ny * nz;

  const G4String& filename = m_placementsFilename;
  std::ifstream inputFile(filename.c_str());
  if (!inputFile.is_open()) {
    G4cerr << "Error: Unable to open file: " << filename << G4endl;
    return;
  }

  std::string line;
  G4double focus = -470.;
  G4double focusY = 0.;
  if (std::getline(inputFile, line)) {
    std::istringstream lineStream(line);
    if (!(lineStream >> focus >> focusY)) {
      G4cerr << "Error: Invalid format for focus in file." << G4endl;
      return;
    }
  }

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
  if (configuredCount != pointCount) {
    G4cout << "Warning: focusArray configured copy count (" << configuredCount
           << ") differs from placement file count (" << pointCount
           << "). Using the placement file count." << G4endl;
  }

  std::vector<G4ThreeVector> points;
  points.reserve(pointCount);
  for (G4int p = 0; p < pointCount; p++) {
    if (!std::getline(inputFile, line)) {
      G4cerr << "Error: Missing point " << p << " in file." << G4endl;
      return;
    }

    std::istringstream lineStream(line);
    G4double x, y, z;
    if (!(lineStream >> x >> y >> z)) {
      G4cerr << "Error: Invalid format for point " << p << " in file." << G4endl;
      return;
    }
    points.push_back(G4ThreeVector(x, y, z));
  }

  const G4ThreeVector targetPoint(focus, focusY, 0.);
  const G4double xSlice = 0.;

  for (std::vector<G4ThreeVector>::const_iterator point = points.begin(); point != points.end(); ++point) {
    G4ThreeVector newPosition = *point;
    const G4double x = newPosition.x();

    if (std::abs(focus - x) > 1e-12) {
      const G4double ratio = (xSlice - x) / (focus - x);
      newPosition = newPosition + ratio * (targetPoint - newPosition);
    }

    G4RotationMatrix rotation = BuildFocusRotation(currentRotationMatrix, newPosition, targetPoint);
    PushBackPlacement(rotation, newPosition);
  }
}

void GateFocusArrayRepeater::DescribeMyself(size_t indent)
{
  G4cout << GateTools::Indent(indent) << "Repetition type:         " << "focusArray" << "\n";
  G4cout << GateTools::Indent(indent) << "Nb of copies along X   : " << m_repeatNumber[0] << "\n";
  G4cout << GateTools::Indent(indent) << "Nb of copies along Y   : " << m_repeatNumber[1] << "\n";
  G4cout << GateTools::Indent(indent) << "Nb of copies along Z   : " << m_repeatNumber[2] << "\n";
  G4cout << GateTools::Indent(indent) << "Repetition step along X: " << G4BestUnit(m_repeatVector[0], "Length") << "\n";
  G4cout << GateTools::Indent(indent) << "Repetition step along Y: " << G4BestUnit(m_repeatVector[1], "Length") << "\n";
  G4cout << GateTools::Indent(indent) << "Repetition step along Z: " << G4BestUnit(m_repeatVector[2], "Length") << "\n";
  G4cout << GateTools::Indent(indent) << "Centering:               " << (GetAutoCenterFlag() ? "Yes" : "No") << "\n";
}
