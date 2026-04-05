/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/

#ifndef GateCoincidenceMultiplesKillerMessenger_h
#define GateCoincidenceMultiplesKillerMessenger_h 1
#include "G4UImessenger.hh"
#include "globals.hh"

#include "GateClockDependentMessenger.hh"


class G4UIdirectory;

class GateCoincidenceMultiplesKiller;

class GateCoincidenceMultiplesKillerMessenger: public GateClockDependentMessenger
{
public:
  GateCoincidenceMultiplesKillerMessenger(GateCoincidenceMultiplesKiller*);
  virtual ~GateCoincidenceMultiplesKillerMessenger();

  inline void SetNewValue(G4UIcommand* aCommand, G4String aString);

  //inline GateCoincidenceMultiplesKiller* GetMultiplesKiller(){ return (GateCoincidenceMultiplesKiller*) GetClockDependent(); }

private:


  GateCoincidenceMultiplesKiller* m_CoincidenceMultiplesKiller;


};

#endif
