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
#include "ExpatRoutines.h"
using namespace CppUnit;
using namespace std;


class TestExpatRoutines : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestExpatRoutines);//declare the test suite
	CPPUNIT_TEST(testParseXMLfile);//test the get octet method
	CPPUNIT_TEST_SUITE_END();//end of test suite declaration

public:

	void setUp(void);
	void tearDown(void);

protected:
	void testParseXMLfile(void);

private:

	ExpatRoutines *ParserObject;
};

//Register for running the test

CPPUNIT_TEST_SUITE_REGISTRATION(TestExpatRoutines);//most important


void TestExpatRoutines::testParseXMLfile(void)
{
	int c = 0;

	//any non-zero return value indicates success
	CPPUNIT_ASSERT(c == ParserObject->parseXMLfile());

	if (c > 0) {
		printf("%s", "Successful Test");
	}
	
}


void TestExpatRoutines::setUp(void)
{
	ParserObject = new ExpatRoutines();
}

void TestExpatRoutines::tearDown(void)
{
	delete ParserObject;
}
