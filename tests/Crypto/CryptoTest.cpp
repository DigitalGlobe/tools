//-----------------------------------------------------------------------------
/* 
 * CryptoTest.cpp
 */
//-----------------------------------------------------------------------------

#include "CryptoTest.h"
#include <hex.h>
#include <string>

using namespace CryptoPP;
using namespace std;

//-----------------------------------------------------------------------------
// global variables

    //-------------------------------------------------------------------------
    /** the name of the Crypto executable file */
    extern string g_sCryptoExecutableFileName;
    //-------------------------------------------------------------------------
    /** the name of the test input file */
    extern string g_sInputFileName;
    //-------------------------------------------------------------------------
    /** the name of the resulting output file */
    extern string g_sOutputFileName;
    //-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CCryptoTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CCryptoTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CCryptoTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests encryption.  The code for this class derives from
 *     <a href="http://www.cryptopp.com/wiki/HexEncoder" />.
 */
void CCryptoTest::testEncryption()
{
    const byte DECODED[] = { 0xFF ,
                             0xEE ,
                             0xDD ,
                             0xCC ,
                             0xBB ,
                             0xAA ,
                             0x99 ,
                             0x88 ,
                             0x77 ,
                             0x66 ,
                             0x55 ,
                             0x44 ,
                             0x33 ,
                             0x22 ,
                             0x11 ,
                             0x00 };
    HexEncoder encoder;
    string     sEncoded;
    word64     wSize = 0LL;

    encoder.Put( DECODED         ,
                 sizeof(DECODED) );
    encoder.MessageEnd();
    wSize = encoder.MaxRetrievable();
    if (wSize > 0)
    {
        sEncoded.resize( static_cast<unsigned int>(wSize) );
        encoder.Get( ( (byte*) ( sEncoded.data() ) ) ,
                     sEncoded.size()                 );
    }
    CPPUNIT_ASSERT( 0 == ::strcmp( "FFEEDDCCBBAA99887766554433221100" ,
                                   sEncoded.c_str()                   ) );
}
//-----------------------------------------------------------------------------
