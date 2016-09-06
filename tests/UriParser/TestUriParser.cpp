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


class TestUriParser : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestUriParser);//declare the test suite
	CPPUNIT_TEST(testUriWriteQuadToDoubleByte);
	CPPUNIT_TEST(testUriGetOctetValue);//test the multiplication method
	CPPUNIT_TEST_SUITE_END();//end of test suite declaration

public:

	void setUp(void);
	void tearDown(void);

protected:
	void testUriWriteQuadToDoubleByte(void);
	void testUriGetOctetValue(void);

private:

	TestUriParser *ParserObject﻿;

};

//Register for running the test

CPPUNIT_TEST_SUITE_REGISTRATION(TestUriParser);//most important


void TestUriParser::testUriWriteQuadToDoubleByte(void)
{
	CPPUNIT_ASSERT(ParserObject﻿->uriWriteQuadToDoubleByte("A2", 3));
}

void TestUriParser::testUriGetOctetValue(void)
{
	CPPUNIT_ASSERT(6 == ParserObject﻿->uriGetOctetValue(2, 3));
}


void TestUriParser::setUp(void)
{
	ParserObject﻿ = new TestUriParser();
}

void TestUriParser::tearDown(void)
{
	delete ParserObject﻿;
}
