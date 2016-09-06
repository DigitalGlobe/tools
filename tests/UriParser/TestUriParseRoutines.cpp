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
#include "UriParseRoutines.h"
using namespace CppUnit;
using namespace std;


class TestUriParseRoutines : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestUriParseRoutines);//declare the test suite
	CPPUNIT_TEST(testUriGetOctetValue);//test the get octet method
	CPPUNIT_TEST(testUriPushToStack);
	CPPUNIT_TEST_SUITE_END();//end of test suite declaration

public:

	void setUp(void);
	void tearDown(void);

protected:
	void testUriGetOctetValue(void);
	void testUriPushToStack(void);

private:
	
	UriParseRoutines *ParserObject;
};

//Register for running the test

CPPUNIT_TEST_SUITE_REGISTRATION(TestUriParseRoutines);//most important


void TestUriParseRoutines::testUriGetOctetValue(void)
{
	unsigned char digitHistory[4];
	digitHistory[0] = 1;
	digitHistory[1] = 168;
	digitHistory[2] = 90;
	digitHistory[3] = 1;
	unsigned char c = 1;
	int i = 0;
	CPPUNIT_ASSERT(c == ParserObject->uriGetOctetValue(digitHistory, 1));
}

void TestUriParseRoutines::testUriPushToStack(void)
{
	UriIp4Parser * parser = new UriIp4Parser();
	parser->stackCount = 1;
	parser->stackOne = 3;
	unsigned char digit = 1;
	unsigned char c = 3;
	CPPUNIT_ASSERT(c == ParserObject->uriPushToStack(parser, digit));
}


void TestUriParseRoutines::setUp(void)
{
	ParserObject = new UriParseRoutines();
}

void TestUriParseRoutines::tearDown(void)
{
	delete ParserObject;
}
