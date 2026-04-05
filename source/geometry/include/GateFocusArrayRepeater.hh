/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/

#ifndef GateFocusArrayRepeater_h
#define GateFocusArrayRepeater_h 1

#include "globals.hh"
#include <string>

#include "GateVGlobalPlacement.hh"

class GateFocusArrayRepeaterMessenger;

class GateFocusArrayRepeater : public GateVGlobalPlacement
{
  public:
    GateFocusArrayRepeater(GateVVolume* itsObjectInserter,
                           const G4String& itsName = "focusArray");
    virtual ~GateFocusArrayRepeater();

  public:
    virtual void PushMyPlacements(const G4RotationMatrix& currentRotationMatrix,
                                  const G4ThreeVector& currentPosition,
                                  G4double aTime);
    virtual void DescribeMyself(size_t indent);

  public:
    inline const G4String& GetPlacementsFilename() const
      { return m_placementsFilename; }

    void SetPlacementsFilename(const G4String& val)
      { m_placementsFilename = val; }

  protected:
    G4String m_placementsFilename;

    GateFocusArrayRepeaterMessenger* m_Messenger;
};

#endif
