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
    G4UIcmdWithAString* SetPlacementsFilenameCmd;
};

#endif
