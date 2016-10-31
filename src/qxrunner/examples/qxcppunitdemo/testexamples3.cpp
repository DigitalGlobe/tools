// This class implements a set of CppUnit example test cases which
// succeed or fail. It isn't intended to show how to write CppUnit tests.

//#include "../../CppUnit/include/cppunit/TestFixture.h"
#include "../../CppUnit/include/cppunit/extensions/HelperMacros.h"
#include "../../CppUnit/include/cppunit/Test.h"
//#include "../../CppUnit/include/cppunit/Portability.h"


class TestExamples3;

CPPUNIT_TEST_SUITE_REGISTRATION(TestExamples3);

class TestExamples3 : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TestExamples3);

	CPPUNIT_TEST(testAssertSucceed);
	CPPUNIT_TEST(testAssertFail);
	CPPUNIT_TEST(testAssertNoThrowSucceed);
	CPPUNIT_TEST(testAssertNoThrowFail);

	CPPUNIT_TEST_SUITE_END();

	void testAssertSucceed()
	{
		CPPUNIT_ASSERT(1 == 1);
	}

	void testAssertFail()
	{
		CPPUNIT_ASSERT(1 == 0);
	}

	void testAssertNoThrowSucceed()
	{
		std::vector<int> v;
		v.push_back(999);

		CPPUNIT_ASSERT_NO_THROW(v.at(0));
	}

	void testAssertNoThrowFail()
	{
		std::vector<int> v;

		CPPUNIT_ASSERT_NO_THROW(v.at(0));
	}
};
