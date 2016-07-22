#------------------------------------------------------------------------------
#
# build.py
#
# Summary : Builds all of the third-party libraries.
#
#------------------------------------------------------------------------------

import os

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
        
            pass
            self._buildLibJPEG()
        
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # private methods
    
        #----------------------------------------------------------------------
        # Builds LibJPEG.
        #
        #     self : this program
        def _buildLibJPEG(self) :
        
            systemManager = SystemManager()
        
            # build 32-bit debug
            os.system( ( "%s \"%s\" \"%s\" \"%s\"" % \
                       ( Program._FILE_NAME_PYTHON                            , \
                         os.path.join( systemManager.getCurrentPathName() , \
                                       Program._FILE_NAME_BUILD_LIBJPEG   )   , \
                         BuildSettingSet.ARGUMENT_VALUE_BITNESS_X86           ,
                         BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_DEBUG   ) ) )
        
            # build 32-bit relase
            os.system( ( "%s \"%s\" \"%s\" \"%s\"" % \
                       ( Program._FILE_NAME_PYTHON                            , \
                         os.path.join( systemManager.getCurrentPathName() , \
                                       Program._FILE_NAME_BUILD_LIBJPEG   )   , \
                         BuildSettingSet.ARGUMENT_VALUE_BITNESS_X86           ,
                         BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_RELEASE ) ) )
        
            # build 64-bit debug
            os.system( ( "%s \"%s\" \"%s\" \"%s\"" % \
                       ( Program._FILE_NAME_PYTHON                            , \
                         os.path.join( systemManager.getCurrentPathName() , \
                                       Program._FILE_NAME_BUILD_LIBJPEG   )   , \
                         BuildSettingSet.ARGUMENT_VALUE_BITNESS_X64           ,
                         BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_DEBUG   ) ) )
        
            # build 64-bit relase
            os.system( ( "%s \"%s\" \"%s\" \"%s\"" % \
                       ( Program._FILE_NAME_PYTHON                            , \
                         os.path.join( systemManager.getCurrentPathName() , \
                                       Program._FILE_NAME_BUILD_LIBJPEG   )   , \
                         BuildSettingSet.ARGUMENT_VALUE_BITNESS_X64           ,
                         BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_RELEASE ) ) )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    
#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
