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
                           const G4String& itsName = "focusArray",
                           G4int itsRepeatNumberX = 1,
                           G4int itsRepeatNumberY = 1,
                           G4int itsRepeatNumberZ = 1,
                           const G4ThreeVector& itsRepeatVector = G4ThreeVector(),
                           G4bool itsFlagAutoCenter = true);
    virtual ~GateFocusArrayRepeater();

  public:
    virtual void PushMyPlacements(const G4RotationMatrix& currentRotationMatrix,
                                  const G4ThreeVector& currentPosition,
                                  G4double aTime);
    virtual void DescribeMyself(size_t indent);

  public:
    inline const G4ThreeVector& GetRepeatVector()
      { return m_repeatVector; }
    inline G4int GetRepeatNumber(size_t axis)
      { return m_repeatNumber[axis]; }
    inline G4int GetRepeatNumberX()
      { return GetRepeatNumber(0); }
    inline G4int GetRepeatNumberY()
      { return GetRepeatNumber(1); }
    inline G4int GetRepeatNumberZ()
      { return GetRepeatNumber(2); }
    inline G4bool GetAutoCenterFlag()
      { return m_flagAutoCenter; }
    inline const G4String& GetPlacementsFilename() const
      { return m_placementsFilename; }

    void SetRepeatVector(const G4ThreeVector& val)
      { m_repeatVector = val; }
    void SetRepeatNumber(size_t axis, G4int number)
      { m_repeatNumber[axis] = number; }
    void SetRepeatNumberX(G4int number)
      { SetRepeatNumber(0, number); }
    void SetRepeatNumberY(G4int number)
      { SetRepeatNumber(1, number); }
    void SetRepeatNumberZ(G4int number)
      { SetRepeatNumber(2, number); }
    void SetAutoCenterFlag(G4bool val)
      { m_flagAutoCenter = val; }
    void SetPlacementsFilename(const G4String& val)
      { m_placementsFilename = val; }

  protected:
    G4ThreeVector m_repeatVector;
    G4int m_repeatNumber[3];
    G4bool m_flagAutoCenter;
    G4String m_placementsFilename;

    GateFocusArrayRepeaterMessenger* m_Messenger;
};

#endif
