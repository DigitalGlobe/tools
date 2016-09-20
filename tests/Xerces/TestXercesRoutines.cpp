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
#include "XercesRoutines.h"
using namespace CppUnit;
using namespace std;


class TestXercesRoutines : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestXercesRoutines);//declare the test suite
	CPPUNIT_TEST(testXercesTest);//t
	CPPUNIT_TEST_SUITE_END();//end of test suite declaration

public:

	void setUp(void);
	void tearDown(void);

protected:
	void testXercesTest(void);

private:

	XercesRoutines *ParserObject;
};

//Register for running the test

CPPUNIT_TEST_SUITE_REGISTRATION(TestXercesRoutines);//most important


void TestXercesRoutines::testXercesTest(void)
{
	int c = 0;

	//any non-zero return value indicates success
	CPPUNIT_ASSERT(c == ParserObject->XercesTest());
}


void TestXercesRoutines::setUp(void)
{
	ParserObject = new XercesRoutines();
}

void TestXercesRoutines::tearDown(void)
{
	delete ParserObject;
}
