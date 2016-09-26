#------------------------------------------------------------------------------
#
# test_kdis.py
#
# Summary : Builds the KDIS library.
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

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of what the script does
        _DESCRIPTION = "Tests the KDIS library."
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the debug configuration
        _CONFIGURATION_DEBUG = "Debug"
        #----------------------------------------------------------------------
        # the release configuration
        _CONFIGURATION_RELEASE = "Release"
        #----------------------------------------------------------------------
        # the 32-bit platform
        _PLATFORM_X86 = "X86"
        #----------------------------------------------------------------------
        # the 64-bit platform
        _PLATFORM_X64 = "X64"
        #----------------------------------------------------------------------

        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "KDIS.sln"
        #----------------------------------------------------------------------
        # the name of the 32-bit debug test file
        _FILE_NAME_TEST_DEBUG_X86 = "KDIS.exe"
        #----------------------------------------------------------------------
        # the name of the 32-bit release test file
        _FILE_NAME_TEST_RELEASE_X86 = "KDIS.exe"
        #----------------------------------------------------------------------
        # the name of the 64-bit debug test file
        _FILE_NAME_TEST_DEBUG_X64 = "KDIS.exe"
        #----------------------------------------------------------------------
        # the name of the 64-bit release test file
        _FILE_NAME_TEST_RELEASE_X64 = "KDIS.exe"
        #----------------------------------------------------------------------
        # the name of the path that contains 32-bit debug compile results
        _PATH_NAME_COMPILE_DEBUG_X86 = "Debug"
        #----------------------------------------------------------------------
        # the name of the path that contains 32-bit release compile results
        _PATH_NAME_COMPILE_RELEASE_X86 = "Release"
        #----------------------------------------------------------------------
        # the name of the path that contains 64-bit debug compile results
        _PATH_NAME_COMPILE_DEBUG_X64 = "X64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that contains 64-bit release compile results
        _PATH_NAME_COMPILE_RELEASE_X64 = "X64\\Release"
        #----------------------------------------------------------------------
        # the name of the 32-bit library path
        _PATH_NAME_LIBRARY_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the 64-bit library path
        _PATH_NAME_LIBRARY_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the test solution
        _PATH_NAME_TEST = "KDIS"
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
            systemManager.appendToPathEnvironmentVariable( Program._PATH_NAME_LIBRARY_X64      \
                                                           if ( buildSettings.X64Specified() ) \
                                                           else Program._PATH_NAME_LIBRARY_X86 )
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
                
            if ( buildSettings.ReleaseSpecified() ) :
            
                if ( buildSettings.X64Specified() ) :
                
                    systemManager.copyFile( "..\\sdk\\x64\\lib\\kdis.dll"  ,
                                            "KDIS\\x64\\Release\\kdis.dll" )
                    systemManager.copyFile( "..\\sdk\\x64\\lib\\kdis.dll"  ,
                                            "KDIS\\x64\\Release\\kdis.dll" )
                                            
                else :
                
                    systemManager.copyFile( "..\\sdk\\x86\\lib\\kdis.dll" ,
                                            "KDIS\\Release\\kdis.dll"     )
                    systemManager.copyFile( "..\\sdk\\x86\\lib\\kdis.dll" ,
                                            "KDIS\\Release\\kdis.dll"     )
                                            
            else :
            
                if ( buildSettings.X64Specified() ) :
                
                    systemManager.copyFile( "..\\sdk\\x64\\lib\\kdis_d.dll" ,
                                            "KDIS\\x64\\Debug\\kdisd.dll"   )
                    systemManager.copyFile( "..\\sdk\\x64\\lib\\kdis_d.dll" ,
                                            "KDIS\\x64\\Debug\\kdisd.dll"   )
                                            
                else :
                
                    systemManager.copyFile( "..\\sdk\\x86\\lib\\kdis_d.dll" ,
                                            "KDIS\\Debug\\kdisd.dll"        )
                    systemManager.copyFile( "..\\sdk\\x86\\lib\\kdis_d.dll" ,
                                            "KDIS\\Debug\\kdisd.dll"        )

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
