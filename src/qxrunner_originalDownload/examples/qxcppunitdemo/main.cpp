#include <QApplication>
#include <qxcppunit/testrunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QxCppUnit::TestRunner runner;

	runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	runner.run();

	return 0;
}
