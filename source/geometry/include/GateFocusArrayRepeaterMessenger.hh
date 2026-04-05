/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/

#ifndef GateFocusArrayRepeaterMessenger_h
#define GateFocusArrayRepeaterMessenger_h 1

#include "globals.hh"
#include "GateObjectRepeaterMessenger.hh"

class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

class GateFocusArrayRepeater;

class GateFocusArrayRepeaterMessenger : public GateObjectRepeaterMessenger
{
  public:
    GateFocusArrayRepeaterMessenger(GateFocusArrayRepeater* itsFocusArrayRepeater);
    ~GateFocusArrayRepeaterMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  public:
    inline GateFocusArrayRepeater* GetFocusArrayRepeater()
      { return (GateFocusArrayRepeater*)GetObjectRepeater(); }

  private:
    G4UIcmdWithAnInteger* SetRepeatNumberXCmd;
    G4UIcmdWithAnInteger* SetRepeatNumberYCmd;
    G4UIcmdWithAnInteger* SetRepeatNumberZCmd;
    G4UIcmdWith3VectorAndUnit* SetRepeatVectorCmd;
    G4UIcmdWithABool* AutoCenterCmd;
    G4UIcmdWithAString* SetPlacementsFilenameCmd;
};

#endif
