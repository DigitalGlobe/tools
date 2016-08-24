#------------------------------------------------------------------------------
#
# build.py
#
# Summary : Builds all of the third-party libraries.
#
#------------------------------------------------------------------------------

import os
import sys

from BuildSettingSet import *
from SystemManager import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # the name of the build file to build Boost
        _FILE_NAME_BUILD_BOOST = "build_boost.py"
        #----------------------------------------------------------------------
        # the name of the build file to build CURL
        _FILE_NAME_BUILD_CURL = "build_curl.py"
        #----------------------------------------------------------------------
        # the name of the build file to build FreeType
        _FILE_NAME_BUILD_FREETYPE = "build_freetype.py"
        #----------------------------------------------------------------------
        # the name of the build file to build HDF5
        _FILE_NAME_BUILD_HDF5 = "build_hdf5.py"
        #----------------------------------------------------------------------
        # the name of the build file to build LibGeoTIFF
        _FILE_NAME_BUILD_LIBGEOTIFF = "build_libgeotiff.py"
        #----------------------------------------------------------------------
        # the name of the build file to build LibJPEG
        _FILE_NAME_BUILD_LIBJPEG = "build_libjpeg.py"
        #----------------------------------------------------------------------
        # the name of the build file to build LibPNG
        _FILE_NAME_BUILD_LIBPNG = "build_libpng.py"
        #----------------------------------------------------------------------
        # the name of the build file to build LibTIFF
        _FILE_NAME_BUILD_LIBTIFF = "build_libtiff.py"
        #----------------------------------------------------------------------
        # the name of the build file to build Newmat
        _FILE_NAME_BUILD_NEWMAT = "build_newmat.py"
        #----------------------------------------------------------------------
        # the name of the build file to build OpenCV
        _FILE_NAME_BUILD_OPENCV = "build_opencv.py"
        #----------------------------------------------------------------------
        # the name of the build file to build PoDoFo
        _FILE_NAME_BUILD_PODOFO = "build_podofo.py"
        #----------------------------------------------------------------------
        # the name of the build file to build PROJ.4
        _FILE_NAME_BUILD_PROJ4 = "build_proj4.py"
        #----------------------------------------------------------------------
        # the name of the build file to build SZip
        _FILE_NAME_BUILD_SZIP = "build_szip.py"
        #----------------------------------------------------------------------
        # the name of the build file to build Xerces
        _FILE_NAME_BUILD_XERCES = "build_xerces.py"
        #----------------------------------------------------------------------
        # the name of the build file to build ZLib
        _FILE_NAME_BUILD_ZLIB = "build_zlib.py"
        #----------------------------------------------------------------------
        # the name of the build file to build CppUnit
        _FILE_NAME_BUILD_CPPUNIT = "build_cppunit.py"
        #----------------------------------------------------------------------
        # the name of the build file to build GDAL
        _FILE_NAME_BUILD_GDAL = "build_gdal.py"
        #----------------------------------------------------------------------
        # the name of the build file to build LasZip
        _FILE_NAME_BUILD_LASZIP = "build_laszip.py"
        #----------------------------------------------------------------------
        # the name of the build file to build LasZip
        _FILE_NAME_BUILD_LIBLAS = "build_liblas.py"
        #----------------------------------------------------------------------
        # the name of the build file to build OGDI
        _FILE_NAME_BUILD_LIBLAS = "build_ogdi.py"
        #----------------------------------------------------------------------
        #the name of the build file to build Expat
        _FILE_NAME_BUILD_EXPAT = "build_expat.py"
        #----------------------------------------------------------------------
        #the name of the build file to build uriparser
        _FILE_NAME_BUILD_URIPARSER = "build_uriparser.py"
        #----------------------------------------------------------------------
         #the name of the build file to build googletest
        _FILE_NAME_BUILD_GOOGLETEST = "build_googletest.py"
        #----------------------------------------------------------------------
        # the name of the Python executable file
        _FILE_NAME_PYTHON = "python.exe"
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
            
                # build all configurations of the specified build file
                self._build(sys.argv[1])
            
            else :
            
                # build libraries that do not depend on other libraries
                #     (order does not matter)
                self._build(Program._FILE_NAME_BUILD_BOOST   )
                self._build(Program._FILE_NAME_BUILD_CPPUNIT )
                self._build(Program._FILE_NAME_BUILD_CURL    )
                self._build(Program._FILE_NAME_BUILD_EXPAT   )
                self._build(Program._FILE_NAME_BUILD_FREETYPE)
                self._build(Program._FILE_NAME_BUILD_HDF5    )
                self._build(Program._FILE_NAME_BUILD_LIBJPEG )
                self._build(Program._FILE_NAME_BUILD_NEWMAT  )
                self._build(Program._FILE_NAME_BUILD_OPENCV  )
                self._build(Program._FILE_NAME_BUILD_PROJ4   )
                self._build(Program._FILE_NAME_BUILD_SZIP    )
                self._build(Program._FILE_NAME_BUILD_ZLIB    )
                self._build(Program._FILE_NAME_BUILD_GDAL    )
                self._build(Program._FILE_NAME_BUILD_LASZIP  )
                self._build(Program._FILE_NAME_BUILD_LIBLAS  )
                self._build(Program._FILE_NAME_BUILD_URIPARSER )
                self._build(Program._FILE_NAME_BUILD_GOOGLETEST )
                

                # build libraries that depend on other libraries
                #     (order does matter)
                self._build(Program._FILE_NAME_BUILD_LIBPNG    )
                self._build(Program._FILE_NAME_BUILD_LIBTIFF   )
                self._build(Program._FILE_NAME_BUILD_LIBGEOTIFF)
                self._build(Program._FILE_NAME_BUILD_PODOFO    )
                self._build(Program._FILE_NAME_BUILD_XERCES    )
                self._build(Program._FILE_NAME_BUILD_OGDI    )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # private methods
    
        #----------------------------------------------------------------------
        # Builds a library using a specified build file.
        #
        # Parameters :
        #     self          : this program
        #     buildFileName : the name of the build file to use
        def _build( self          , \
                    buildFileName ) :
                    
            systemManager = SystemManager()
        
            # build 32-bit debug
            os.system( ( "%s \"%s\" \"%s\" \"%s\"" % \
                       ( Program._FILE_NAME_PYTHON                            , \
                         os.path.join( systemManager.getCurrentPathName() , \
                                       buildFileName                      )   , \
                         BuildSettingSet.ARGUMENT_VALUE_BITNESS_X86           ,
                         BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_DEBUG   ) ) )
        
            # build 32-bit release
            os.system( ( "%s \"%s\" \"%s\" \"%s\"" % \
                       ( Program._FILE_NAME_PYTHON                            , \
                         os.path.join( systemManager.getCurrentPathName() , \
                                       buildFileName                      )   , \
                         BuildSettingSet.ARGUMENT_VALUE_BITNESS_X86           ,
                         BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_RELEASE ) ) )
        
            # build 64-bit debug
            os.system( ( "%s \"%s\" \"%s\" \"%s\"" % \
                       ( Program._FILE_NAME_PYTHON                            , \
                         os.path.join( systemManager.getCurrentPathName() , \
                                       buildFileName                      )   , \
                         BuildSettingSet.ARGUMENT_VALUE_BITNESS_X64           ,
                         BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_DEBUG   ) ) )
        
            # build 64-bit release
            os.system( ( "%s \"%s\" \"%s\" \"%s\"" % \
                       ( Program._FILE_NAME_PYTHON                            , \
                         os.path.join( systemManager.getCurrentPathName() , \
                                       buildFileName                      )   , \
                         BuildSettingSet.ARGUMENT_VALUE_BITNESS_X64           ,
                         BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_RELEASE ) ) )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    
#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
