#include <iostream>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include "GoogleTestRoutines.h"
using namespace CppUnit;
using namespace std;


class TestGoogleTestRoutines : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestGoogleTestRoutines);//declare the test suite
	CPPUNIT_TEST(testGoogleFactorial);//
	CPPUNIT_TEST(testGoogleIsPrime);//
	CPPUNIT_TEST_SUITE_END();//end of test suite declaration

public:

	void setUp(void);
	void tearDown(void);

protected:
	void testGoogleFactorial(void);
	void testGoogleIsPrime(void);

private:

	GoogleTestRoutines *ParserObject;
};

//Register for running the test

CPPUNIT_TEST_SUITE_REGISTRATION(TestGoogleTestRoutines);//most important


void TestGoogleTestRoutines::testGoogleFactorial(void)
{
	int c = 120;

	//any non-zero return value indicates success
	CPPUNIT_ASSERT(c == ParserObject->Factorial(5));
}

void TestGoogleTestRoutines::testGoogleIsPrime(void)
{
	bool isPrimeVal = true;

	//any non-zero return value indicates success
	CPPUNIT_ASSERT(isPrimeVal == ParserObject->IsPrime(11));
}


void TestGoogleTestRoutines::setUp(void)
{
	ParserObject = new GoogleTestRoutines();
}

void TestGoogleTestRoutines::tearDown(void)
{
	delete ParserObject;
}
