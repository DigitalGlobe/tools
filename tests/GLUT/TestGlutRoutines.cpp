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
#include "GlutRoutines.h"
using namespace CppUnit;
using namespace std;


class TestGlutRoutines : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestGlutRoutines);//declare the test suite
	CPPUNIT_TEST(testGlutTest);//t
	CPPUNIT_TEST_SUITE_END();//end of test suite declaration

public:

	void setUp(void);
	void tearDown(void);

protected:
	void testGlutTest(void);

private:

	GlutRoutines *ParserObject;
};

//Register for running the test

CPPUNIT_TEST_SUITE_REGISTRATION(TestGlutRoutines);//most important


void TestGlutRoutines::testGlutTest(void)
{
	int c = 0;

	int argc = 1;
	char *argv[1] = { (char*)"GlutTesting" };

	//any non-zero return value indicates success
	CPPUNIT_ASSERT(c == ParserObject->GlutTest(argc, argv));

	
}


void TestGlutRoutines::setUp(void)
{
	ParserObject = new GlutRoutines();
}

void TestGlutRoutines::tearDown(void)
{
	delete ParserObject;
}
