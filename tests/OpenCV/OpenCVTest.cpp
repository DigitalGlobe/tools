//-----------------------------------------------------------------------------
/* 
 * OpenCVTest.cpp
 */
//-----------------------------------------------------------------------------

#include "OpenCVTest.h"
#include <opencv2/imgproc.hpp>
#include <vector>

#include <iostream>



using namespace cv;

using namespace std;


//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(COpenCVTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void COpenCVTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void COpenCVTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests convex hulls.  This method derives from:
 *     <a href="http://docs.opencv.org/master/d0/d7a/convexhull_8cpp-example.html#gsc.tab=0">convexhull.cpp</a>.
 */
void COpenCVTest::testConvexHull()
{
    vector<Point> convexHullPoints;
    vector<Point> points;

    // populate points
    points.push_back( Point(272, 214) );
    points.push_back( Point(254, 372) );  
    points.push_back( Point(185, 161) );
    points.push_back( Point(368, 251) );
    points.push_back( Point(154, 126) );
    points.push_back( Point(163, 132) );
    points.push_back( Point(313, 150) );

    // get the convex hull
    ::convexHull( points           ,
                  convexHullPoints ,
                  true             );
    CPPUNIT_ASSERT_EQUAL( 4                                           ,
                          static_cast<int>( convexHullPoints.size() ) );
    CPPUNIT_ASSERT_EQUAL( 154                   ,
                          convexHullPoints[0].x );
    CPPUNIT_ASSERT_EQUAL( 126                   ,
                          convexHullPoints[0].y );
    CPPUNIT_ASSERT_EQUAL( 254                   ,
                          convexHullPoints[1].x );
    CPPUNIT_ASSERT_EQUAL( 372                   ,
                          convexHullPoints[1].y );
    CPPUNIT_ASSERT_EQUAL( 368                   ,
                          convexHullPoints[2].x );
    CPPUNIT_ASSERT_EQUAL( 251                   ,
                          convexHullPoints[2].y );
    CPPUNIT_ASSERT_EQUAL( 313                   ,
                          convexHullPoints[3].x );
    CPPUNIT_ASSERT_EQUAL( 150                   ,
                          convexHullPoints[3].y );
}
//-----------------------------------------------------------------------------
