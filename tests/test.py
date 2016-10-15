#------------------------------------------------------------------------------
#
# test.py
#
# Summary : Tests all of the third-party libraries.
#
#------------------------------------------------------------------------------

import os
import sys

from BuildSettingSet import *
from SystemManager   import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # the name of the Python executable file
        _FILE_NAME_PYTHON = "python.exe"
        #----------------------------------------------------------------------
        # the name of the test file to test APR
        _FILE_NAME_TEST_APR = "test_apr.py"
        #----------------------------------------------------------------------
        # the name of the test file to test APR Util
        _FILE_NAME_TEST_APR_UTIL = "test_aprutil.py"
        #----------------------------------------------------------------------
        # the name of the test file to test Bison
        _FILE_NAME_TEST_BISON = "test_bison.py"
        #----------------------------------------------------------------------
        # the name of the test file to test CURL
        _FILE_NAME_TEST_CURL = "test_curl.py"
        #----------------------------------------------------------------------
         # the name of the test file to test Expat
        _FILE_NAME_TEST_EXPAT = "test_expat.py"
        #----------------------------------------------------------------------
        # the name of the test file to test FreeType
        _FILE_NAME_TEST_FREETYPE = "test_freetype.py"
        #----------------------------------------------------------------------
        # the name of the test file to test GEos
        _FILE_NAME_TEST_GEOS = "test_geos.py"
        #----------------------------------------------------------------------
        # the name of the test file to test Glew
        _FILE_NAME_TEST_GLEW = "test_glew.py"
        #----------------------------------------------------------------------
        # the name of the test file to test Glut
        _FILE_NAME_TEST_GLUT = "test_glut.py"
        #----------------------------------------------------------------------
        # the name of the test file to test GoogleTest
        _FILE_NAME_TEST_GOOGLETEST = "test_googletest.py"
        #----------------------------------------------------------------------
        # the name of the test file to test KDIS
        _FILE_NAME_TEST_KDIS = "test_kdis.py"
        #----------------------------------------------------------------------
        # the name of the test file to test LibGEOTIFF
        _FILE_NAME_TEST_LIBGEOTIFF = "test_libgeotiff.py"
        #----------------------------------------------------------------------
        # the name of the test file to test HDF5
        _FILE_NAME_TEST_HDF5 = "test_hdf5.py"
        #----------------------------------------------------------------------
        # the name of the test file to test LibICONV
        _FILE_NAME_TEST_LIBICONV = "test_libiconv.py"
        #----------------------------------------------------------------------
        # the name of the test file to test LibJPEG
        _FILE_NAME_TEST_LIBJPEG = "test_libjpeg.py"
        #----------------------------------------------------------------------
        # the name of the test file to test LibPNG
        _FILE_NAME_TEST_LIBPNG = "test_libpng.py"
        #----------------------------------------------------------------------
        # the name of the test file to test LibTIFF
        _FILE_NAME_TEST_LIBTIFF = "test_libtiff.py"
        #----------------------------------------------------------------------
        # the name of the test file to test LibXML
        _FILE_NAME_TEST_LIBXML = "test_libxml.py"
        #----------------------------------------------------------------------
        # the name of the test file to test Newmat
        _FILE_NAME_TEST_NEWMAT = "test_newmat.py"
        #----------------------------------------------------------------------
        # the name of the test file to test OpenThreads
        _FILE_NAME_TEST_OPENTHREADS = "test_openthreads.py"
        #----------------------------------------------------------------------
        # the name of the test file to test OpenCV
        _FILE_NAME_TEST_OPENCV = "test_opencv.py"
        #----------------------------------------------------------------------
        # the name of the test file to test OpenDIS
        _FILE_NAME_TEST_OPENDIS = "test_opendis.py"
        #----------------------------------------------------------------------
        # the name of the test file to test PoDoFo
        _FILE_NAME_TEST_PODOFO = "test_podofo.py"
        #----------------------------------------------------------------------
        # the name of the test file to test PROJ4
        _FILE_NAME_TEST_PROJ4 = "test_proj4.py"
        #----------------------------------------------------------------------
        # the name of the test file to test SZip
        _FILE_NAME_TEST_SZIP = "test_szip.py"
        #----------------------------------------------------------------------
        # the name of the test file to test uriparser
        _FILE_NAME_TEST_URIPARSER = "test_uriparser.py"
        #----------------------------------------------------------------------
        # the name of the test file to test xerces
        _FILE_NAME_TEST_XERCES = "test_xerces.py"
        #----------------------------------------------------------------------
        # the name of the test file to test ZLib
        _FILE_NAME_TEST_ZLIB = "test_zlib.py"
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # constructors
    
        #----------------------------------------------------------------------
        # Constructs this program.
        #
        # Parameters :
        #     self : this program
        def __init__(self) :
        
            pass
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # public methods
    
        #----------------------------------------------------------------------
        # The main method of the program.
        #
        # Parameters :
        #     self : this program
        def main(self) :
        
            if ( len(sys.argv) > 1 ) :
            
                # test all configurations of the specified test file
                self._test(sys.argv[1])
            
            else :
            
                self._test(Program._FILE_NAME_TEST_APR        )
                self._test(Program._FILE_NAME_TEST_APR_UTIL   )
                self._test(Program._FILE_NAME_TEST_BISON      )
                self._test(Program._FILE_NAME_TEST_CURL       )
                self._test(Program._FILE_NAME_TEST_EXPAT      )
                self._test(Program._FILE_NAME_TEST_FREETYPE   )
                self._test(Program._FILE_NAME_TEST_GEOS       )
                self._test(Program._FILE_NAME_TEST_GLEW       )
                self._test(Program._FILE_NAME_TEST_GLUT       )
                self._test(Program._FILE_NAME_TEST_GOOGLETEST )
                self._test(Program._FILE_NAME_TEST_HDF5       )
                self._test(Program._FILE_NAME_TEST_KDIS       )
                self._test(Program._FILE_NAME_TEST_LIBGEOTIFF )
                self._test(Program._FILE_NAME_TEST_LIBICONV   )
                self._test(Program._FILE_NAME_TEST_LIBJPEG    )
                self._test(Program._FILE_NAME_TEST_LIBTIFF    )
                self._test(Program._FILE_NAME_TEST_LIBPNG     )
                self._test(Program._FILE_NAME_TEST_LIBXML     )
                self._test(Program._FILE_NAME_TEST_NEWMAT     )
                self._test(Program._FILE_NAME_TEST_OPENCV     )
                self._test(Program._FILE_NAME_TEST_OPENDIS    )
                self._test(Program._FILE_NAME_TEST_OPENTHREADS)
                self._test(Program._FILE_NAME_TEST_PODOFO     )
                self._test(Program._FILE_NAME_TEST_PROJ4      )
                self._test(Program._FILE_NAME_TEST_SZIP       )
                self._test(Program._FILE_NAME_TEST_URIPARSER  )
                self._test(Program._FILE_NAME_TEST_XERCES     )
                self._test(Program._FILE_NAME_TEST_ZLIB       )
    #--------------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # private methods
    
        #----------------------------------------------------------------------
        # Tests a library using a specified test file.
        #
        # Parameters :
        #     self         : this program
        #     testFileName : the name of the test file to use
        def _test( self         , \
                   testFileName ) :
                    
            systemManager = SystemManager()
        
            # test 32-bit debug
            if ( systemManager.execute( "%s \"%s\" \"%s\" \"%s\"" % \
                                        ( Program._FILE_NAME_PYTHON                            , \
                                          os.path.join( systemManager.getCurrentPathName() , \
                                                        testFileName                       )   , \
                                          BuildSettingSet.ARGUMENT_VALUE_BITNESS_X86           , \
                                          BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_DEBUG   ) ) != 0 ) :
                                          
                sys.exit(-1)

            # test 32-bit release
            if ( systemManager.execute( "%s \"%s\" \"%s\" \"%s\"" % \
                                        ( Program._FILE_NAME_PYTHON                            , \
                                          os.path.join( systemManager.getCurrentPathName() , \
                                                        testFileName                       )   , \
                                          BuildSettingSet.ARGUMENT_VALUE_BITNESS_X86           , \
                                          BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_RELEASE ) ) != 0 ) :
                                          
                sys.exit(-1)

            # test 64-bit debug
            if ( systemManager.execute( "%s \"%s\" \"%s\" \"%s\"" % \
                                        ( Program._FILE_NAME_PYTHON                            , \
                                          os.path.join( systemManager.getCurrentPathName() , \
                                                        testFileName                       )   , \
                                          BuildSettingSet.ARGUMENT_VALUE_BITNESS_X64           , \
                                          BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_DEBUG   ) ) != 0 ) :
                                          
                sys.exit(-1)

            # test 64-bit release
            if ( systemManager.execute( "%s \"%s\" \"%s\" \"%s\"" % \
                                        ( Program._FILE_NAME_PYTHON                            , \
                                          os.path.join( systemManager.getCurrentPathName() , \
                                                        testFileName                       )   , \
                                          BuildSettingSet.ARGUMENT_VALUE_BITNESS_X64           , \
                                          BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_RELEASE ) ) != 0 ) :
                                          
                sys.exit(-1)
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    
#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
