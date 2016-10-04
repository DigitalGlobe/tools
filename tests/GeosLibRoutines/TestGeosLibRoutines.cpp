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
#include "GeosLibRoutines.h"
using namespace CppUnit;
using namespace std;


class TestGeosLibRoutines : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestGeosLibRoutines);//declare the test suite
	CPPUNIT_TEST(testGeosCreatePoint);//
	CPPUNIT_TEST(testGeosCreateRectangle);//
	CPPUNIT_TEST_SUITE_END();//end of test suite declaration

public:

	void setUp(void);
	void tearDown(void);

protected:
	void testGeosCreatePoint(void);
	void testGeosCreateRectangle(void);

private:

	GeosLibRoutines *ParserObject;
};

//Register for running the test

CPPUNIT_TEST_SUITE_REGISTRATION(TestGeosLibRoutines);//most important


void TestGeosLibRoutines::testGeosCreatePoint()
{
	int c = 0;

	double x = 10;
	double y = 15;

	//any non-zero return value indicates success
	CPPUNIT_ASSERT(c == ParserObject->createPoint(x, y));
}

void TestGeosLibRoutines::testGeosCreateRectangle()
{
	int c = 0;

	double x = 0;
	double y = 10;
	double width = 10;
	double height = 0;

	//any non-zero return value indicates success
	CPPUNIT_ASSERT(c == ParserObject->createRectangle(x, y, width, height));
}


void TestGeosLibRoutines::setUp(void)
{
	ParserObject = new GeosLibRoutines();
}

void TestGeosLibRoutines::tearDown(void)
{
	delete ParserObject;
}
