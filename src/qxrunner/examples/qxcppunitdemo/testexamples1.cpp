// This class implements a set of CppUnit example test cases which
// succeed or fail. It isn't intended to show how to write CppUnit tests.

//#include "../../CppUnit/include/cppunit/TestFixture.h"
#include "../../CppUnit/include/cppunit/extensions/HelperMacros.h"
#include "../../CppUnit/include/cppunit/Test.h"
//#include "../../CppUnit/include/cppunit/Portability.h"

class TestExamples1;

CPPUNIT_TEST_SUITE_REGISTRATION(TestExamples1);

class TestExamples1 : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TestExamples1);

	CPPUNIT_TEST(testAssertEqualSucceed);
	CPPUNIT_TEST(testAssertEqualFail);
	CPPUNIT_TEST(testAssertDoublesEqualSucceed);
	CPPUNIT_TEST(testAssertDoublesEqualFail);

	CPPUNIT_TEST_SUITE_END();

	void testAssertEqualSucceed()
	{
		CPPUNIT_ASSERT_EQUAL(1, 1);
	}

	void testAssertEqualFail()
	{
		CPPUNIT_ASSERT_EQUAL(1, 0);
	}

	void testAssertDoublesEqualSucceed()
	{
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, 1.1, 0.5);
	}

	void testAssertDoublesEqualFail()
	{
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, 1.1, 0.05);
	}
};
