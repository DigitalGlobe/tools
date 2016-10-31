// This class implements a set of CppUnit example test cases which
// succeed or fail. It isn't intended to show how to write CppUnit tests.

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class TestExamples2;

CPPUNIT_TEST_SUITE_REGISTRATION(TestExamples2);

class TestExamples2 : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TestExamples2);

	CPPUNIT_TEST(testFail);
	CPPUNIT_TEST(testAssertAssertionPass);
	CPPUNIT_TEST(testAssertAssertionFail);
	CPPUNIT_TEST(testUnhandledError);

	CPPUNIT_TEST_SUITE_END();

	void testFail()
	{
		CPPUNIT_FAIL("Intentionally failed");
	}

	void testAssertAssertionPass()
	{
		CPPUNIT_ASSERT_ASSERTION_PASS(CPPUNIT_ASSERT(1 == 1));
	}

	void testAssertAssertionFail()
	{
		CPPUNIT_ASSERT_ASSERTION_FAIL(CPPUNIT_ASSERT(1 == 2));
	}

	void testUnhandledError()
	{
		throw "Unhandled error";
	}
};
