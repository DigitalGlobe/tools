//-----------------------------------------------------------------------------
/* 
 * KDISTest.cpp
 */
//-----------------------------------------------------------------------------

#include "KDISTest.h"
#include "TargetPosition.h"
#include "TargetPositionDecoder.h"
#include <KDIS/Extras/PDU_Factory.h>
#include <KDIS/DataTypes/VariableDatum.h>
#include <KDIS/DataTypes/EntityIdentifier.h>
#include <KDIS/DataTypes/WorldCoordinates.h>
#include <KDIS/PDU/Simulation_Management/Set_Data_PDU.h>

using namespace std;

using namespace KDIS;
using namespace DATA_TYPE;
using namespace PDU;
using namespace UTILS;

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CKDISTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CKDISTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CKDISTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests reading an KDIS file.
 */
void CKDISTest::testRead()
{
    CTargetPosition* pTargetPosition = new CTargetPosition( EntityIdentifier( 1 ,
                                                                              2 ,
                                                                              3 ) ,
                                                            WorldCoordinates( 3 ,
                                                                              2 ,
                                                                              1 ) );
    PDU_Factory      pduFactory;
    Set_Data_PDU     pduData;

    VariableDatum::RegisterFactoryDecoder( CTargetPosition::DEFAULT_DATUM_ID                        ,
                                           VariableDatum::FacDecPtr( new CTargetPositionDecoder() ) );
    pduData.AddVariableDatum(pTargetPosition);
    KDataStream      stream = pduData.Encode();
    auto_ptr<Header> pdu    = pduFactory.Decode(stream);
    if ( pdu.get() )
    {
        Set_Data_PDU*    pPduRecord      = static_cast<Set_Data_PDU*>( pdu.get() );
        VarDtmPtr        pItem           = pPduRecord->GetVariableDatum()[0];
        CTargetPosition* pTargetPosition = reinterpret_cast<CTargetPosition*>( static_cast<VariableDatum*>(pItem) );
        
        CPPUNIT_ASSERT(pTargetPosition != nullptr);
        CPPUNIT_ASSERT_EQUAL( 56U                                              ,
                              static_cast<unsigned int>( pdu->GetPDULength() ) );
        VariableDatum::ClearFactoryDecoders();
    }
    else
    {
        VariableDatum::ClearFactoryDecoders();
        CPPUNIT_ASSERT(false);
    }
}
//-----------------------------------------------------------------------------
