/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/



#include "GateCoincidenceMultiplesKillerMessenger.hh"
#include "GateCoincidenceMultiplesKiller.hh"
#include "GateTools.hh"

#include "GateCoincidenceDigi.hh"

#include "GateDigitizerMgr.hh"
#include "G4SystemOfUnits.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"


GateCoincidenceMultiplesKiller::GateCoincidenceMultiplesKiller(GateCoincidenceDigitizer* digitizer, G4String name)
    : GateVDigitizerModule(name, "digitizerMgr/CoincidenceDigitizer/" + digitizer->m_digitizerName + "/" + name, digitizer),
      m_outputDigi(0),
      m_OutputDigiCollection(0),
      m_digitizer(digitizer)
{
     G4String colName = digitizer->GetOutputName();
     collectionName.push_back(colName);

     m_messenger = new GateCoincidenceMultiplesKillerMessenger(this);
}

GateCoincidenceMultiplesKiller::~GateCoincidenceMultiplesKiller()
{
    delete m_messenger;
}
void GateCoincidenceMultiplesKiller::Digitize() {
  
	G4String digitizerName = m_digitizer->m_digitizerName;
	G4String outputCollName = m_digitizer-> GetOutputName();

	m_OutputDigiCollection = new GateCoincidenceDigiCollection(GetName(),outputCollName); // to create the Digi Collection

	G4DigiManager* DigiMan = G4DigiManager::GetDMpointer();
	GateCoincidenceDigiCollection* IDC = 0;
	IDC = (GateCoincidenceDigiCollection*) (DigiMan->GetDigiCollection(m_DCID));
	
	GateCoincidenceDigi* inputDigi = new GateCoincidenceDigi();



    if (IDC) {

    G4int n_digi = IDC->entries();
 
    for (G4int i = 0; i < n_digi; i++) {
        inputDigi = (*IDC)[i];

        if (inputDigi->size() == 2) {
            m_outputDigi = new GateCoincidenceDigi(*inputDigi);
            m_OutputDigiCollection->insert(m_outputDigi);

        } else {
            if (nVerboseLevel > 1) {
                G4cout << "[GateCoincidenceMultiplesKiller::Digitize]: input digi was null or size != 2 -> killing this digi\n\n";
            }
          
        }

  
        }
    } else
    {
  	  if (nVerboseLevel>1)
  	  	G4cout << "[GateCoincidenceDeadTime::Digitize]: input digi collection is null -> nothing to do\n\n";
  	    return;
    }


    StoreDigiCollection(m_OutputDigiCollection);
}

void GateCoincidenceMultiplesKiller::DescribeMyself(size_t indent)
{
    G4cout << GateTools::Indent(indent) << "MultiplesKiller " << Gateendl;
}

