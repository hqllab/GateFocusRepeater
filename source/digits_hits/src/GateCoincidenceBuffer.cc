/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/


#include "G4UnitsTable.hh"
#include "GateTools.hh"
#include "GateVolumeID.hh"
#include "GateOutputVolumeID.hh"
#include "GateVVolume.hh"
#include "GateDetectorConstruction.hh"
#include "GateCrystalSD.hh"
#include "GateVSystem.hh"
#include "GateObjectChildList.hh"
#include "GateMaps.hh"
#include "GateApplicationMgr.hh"
#include <math.h>
#include "GateCoincidenceBuffer.hh"
#include "GateCoincidenceBufferMessenger.hh"


#include "GateDigi.hh"
#include "GateCoincidenceDigi.hh"

#include "GateDigitizerMgr.hh"
#include "G4SystemOfUnits.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4ios.hh"




GateCoincidenceBuffer::GateCoincidenceBuffer(GateCoincidenceDigitizer *digitizer, G4String name)
  :GateVDigitizerModule(name,"digitizerMgr/CoincidenceDigitizer/"+digitizer->m_digitizerName+"/"+name, digitizer),
   m_bufferSize(1)
  , m_bufferPos(0)
  , m_oldClock(0)
  , m_readFrequency(1),
   m_outputDigi(0),
 m_OutputDigiCollection(0),
 m_digitizer(digitizer)
//  , m_doModifyTime(false)
  , m_mode(0)

{	G4String colName = digitizer->GetOutputName() ;
collectionName.push_back(colName);
  m_messenger = new GateCoincidenceBufferMessenger(this);
}




GateCoincidenceBuffer::~GateCoincidenceBuffer()
{
  delete m_messenger;
}


void GateCoincidenceBuffer::Digitize()
{

	G4String digitizerName = m_digitizer->m_digitizerName;
	G4String outputCollName = m_digitizer-> GetOutputName();

	m_OutputDigiCollection = new GateCoincidenceDigiCollection(GetName(),outputCollName); // to create the Digi Collection

G4DigiManager* DigiMan = G4DigiManager::GetDMpointer();


	GateCoincidenceDigiCollection* IDC = 0;
	IDC = (GateCoincidenceDigiCollection*) (DigiMan->GetDigiCollection(m_DCID));

	GateCoincidenceDigi*  inputDigi = new GateCoincidenceDigi();

	std::vector< GateCoincidenceDigi* >* OutputDigiCollectionVector = m_OutputDigiCollection->GetVector ();
	std::vector<GateCoincidenceDigi*>::iterator iter;

	 if (IDC)
	     {
		  G4int n_digi = IDC->entries();

		  //loop over input digits
		  for (G4int i=0;i<n_digi;i++)
		  {
		  inputDigi=(*IDC)[i];


			 buffer_t clock = (buffer_t) ((inputDigi->GetEndTime()-GateApplicationMgr::GetInstance()->GetTimeStart())* m_readFrequency);

			  buffer_t deltaClocks = (m_oldClock<clock)? clock - m_oldClock : 0;


			  switch (m_mode)
			  {
			      case 0 : m_bufferPos = m_bufferPos>deltaClocks ? m_bufferPos-deltaClocks : 0; break;
			      case 1 : if (deltaClocks>0) m_bufferPos=0;break;
			  }
			

			  if (m_bufferPos+1<=m_bufferSize)
			  {m_outputDigi = new GateCoincidenceDigi(*inputDigi);
				 // if (m_doModifyTime)
				  //{
				//	  G4double tme = GateApplicationMgr::GetInstance()->GetTimeStart()+clock/m_readFrequency;

					//  if (m_mode==1) tme += 1./m_readFrequency;
					 // 	  m_outputDigi->SetTime(tme);
			  	   //}
				  m_OutputDigiCollection->insert(m_outputDigi);

				  m_bufferPos++;
				 
			  }
			  m_oldClock = clock;


		  } //loop  over input digits
	    } //IDC
	  else
	    {
	  	  if (nVerboseLevel>1)
	  	  	G4cout << "[GateCoincidenceBuffer::Digitize]: input digi collection is null -> nothing to do\n\n";
	  	    return;
	    }
	  StoreDigiCollection(m_OutputDigiCollection);

	 }

void GateCoincidenceBuffer::DescribeMyself(size_t indent)
{
  G4cout << GateTools::Indent(indent) << "Buffer: " << G4BestUnit(m_bufferSize,"Memory size")
         << "Read @ "<< G4BestUnit(m_readFrequency,"Frequency")<< Gateendl;
}
