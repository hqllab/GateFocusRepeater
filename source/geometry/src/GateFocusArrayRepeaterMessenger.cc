/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/

#include "GateFocusArrayRepeaterMessenger.hh"
#include "GateFocusArrayRepeater.hh"

#include "G4UIcmdWithAString.hh"

GateFocusArrayRepeaterMessenger::GateFocusArrayRepeaterMessenger(
    GateFocusArrayRepeater* itsFocusArrayRepeater)
  : GateObjectRepeaterMessenger(itsFocusArrayRepeater)
{
  G4String cmdName;

  cmdName = GetDirectoryName() + "setPlacementsFilename";
  SetPlacementsFilenameCmd = new G4UIcmdWithAString(cmdName, this);
  SetPlacementsFilenameCmd->SetGuidance("Set the placements filename used by the focus array repeater.");
  SetPlacementsFilenameCmd->SetParameterName("filename", false);
}

GateFocusArrayRepeaterMessenger::~GateFocusArrayRepeaterMessenger()
{
  delete SetPlacementsFilenameCmd;
}

void GateFocusArrayRepeaterMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == SetPlacementsFilenameCmd)
    GetFocusArrayRepeater()->SetPlacementsFilename(newValue);
  else
    GateObjectRepeaterMessenger::SetNewValue(command, newValue);
}
