#include <iostream>


#include "../../src/CppUnit/include/cppunit/ui/text/TextTestRunner.h"
#include "../../src/CppUnit/include/cppunit/extensions/HelperMacros.h"
#include "../../src/CppUnit/include/cppunit/extensions/TestFactoryRegistry.h"
#include "../../src/CppUnit/include/cppunit/TestResult.h"
#include "../../src/CppUnit/include/cppunit/TestResultCollector.h"
#include "../../src/CppUnit/include/cppunit/TestRunner.h"
#include "../../src/CppUnit/include/cppunit/BriefTestProgressListener.h"
#include "../../src/CppUnit/include/cppunit/CompilerOutputter.h"
#include "../../src/CppUnit/include/cppunit/XmlOutputter.h"
#include "QWTRoutines.h"
#include <cppunit/Portability.h>
using namespace CppUnit;
using namespace std;


class TestQWTRoutines : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestQWTRoutines);//declare the test suite
    CPPUNIT_TEST(testQWTTest);//t
	CPPUNIT_TEST_SUITE_END();//end of test suite declaration

public:

	void setUp(void);
	void tearDown(void);

protected:
    void testQWTTest(void);

private:

    QWTRoutines *ParserObject;
};

//Register for running the test

CPPUNIT_TEST_SUITE_REGISTRATION(TestQWTRoutines);//most important


void TestQWTRoutines::testQWTTest(void)
{
	int c = 0;

	int argc = 1;
	char *argv[1] = { (char*)"GlutTesting" };

	//any non-zero return value indicates success
    CPPUNIT_ASSERT(c == ParserObject->QWTTest(argc, argv));

	
}


void TestQWTRoutines::setUp(void)
{
    ParserObject = new QWTRoutines();
}

void TestQWTRoutines::tearDown(void)
{
	delete ParserObject;
}
