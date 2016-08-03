#------------------------------------------------------------------------------
#
# UtilityPathNameTest.py
#
#------------------------------------------------------------------------------

import unittest

from UtilityPathName import *

#------------------------------------------------------------------------------
# The UtilityPathNameTest class represents test cases for the UtilityPathName
# class.
class UtilityPathNameTest(unittest.TestCase) :

    #--------------------------------------------------------------------------
    # initialization and deinitialization methods
    
        #----------------------------------------------------------------------
        # Sets up the test-case class.
        #
        # Parameters :
        #     cls : the test-case class
        @classmethod
        def setUpClass(cls) :
        
            pass
        #----------------------------------------------------------------------
        # Sets up this test case.
        #
        # Parameters :
        #     self : this test case
        def setUp(self) :
        
            pass
        #----------------------------------------------------------------------
        # Tears down this test case.
        #
        # Parameters :
        #     self : this test case
        def tearDown(self) :
        
            pass
        #----------------------------------------------------------------------
        # Tears down the test-case class.
        #
        # Parameters :
        #     cls : the test-case class
        @classmethod
        def tearDownClass(cls) :
        
            pass
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # test methods
    
        #----------------------------------------------------------------------
        # Tests the getFileName() method.
        #
        # Parameters :
        #     self : this test case
        def testGetFileName(self) :
        
            self.assertEqual( "File"                                               , \
                              UtilityPathName.getFileName("C:\\Directory\\File"    ) )
            self.assertEqual( "File."                                              , \
                              UtilityPathName.getFileName("C:\\Directory\\File."   ) )
            self.assertEqual( "File.txt"                                           , \
                              UtilityPathName.getFileName("C:\\Directory\\File.txt") )
            self.assertEqual( "File"                                               , \
                              UtilityPathName.getFileName("File"                   ) )
            self.assertEqual( "File."                                              , \
                              UtilityPathName.getFileName("File."                  ) )
            self.assertEqual( "File.txt"                                           , \
                              UtilityPathName.getFileName("File.txt"               ) )
            self.assertEqual( ".cshrc"                                             , \
                              UtilityPathName.getFileName(".cshrc"                 ) )
            self.assertEqual( ""                                                   , \
                              UtilityPathName.getFileName(""                       ) )
        #----------------------------------------------------------------------
        # Tests the getParent() method.
        #
        # Parameters :
        #     self : this test case
        def testGetParent(self) :
        
            self.assertEqual( "C:\\Directory"                                          , \
                              UtilityPathName.getParent("C:\\Directory\\File.txt"    ) )
            self.assertEqual( "C:\\Directory"                                          , \
                              UtilityPathName.getParent("C:\\Directory\\Subdirectory") )
            self.assertEqual( ""                                                       , \
                              UtilityPathName.getParent("File.txt"                     ) )
            self.assertEqual( ""                                                       , \
                              UtilityPathName.getParent(""                             ) )
            self.assertEqual( "C:\\"                                                   , \
                              UtilityPathName.getParent("C:\\Directory"                ) )
            self.assertEqual( "C:"                                                     , \
                              UtilityPathName.getParent("C:"                           ) )
        #----------------------------------------------------------------------
        # Tests the getPathNameWithoutExtension() method.
        #
        # Parameters :
        #     self : this test case
        def testGetPathNameWithoutExtension(self) :
        
            self.assertEqual( "C:\\Directory\\File"                                                  , \
                              UtilityPathName.getPathNameWithoutExtension("C:\\Directory\\File"    ) )
            self.assertEqual( "C:\\Directory\\File"                                                  , \
                              UtilityPathName.getPathNameWithoutExtension("C:\\Directory\\File."   ) )
            self.assertEqual( "C:\\Directory\\File"                                                  , \
                              UtilityPathName.getPathNameWithoutExtension("C:\\Directory\\File.txt") )
            self.assertEqual( "File"                                                                 , \
                              UtilityPathName.getPathNameWithoutExtension("File"                   ) )
            self.assertEqual( "File"                                                                 , \
                              UtilityPathName.getPathNameWithoutExtension("File."                  ) )
            self.assertEqual( "File"                                                                 , \
                              UtilityPathName.getPathNameWithoutExtension("File.txt"               ) )
            self.assertEqual( ".cshrc"                                                               , \
                              UtilityPathName.getPathNameWithoutExtension(".cshrc"                 ) )
            self.assertEqual( ""                                                                     , \
                              UtilityPathName.getPathNameWithoutExtension(""                       ) )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    
#------------------------------------------------------------------------------
if (__name__ == '__main__') :

    unittest.main()
#------------------------------------------------------------------------------
