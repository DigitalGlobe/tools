//-----------------------------------------------------------------------------
/* 
 * PROJ4Test.cpp
 */
//-----------------------------------------------------------------------------

#include "PROJ4Test.h"
#include <proj_api.h>

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CPROJ4Test);

//-----------------------------------------------------------------------------
// class constants

    const double CPROJ4Test::DELTA_DOUBLE = 0.01;

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CPROJ4Test::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CPROJ4Test::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests using latitude and longitude values in decimal degress, performing
 * Mercator projection with a Clarke 1866 ellipsoid and a 33-degree latitude
 * of true scale, and calculating the projected Cartesian values in meters.
 * The code for this method derives from:
 *     <a href="https://proj4.haxx.se/libproj4/c/simple.html">PROJ API Reference</a>.
 */
void CPROJ4Test::testConversion()
{
    const int COUNT_POINT                  = 1;
    const int OFFSET_POINT                 = 1;
    double    dX                           = (-16.00 * DEG_TO_RAD);
    double    dY                           = ( 20.25 * DEG_TO_RAD);
    int       iTransformResult             = 0;
    projPJ    pMercatorProjection          = nullptr;
    projPJ    pLatitudeLongitudeProjection = nullptr;

    // initialize Mercator
    pMercatorProjection = ::pj_init_plus("+proj=merc +ellps=clrk66 +lat_ts=33");
    CPPUNIT_ASSERT(nullptr != pMercatorProjection);

    // initialize latitude/longitude
    pLatitudeLongitudeProjection = ::pj_init_plus("+proj=latlong +ellps=clrk66");
    CPPUNIT_ASSERT(nullptr != pLatitudeLongitudeProjection);

    // calculate the Cartesian values
    iTransformResult = ::pj_transform(  pLatitudeLongitudeProjection ,
                                        pMercatorProjection          ,
                                        COUNT_POINT                  ,
                                        OFFSET_POINT                 ,
                                       &dX                           ,
                                       &dY                           ,
                                        nullptr                      );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -1495284.21              ,
                                  dX                       ,
                                  CPROJ4Test::DELTA_DOUBLE );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1920596.79               ,
                                  dY                       ,
                                  CPROJ4Test::DELTA_DOUBLE );
}
//-----------------------------------------------------------------------------
