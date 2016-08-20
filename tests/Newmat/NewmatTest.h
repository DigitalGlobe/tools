//-----------------------------------------------------------------------------
/*
 * CNewmatTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include "TestException.h"
#include <newmatap.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CNewmatTest</code> class represents test fixtures for the Newmat
 * library.
 */
class CNewmatTest
{
    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testRegression() throw (CTestException);

    private:

        //---------------------------------------------------------------------
        // private constants

            //-----------------------------------------------------------------
            /** the maximum difference between which two real values are
                approximately equal */
            static const Real DELTA_REAL;
            //-----------------------------------------------------------------

        //---------------------------------------------------------------------
        // private methods

            bool areApproximatelyEqual( const Real& firstValue  ,
                                        const Real& secondValue ) const;
            void assertTrue(bool bCondition) const throw (CTestException);

};
//-----------------------------------------------------------------------------
