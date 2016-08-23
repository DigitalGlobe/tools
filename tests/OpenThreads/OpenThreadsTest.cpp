//-----------------------------------------------------------------------------
/* 
 * OpenThreadsTest.cpp
 */
//-----------------------------------------------------------------------------

#include "OpenThreadsTest.h"
#include "TestThread.h"

using namespace OpenThreads;
using namespace std;

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(COpenThreadsTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void COpenThreadsTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void COpenThreadsTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests threading.
 */
void COpenThreadsTest::testThreading()
{
    const int THREADS_COUNT = 2;

    // initialize OpenThreads
    OpenThreads::Thread::Init();

    // create and start the threads
    CTestThread aThreads[THREADS_COUNT];
    for ( int iThreadIndex = 0         ;
          iThreadIndex < THREADS_COUNT ;
          ++iThreadIndex               )
    {
        aThreads[iThreadIndex].start();
    }

    // wait for the threads to finish
    for ( int iThreadIndex = 0         ;
          iThreadIndex < THREADS_COUNT ;
          ++iThreadIndex               )
    {
        aThreads[iThreadIndex].join();
    }
}
//-----------------------------------------------------------------------------
