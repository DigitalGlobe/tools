//-----------------------------------------------------------------------------
/* 
 * FreeTypeTest.cpp
 */
//-----------------------------------------------------------------------------

#include "FreeTypeTest.h"

#include <ft2build.h>
#include FT_FREETYPE_H

//-----------------------------------------------------------------------------
// class constants

    const char* CFreeTypeTest::FILE_NAME_INPUT = "FreeType\\data\\times.ttf";

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CFreeTypeTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CFreeTypeTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CFreeTypeTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests reading a file.  This method derives from:
 *     <a href="https://www.freetype.org/freetype2/docs/tutorial/step1.html">FreeType Tutorial / I</a>.
 */
void CFreeTypeTest::testReadFile()
{
    FT_Library freeTypeLibrary;
    FT_Error   iDestroyResult        = 0;
    FT_Error   iInitializationResult = 0;
    FT_Error   iFaceDoneResult       = 0;
    FT_Error   iFaceNewResult        = 0;
    FT_Face    pFace                 = nullptr;

    // initialize FreeType
    iInitializationResult = ::FT_Init_FreeType(&freeTypeLibrary);
    CPPUNIT_ASSERT_EQUAL( 0                     ,
                          iInitializationResult );

    // open font
    iFaceNewResult = ::FT_New_Face(  freeTypeLibrary                ,
                                     CFreeTypeTest::FILE_NAME_INPUT ,
                                     0                              ,
                                    &pFace                          );
    CPPUNIT_ASSERT_EQUAL( 0              ,
                          iFaceNewResult );

    // close font
    iFaceDoneResult = ::FT_Done_Face(pFace);
    pFace           = nullptr;
    CPPUNIT_ASSERT_EQUAL( 0               ,
                          iFaceDoneResult );

    // deinitialize FreeType
    iDestroyResult = ::FT_Done_FreeType(freeTypeLibrary);
    CPPUNIT_ASSERT_EQUAL( 0              ,
                          iDestroyResult );
}
//-----------------------------------------------------------------------------
