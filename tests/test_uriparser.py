#------------------------------------------------------------------------------
#
# test_uriparser.py
#
# Summary : Tests the UriParser libraries.
#
#------------------------------------------------------------------------------

import os
import sys

from BuildSettingSet import *
from FileDistributor import *
from PathFinder      import *
from SystemManager   import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

        # a description of what the script does
        _DESCRIPTION = "Tests the UriParser libs"
        _CONFIGURATION_DEBUG = "Debug"
        _CONFIGURATION_RELEASE = "Release"
        # the 32-bit platform
        _PLATFORM_X86 = "X86"
        # the 64-bit platform
        _PLATFORM_X64 = "X64"
        _FILE_NAME_SOLUTION = "UriParser.sln"
        _FILE_NAME_TEST_DEBUG_X86 = "UriParser.exe"
        _FILE_NAME_TEST_RELEASE_X86 = "UriParser.exe"
        _FILE_NAME_TEST_DEBUG_X64 = "UriParser.exe"
        _FILE_NAME_TEST_RELEASE_X64 = "UriParser.exe"
        _PATH_NAME_COMPILE_DEBUG_X86 = "Debug"
        _PATH_NAME_COMPILE_RELEASE_X86 = "Release"
        _PATH_NAME_COMPILE_DEBUG_X64 = "X64\\Debug"
        _PATH_NAME_COMPILE_RELEASE_X64 = "X64\\Release"
        #----------------------------------------------------------------------
        # the name of the path that contains the test solution
        _PATH_NAME_TEST = "UriParser"
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
        
            fileDistributor = FileDistributor()
            systemManager   = SystemManager()
            pathFinder      = PathFinder()
            
            # process command-line arguments
            buildSettings = BuildSettingSet.fromCommandLine(Program._DESCRIPTION)
            
            # get the test path name
            testPathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_TEST)
                                         
            # initialize environment variables
            systemManager.initializeIncludeEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.initializeLibraryEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.appendToPathEnvironmentVariable( pathFinder.getVisualStudioBinPathName( buildSettings.X64Specified() ) )
            os.environ["PLATFORM"] = ( Program._PLATFORM_X64               \
                                       if ( buildSettings.X64Specified() ) \
                                       else Program._PLATFORM_X86          )
                                       
            # determine the compile path
            compilePathName = os.path.join( testPathName , \
                                            fileDistributor.determinePathName( buildSettings.X64Specified()           , \
                                                                               buildSettings.ReleaseSpecified()       , \
                                                                               Program._PATH_NAME_COMPILE_DEBUG_X86   , \
                                                                               Program._PATH_NAME_COMPILE_RELEASE_X86 , \
                                                                               Program._PATH_NAME_COMPILE_DEBUG_X64   , \
                                                                               Program._PATH_NAME_COMPILE_RELEASE_X64 ) )
                
            # build the test
            systemManager.removeDirectory(compilePathName)
            solutionFileName   = os.path.join( testPathName                , \
                                               Program._FILE_NAME_SOLUTION )
            configuration      = ( Program._CONFIGURATION_RELEASE          \
                                   if ( buildSettings.ReleaseSpecified() ) \
                                   else Program._CONFIGURATION_DEBUG       )
            msBuildCommandLine = ( ( "\"%s\" "                  + \
                                     "/p:Configuration=\"%s\" " + \
                                     "\"%s\""                   ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                     configuration                                                 ,
                                     solutionFileName                                              ) )
            msBuildResult = systemManager.execute(msBuildCommandLine)
            if (msBuildResult != 0) :
            
                sys.exit(-1)
                
            # run the test
            testFileName = os.path.join( compilePathName , \
                                         fileDistributor.determinePathName( buildSettings.X64Specified()        , \
                                                                            buildSettings.ReleaseSpecified()    , \
                                                                            Program._FILE_NAME_TEST_DEBUG_X86   , \
                                                                            Program._FILE_NAME_TEST_RELEASE_X86 , \
                                                                            Program._FILE_NAME_TEST_DEBUG_X64   , \
                                                                            Program._FILE_NAME_TEST_RELEASE_X64 ) )
            testResult   = systemManager.execute(testFileName)
            
            # delete the compile folder
            systemManager.removeDirectory(compilePathName)
            
            sys.exit(testResult)
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------