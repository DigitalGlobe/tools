//-----------------------------------------------------------------------------
/* 
 * NewmatTest.cpp
 */
//-----------------------------------------------------------------------------

#include "NewmatTest.h"
#include <newmatap.h>
#include <math.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// class constants

    const Real CNewmatTest::DELTA_REAL(0.0001);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CNewmatTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CNewmatTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests linear regression.
 *
 * @throws  CTestException  if the test failed
 */
void CNewmatTest::testRegression() throw (CTestException)
{
    const Real      X1[]              = { 2.4, 1.8, 2.4, 3.0, 2.0, 1.2, 2.0, 2.7, 3.6 };
    const Real      X2[]              = { 1.7, 0.9, 1.6, 1.9, 0.5, 0.6, 1.1, 1.0, 0.5 };
    const Real      Y[]               = { 8.3, 5.5, 8.0, 8.5, 5.7, 4.4, 6.3, 7.9, 9.1 };
    const int       COUNT_OBSERVATION = 9;
    const int       COUNT_PREDICTOR   = 2;
    ColumnVector    y(COUNT_OBSERVATION);
    Matrix          x( COUNT_OBSERVATION     ,
                       (COUNT_PREDICTOR + 1) );
    SymmetricMatrix sumOfSquares;

    // load X1 and X2 into X
    x.column(1) = 1.0;
    x.column(2) << X1;
    x.column(3) << X2;

    // vector of Y values
    y << Y;

    // form sum of squares and product matrix
    sumOfSquares << ( x.t() * x );

    // calculate estimate
    ColumnVector estimate = ( sumOfSquares.i() *
                              ( x.t() * y )    );
    this->assertTrue( 3 == estimate.nrows() );
    this->assertTrue( this->areApproximatelyEqual( 1.391655    ,
                                                   estimate(1) ) );
    this->assertTrue( this->areApproximatelyEqual( 1.983103    ,
                                                   estimate(2) ) );
    this->assertTrue( this->areApproximatelyEqual( 0.952208    ,
                                                   estimate(3) ) );
}
//-----------------------------------------------------------------------------
/**
 * Determines whether specified values are approximately equal.
 *
 * @param   firstValue   the first value to compare
 * @param   secondValue  the second value to compare
 * @return  <code>true</code> if the spec
 */
bool CNewmatTest::areApproximatelyEqual( const Real& firstValue  ,
                                         const Real& secondValue ) const
{
    return ( ::fabs( firstValue  -
                     secondValue ) <= CNewmatTest::DELTA_REAL );
}
//-----------------------------------------------------------------------------
/**
 * Checks that a specified condition is <code>true</code>.
 *
 * @param   bCondition  the condition to check
 * @throws  CTestException  if the specified condition is <code>false</code>
 */
void CNewmatTest::assertTrue(bool bCondition) const throw (CTestException)
{
    if (!bCondition)
    {
        throw CTestException();
    }
}
//-----------------------------------------------------------------------------
