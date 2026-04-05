/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/

#include "GateFocusArrayRepeaterMessenger.hh"
#include "GateFocusArrayRepeater.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

GateFocusArrayRepeaterMessenger::GateFocusArrayRepeaterMessenger(
    GateFocusArrayRepeater* itsFocusArrayRepeater)
  : GateObjectRepeaterMessenger(itsFocusArrayRepeater)
{
  G4String cmdName;

  cmdName = GetDirectoryName() + "setRepeatVector";
  SetRepeatVectorCmd = new G4UIcmdWith3VectorAndUnit(cmdName, this);
  SetRepeatVectorCmd->SetGuidance("Set the repetition vector (from the center of one copy to the center of the next one).");
  SetRepeatVectorCmd->SetParameterName("dX", "dY", "dZ", false);
  SetRepeatVectorCmd->SetUnitCategory("Length");

  cmdName = GetDirectoryName() + "setRepeatNumberX";
  SetRepeatNumberXCmd = new G4UIcmdWithAnInteger(cmdName, this);
  SetRepeatNumberXCmd->SetGuidance("Set the number of copies of the object along X.");
  SetRepeatNumberXCmd->SetParameterName("Nx", false);
  SetRepeatNumberXCmd->SetRange("Nx >= 1");

  cmdName = GetDirectoryName() + "setRepeatNumberY";
  SetRepeatNumberYCmd = new G4UIcmdWithAnInteger(cmdName, this);
  SetRepeatNumberYCmd->SetGuidance("Set the number of copies of the object along Y.");
  SetRepeatNumberYCmd->SetParameterName("Ny", false);
  SetRepeatNumberYCmd->SetRange("Ny >= 1");

  cmdName = GetDirectoryName() + "setRepeatNumberZ";
  SetRepeatNumberZCmd = new G4UIcmdWithAnInteger(cmdName, this);
  SetRepeatNumberZCmd->SetGuidance("Set the number of copies of the object along Z.");
  SetRepeatNumberZCmd->SetParameterName("Nz", false);
  SetRepeatNumberZCmd->SetRange("Nz >= 1");

  cmdName = GetDirectoryName() + "autoCenter";
  AutoCenterCmd = new G4UIcmdWithABool(cmdName, this);
  AutoCenterCmd->SetGuidance("Enable or disable auto-centering.");
  AutoCenterCmd->SetParameterName("flag", true);
  AutoCenterCmd->SetDefaultValue(true);

  cmdName = GetDirectoryName() + "setPlacementsFilename";
  SetPlacementsFilenameCmd = new G4UIcmdWithAString(cmdName, this);
  SetPlacementsFilenameCmd->SetGuidance("Set the placements filename used by the focus array repeater.");
  SetPlacementsFilenameCmd->SetParameterName("filename", false);
}

GateFocusArrayRepeaterMessenger::~GateFocusArrayRepeaterMessenger()
{
  delete SetPlacementsFilenameCmd;
  delete AutoCenterCmd;
  delete SetRepeatVectorCmd;
  delete SetRepeatNumberXCmd;
  delete SetRepeatNumberYCmd;
  delete SetRepeatNumberZCmd;
}

void GateFocusArrayRepeaterMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == SetRepeatVectorCmd)
    GetFocusArrayRepeater()->SetRepeatVector(SetRepeatVectorCmd->GetNew3VectorValue(newValue));
  else if (command == SetRepeatNumberXCmd)
    GetFocusArrayRepeater()->SetRepeatNumberX(SetRepeatNumberXCmd->GetNewIntValue(newValue));
  else if (command == SetRepeatNumberYCmd)
    GetFocusArrayRepeater()->SetRepeatNumberY(SetRepeatNumberYCmd->GetNewIntValue(newValue));
  else if (command == SetRepeatNumberZCmd)
    GetFocusArrayRepeater()->SetRepeatNumberZ(SetRepeatNumberZCmd->GetNewIntValue(newValue));
  else if (command == AutoCenterCmd)
    GetFocusArrayRepeater()->SetAutoCenterFlag(AutoCenterCmd->GetNewBoolValue(newValue));
  else if (command == SetPlacementsFilenameCmd)
    GetFocusArrayRepeater()->SetPlacementsFilename(newValue);
  else
    GateObjectRepeaterMessenger::SetNewValue(command, newValue);
}
