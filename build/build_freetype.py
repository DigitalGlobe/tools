#------------------------------------------------------------------------------
#
# build_freetype.py
#
# Summary : Builds the FreeType library.
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the FreeType library."
        #----------------------------------------------------------------------
        # the name of the build 32-bit debug library file
        _FILE_NAME_LIBRARY_BUILD_DEBUG_X86 = "freetype265d.lib"
        #----------------------------------------------------------------------
        # the name of the build 32-bit release library file
        _FILE_NAME_LIBRARY_BUILD_RELEASE_X86 = "freetype265.lib"
        #----------------------------------------------------------------------
        # the name of the build 64-bit debug library file
        _FILE_NAME_LIBRARY_BUILD_DEBUG_X64 = "freetype265d.lib"
        #----------------------------------------------------------------------
        # the name of the build 64-bit release library file
        _FILE_NAME_LIBRARY_BUILD_RELEASE_X64 = "freetype265.lib"
        #----------------------------------------------------------------------
        # the name of the distribution debug library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG = "freetype_d.lib"
        #----------------------------------------------------------------------
        # the name of the distribution release library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE = "freetype.lib"
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "builds\\windows\\vc2010\\freetype.sln"
        #----------------------------------------------------------------------
        # the pattern for binary files
        _FILE_PATTERN_BINARY = "*.exe"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit binary files
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit binary files
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "FreeType"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug build
        # files
        _PATH_NAME_BUILD_DEBUG_X86 = "objs\\vc2010\\Win32"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release build
        # files
        _PATH_NAME_BUILD_RELEASE_X86 = "objs\\vc2010\\Win32"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug build
        # files
        _PATH_NAME_BUILD_DEBUG_X64 = "objs\\vc2010\\x64"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release build
        # files
        _PATH_NAME_BUILD_RELEASE_X64 = "objs\\vc2010\\x64"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\FreeType"
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
        
            systemManager = SystemManager()
            pathFinder    = PathFinder()
            
            # process command-line arguments
            buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)
            
            # initialize environment variables
            if ( buildSettings.X64Specified() ) :
            
                os.environ["PLATFORM"] = "X64"
            
            # determine path names
            binaryPathName = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64) \
                               if ( buildSettings.X64Specified() )                                     \
                               else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildBinaryPathName         = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE_X64                                          )                  
                 buildLibraryFileName        = os.path.join( buildBinaryPathName                                                           , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE_X64                                  )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildBinaryPathName         = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE_X86                                          )                  
                 buildLibraryFileName        = os.path.join( buildBinaryPathName                                                           , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE_X86                                  )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildBinaryPathName         = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG_X64                                            )                  
                 buildLibraryFileName        = os.path.join( buildBinaryPathName                                                           , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG_X64                                    )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 
            else :
            
                 buildBinaryPathName         = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG_X86                                            )                  
                 buildLibraryFileName        = os.path.join( buildBinaryPathName                                                           , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG_X86                                    )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 
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
                                     ( "Release"                           \
                                       if ( buildSettings.ReleaseSpecified() ) \
                                       else "Debug"                            )                   , \
                                     solutionFileName                                              ) )
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # copy the binary files
            buildBinaryFileNames = glob.glob( os.path.join( buildBinaryPathName          , \
                                                            Program._FILE_PATTERN_BINARY ) );
            for buildBinaryFileName in buildBinaryFileNames :
            
                binaryFileName = os.path.join( binaryPathName , \
                                               ( os.path.basename(buildBinaryFileName) \
                                                 if ( buildSettings.ReleaseSpecified() ) \
                                                 else systemManager.getDebugFileName( os.path.basename(buildBinaryFileName) ) ) )
                systemManager.copyFile( buildBinaryFileName , \
                                        binaryFileName      )
                                            
            # distribute the library file
            systemManager.copyFile( buildLibraryFileName        , \
                                    distributionLibraryFileName )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
