#------------------------------------------------------------------------------
#
# build_bison.py
#
# Summary : Builds the Bison library.
#
#------------------------------------------------------------------------------

import glob
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
        DESCRIPTION = "Builds the Bison library."
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "vc_proj\\win_flex_bison.sln"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "Bison"
        #----------------------------------------------------------------------
        # the name of the build debug x86 binary file
        _FILE_NAME_BINARY_BUILD_DEBUG_X86 = "bin\\Debug\\win_bison.exe"
        #----------------------------------------------------------------------
        # the name of the distribution debug x86 binary file
        _FILE_NAME_BINARY_DISTRIBUTION_DEBUG_X86 = "..\\sdk\\x86\\bin\\bison_d.exe"
        #----------------------------------------------------------------------
        # the name of the build release x86 binary file
        _FILE_NAME_BINARY_BUILD_RELEASE_X86 = "bin\\Release\\win_bison.exe"
        #----------------------------------------------------------------------
        # the name of the distribution release x86 binary file
        _FILE_NAME_BINARY_DISTRIBUTION_RELEASE_X86 = "..\\sdk\\x86\\bin\\bison.exe"
        #----------------------------------------------------------------------
        # the name of the build debug x64 binary file
        _FILE_NAME_BINARY_BUILD_DEBUG_X64 = "vc_proj\\x64\\Debug\\win_bison.exe"
        #----------------------------------------------------------------------
        # the name of the distribution debug x64 binary file
        _FILE_NAME_BINARY_DISTRIBUTION_DEBUG_X64 = "..\\sdk\\x64\\bin\\bison_d.exe"
        #----------------------------------------------------------------------
        # the name of the build release x64 binary file
        _FILE_NAME_BINARY_BUILD_RELEASE_X64 = "vc_proj\\x64\\Release\\win_bison.exe"
        #----------------------------------------------------------------------
        # the name of the distribution release x64 binary file
        _FILE_NAME_BINARY_DISTRIBUTION_RELEASE_X64 = "..\\sdk\\x64\\bin\\bison.exe"
        #----------------------------------------------------------------------
        # the name of the build debug x86 data path
        _PATH_NAME_DATA_BUILD_DEBUG_X86 = "bison\\data"
        #----------------------------------------------------------------------
        # the name of the distribution debug x86 data path
        _PATH_NAME_DATA_DISTRIBUTION_DEBUG_X86 = "..\\sdk\\x86\\bin\\data"
        #----------------------------------------------------------------------
        # the name of the build release x86 data path
        _PATH_NAME_DATA_BUILD_RELEASE_X86 = "bison\\data"
        #----------------------------------------------------------------------
        # the name of the distribution release x86 data path
        _PATH_NAME_DATA_DISTRIBUTION_RELEASE_X86 = "..\\sdk\\x86\\bin\\data"
        #----------------------------------------------------------------------
        # the name of the build debug x64 data path
        _PATH_NAME_DATA_BUILD_DEBUG_X64 = "bison\\data"
        #----------------------------------------------------------------------
        # the name of the distribution debug x64 data path
        _PATH_NAME_DATA_DISTRIBUTION_DEBUG_X64 = "..\\sdk\\x64\\bin\\data"
        #----------------------------------------------------------------------
        # the name of the build release x64 data path
        _PATH_NAME_DATA_BUILD_RELEASE_X64 = "bison\\data"
        #----------------------------------------------------------------------
        # the name of the distribution release x64 data path
        _PATH_NAME_DATA_DISTRIBUTION_RELEASE_X64 = "..\\sdk\\x64\\bin\\data"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\Bison"
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
            buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)
            
            # initialize environment variables
            if ( buildSettings.X64Specified() ) :
            
                os.environ["PLATFORM"] = "X64"
            
            # determine path names and path names
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)

            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )
                               
            # build the solution
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( ( "\"%s\" "                  + \
                                     "/p:Configuration=\"%s\" " + \
                                     "\"%s\""                   ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                     ( "Release"                               \
                                       if ( buildSettings.ReleaseSpecified() ) \
                                       else "Debug"                            )                   , \
                                     solutionFileName                                              ) )
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute files
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :

                 dataBuildPathName          = os.path.join( buildPathName                               , \
                                                            Program._PATH_NAME_DATA_BUILD_RELEASE_X64   ) 
                 dataDistributionPathName   = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DATA_DISTRIBUTION_RELEASE_X64)
                 binaryBuildFileName        = os.path.join( buildPathName                               , \
                                                            Program._FILE_NAME_BINARY_BUILD_RELEASE_X64 ) 
                 binaryDistributionFileName = systemManager.getCurrentRelativePathName(Program._FILE_NAME_BINARY_DISTRIBUTION_RELEASE_X64)
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 dataBuildPathName          = os.path.join( buildPathName                               , \
                                                            Program._PATH_NAME_DATA_BUILD_DEBUG_X64     ) 
                 dataDistributionPathName   = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DATA_DISTRIBUTION_DEBUG_X64)
                 binaryBuildFileName        = os.path.join( buildPathName                               , \
                                                            Program._FILE_NAME_BINARY_BUILD_RELEASE_X86 ) 
                 binaryDistributionFileName = systemManager.getCurrentRelativePathName(Program._FILE_NAME_BINARY_DISTRIBUTION_RELEASE_X86)
                 
            elif ( buildSettings.X64Specified() ) :
            
                 dataBuildPathName          = os.path.join( buildPathName                               , \
                                                            Program._PATH_NAME_DATA_BUILD_DEBUG_X64     ) 
                 dataDistributionPathName   = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DATA_DISTRIBUTION_DEBUG_X64)
                 binaryBuildFileName        = os.path.join( buildPathName                               , \
                                                            Program._FILE_NAME_BINARY_BUILD_DEBUG_X64   ) 
                 binaryDistributionFileName = systemManager.getCurrentRelativePathName(Program._FILE_NAME_BINARY_DISTRIBUTION_DEBUG_X64  )
                 
            else :
            
                 dataBuildPathName          = os.path.join( buildPathName                               , \
                                                            Program._PATH_NAME_DATA_BUILD_DEBUG_X86     ) 
                 dataDistributionPathName   = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DATA_DISTRIBUTION_DEBUG_X86)
                 binaryBuildFileName        = os.path.join( buildPathName                               , \
                                                            Program._FILE_NAME_BINARY_BUILD_DEBUG_X86   ) 
                 binaryDistributionFileName = systemManager.getCurrentRelativePathName(Program._FILE_NAME_BINARY_DISTRIBUTION_DEBUG_X86  )

            systemManager.copyFile( binaryBuildFileName        ,
                                    binaryDistributionFileName )
            systemManager.removeDirectory(dataDistributionPathName)
            systemManager.copyDirectory( dataBuildPathName         , \
                                         dataDistributionPathName  )
            
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
