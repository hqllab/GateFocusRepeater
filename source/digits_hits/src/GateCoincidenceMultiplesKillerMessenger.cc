/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/







#include "GateCoincidenceMultiplesKillerMessenger.hh"
#include "GateCoincidenceMultiplesKiller.hh"
#include "GateDigitizerMgr.hh"
#include "G4UImessenger.hh"
#include "globals.hh"

#include "GateClockDependentMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"

GateCoincidenceMultiplesKillerMessenger::GateCoincidenceMultiplesKillerMessenger(GateCoincidenceMultiplesKiller* CoincidenceMultiplesKiller)
    : GateClockDependentMessenger(CoincidenceMultiplesKiller),
      m_CoincidenceMultiplesKiller(CoincidenceMultiplesKiller)
{
    G4String guidance;
    G4String cmdName;


}

GateCoincidenceMultiplesKillerMessenger::~GateCoincidenceMultiplesKillerMessenger()
{

}

void GateCoincidenceMultiplesKillerMessenger::SetNewValue(G4UIcommand* aCommand, G4String newValue)
{

	}
