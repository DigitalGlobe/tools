//-----------------------------------------------------------------------------
/*
 * CppUnitFacade.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

//-----------------------------------------------------------------------------
namespace DigitalGlobeTesting
{
    //-------------------------------------------------------------------------
    /**
     * The <code>CCppUnitFacade</code> class represents facades that provide
     * access to <a href="https://sourceforge.net/projects/cppunit/">CppUnit</a>.
     */
    class CCppUnitFacade
    {
        public:
        
            //-----------------------------------------------------------------
            // public constructors
            
                //-------------------------------------------------------------
                /**
                 * Constructs this facade as a default facade.
                 */
                CCppUnitFacade()
                {
                }
                //-------------------------------------------------------------
            
            //-----------------------------------------------------------------
            // public methods
            
                //-------------------------------------------------------------
                /**
                 * Runs CppUnit tests.
                 *
                 * @return  zero if the tests were successful, non-zero if one or more of the
                 *          tests failed
                 */
                int RunCppUnitTests()
                {
                    bool                        bSuccess = false;
                    CppUnit::Test*              pSuite   = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
                    CppUnit::TextUi::TestRunner runner;
                    
                    runner.addTest(pSuite);
                    runner.setOutputter( new CppUnit::CompilerOutputter( &( runner.result() ) ,
                                                                            std::cerr         ) );
                    bSuccess = runner.run();
                    
                    return ( bSuccess ?
                              0       :
                             -1       );
                }
                //-------------------------------------------------------------
            
            //-----------------------------------------------------------------
            
    };
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
