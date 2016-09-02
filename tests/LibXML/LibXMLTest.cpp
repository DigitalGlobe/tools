//-----------------------------------------------------------------------------
/* 
 * LibXMLTest.cpp
 */
//-----------------------------------------------------------------------------

#include "LibXMLTest.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

using namespace std;

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CLibXMLTest);

//-----------------------------------------------------------------------------
// class constants

    const char* CLibXMLTest::FILE_NAME_INPUT = "LibXML\\data\\books.xml";

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CLibXMLTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CLibXMLTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests reading a file.  The code for this method derives from:
 * <a href="http://xmlsoft.org/examples/tree1.c" />.
 */
void CLibXMLTest::testReadFile()
{
    xmlDoc*  pDocument    = nullptr;
    xmlNode* pRootElement = nullptr;

    // initialize the library
    LIBXML_TEST_VERSION

    // read the input file
    pDocument = ::xmlReadFile( CLibXMLTest::FILE_NAME_INPUT ,
                               nullptr                      ,
                               0                            );
    CPPUNIT_ASSERT(nullptr != pDocument);

    // get the root element
    pRootElement = ::xmlDocGetRootElement(pDocument);
    CPPUNIT_ASSERT(nullptr != pRootElement);
    CPPUNIT_ASSERT( 0 == ::strcmp( "catalog"                                         ,
                                   reinterpret_cast<const char*>(pRootElement->name) ) );

    // clean up
    ::xmlFreeDoc(pDocument);
    pDocument = nullptr;
    ::xmlCleanupParser();
}
//-----------------------------------------------------------------------------
