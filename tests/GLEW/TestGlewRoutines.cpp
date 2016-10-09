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
#include "GlewRoutines.h"
using namespace CppUnit;
using namespace std;


class TestGlewRoutines : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestGlewRoutines);//declare the test suite
	CPPUNIT_TEST(testGlewTest);//t
	CPPUNIT_TEST_SUITE_END();//end of test suite declaration

public:

	void setUp(void);
	void tearDown(void);

protected:
	void testGlewTest(void);

private:

	GlewRoutines *ParserObject;
};

//Register for running the test

CPPUNIT_TEST_SUITE_REGISTRATION(TestGlewRoutines);//most important


void TestGlewRoutines::testGlewTest(void)
{
	int c = 0;

	int argc = 1;
	char *argv[1] = { (char*)"GlewTesting" };

	//any non-zero return value indicates success
	CPPUNIT_ASSERT(c == ParserObject->GlewTest(argc, argv));
}


void TestGlewRoutines::setUp(void)
{
	ParserObject = new GlewRoutines();
}

void TestGlewRoutines::tearDown(void)
{
	delete ParserObject;
}
