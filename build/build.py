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
        # the name of the build file to build LibJPEG
        _FILE_NAME_BUILD_LIBJPEG = "build_libjpeg.py"
        #----------------------------------------------------------------------
        # the name of the build file to build LibPNG
        _FILE_NAME_BUILD_LIBPNG = "build_libpng.py"
        #----------------------------------------------------------------------
        # the name of the build file to build LibTIFF
        _FILE_NAME_BUILD_LIBTIFF = "build_libtiff.py"
        #----------------------------------------------------------------------
        # the name of the build file to build PROJ.4
        _FILE_NAME_BUILD_PROJ4 = "build_proj4.py"
        #----------------------------------------------------------------------
        # the name of the build file to build ZLib
        _FILE_NAME_BUILD_ZLIB = "build_zlib.py"
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
                self._build(Program._FILE_NAME_BUILD_LIBJPEG)
                self._build(Program._FILE_NAME_BUILD_PROJ4  )
                self._build(Program._FILE_NAME_BUILD_ZLIB   )
                
                # build libraries that depend on other libraries
                self._build(Program._FILE_NAME_BUILD_LIBPNG )
                self._build(Program._FILE_NAME_BUILD_LIBTIFF)
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
