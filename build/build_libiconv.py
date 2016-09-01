#------------------------------------------------------------------------------
#
# build_libiconv.py
#
# Summary : Builds the LibICONV library.
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder      import *
from SystemManager   import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the LibICONV library."
        #----------------------------------------------------------------------
        # the name of the build debug debug file
        _FILE_NAME_DEBUG_BUILD_DEBUG = "libiconvD.pdb"
        #----------------------------------------------------------------------
        # the name of the build release debug file
        _FILE_NAME_DEBUG_BUILD_RELEASE = "libiconv.pdb"
        #----------------------------------------------------------------------
        # the name of the build debug dynamic file
        _FILE_NAME_DYNAMIC_BUILD_DEBUG = "libiconvD.dll"
        #----------------------------------------------------------------------
        # the name of the build release dynamic file
        _FILE_NAME_DYNAMIC_BUILD_RELEASE = "libiconv.dll"
        #----------------------------------------------------------------------
        # the name of the build debug library file
        _FILE_NAME_LIBRARY_BUILD_DEBUG = "libiconvD.lib"
        #----------------------------------------------------------------------
        # the name of the build release library file
        _FILE_NAME_LIBRARY_BUILD_RELEASE = "libiconv.lib"
        #----------------------------------------------------------------------
        # the name of the distribution debug debug file
        _FILE_NAME_DEBUG_DISTRIBUTION_DEBUG = "libiconv_d.pdb"
        #----------------------------------------------------------------------
        # the name of the distribution release debug file
        _FILE_NAME_DEBUG_DISTRIBUTION_RELEASE = "libiconv.pdb"
        #----------------------------------------------------------------------
        # the name of the distribution debug dynamic file
        _FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG = "libiconv_d.dll"
        #----------------------------------------------------------------------
        # the name of the distribution release dynamic file
        _FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE = "libiconv.dll"
        #----------------------------------------------------------------------
        # the name of the distribution debug library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG = "libiconv_d.lib"
        #----------------------------------------------------------------------
        # the name of the distribution release library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE = "libiconv.lib"
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "LibIconv.sln"
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
        _PATH_NAME_BUILD = "LibICONV"
        #----------------------------------------------------------------------
        # the name of the path that will contain 32-bit debug build files
        _PATH_NAME_BUILD_DEBUG_X86 = "Debug_Win32"
        #----------------------------------------------------------------------
        # the name of the path that will contain 32-bit release build files
        _PATH_NAME_BUILD_RELEASE_X86 = "Release_Win32"
        #----------------------------------------------------------------------
        # the name of the path that will contain 64-bit debug build files
        _PATH_NAME_BUILD_DEBUG_X64 = "Debug_x64"
        #----------------------------------------------------------------------
        # the name of the path that will contain 64-bit release build files
        _PATH_NAME_BUILD_RELEASE_X64 = "Release_x64"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\LibICONV"
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
            systemManager.initializeIncludeEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.initializeLibraryEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.appendToPathEnvironmentVariable( pathFinder.getVisualStudioBinPathName( buildSettings.X64Specified() ) )
            if ( buildSettings.X64Specified() ) :
            
                os.environ["PLATFORM"] = "X64"
                
            print(os.environ["PATH"])
            
            # determine path names
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE_X64                                          ,
                                                             Program._FILE_NAME_DEBUG_BUILD_RELEASE                                        ) 
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE_X64                                          ,
                                                             Program._FILE_NAME_DYNAMIC_BUILD_RELEASE                                      ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE_X64                                          ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION_RELEASE                                 )
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE                               )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE_X86                                          ,
                                                             Program._FILE_NAME_DEBUG_BUILD_RELEASE                                        ) 
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE_X86                                          ,
                                                             Program._FILE_NAME_DYNAMIC_BUILD_RELEASE                                      ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE_X86                                          ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION_RELEASE                                 )
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE                               )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG_X64                                            ,
                                                             Program._FILE_NAME_DEBUG_BUILD_DEBUG                                          ) 
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG_X64                                            ,
                                                             Program._FILE_NAME_DYNAMIC_BUILD_DEBUG                                        ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG_X64                                            ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        ) 
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION_DEBUG                                   )
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG                                 )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 
            else :
            
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG_X86                                            ,
                                                             Program._FILE_NAME_DEBUG_BUILD_DEBUG                                          ) 
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG_X86                                            ,
                                                             Program._FILE_NAME_DYNAMIC_BUILD_DEBUG                                        ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG_X86                                            ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        ) 
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION_DEBUG                                   )
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG                                 )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 
            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )
                               
            # build the solution
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( ( "\"%s\" "              + \
                                     "/p:Configuration=%s " + \
                                     "\"%s\""               ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                   ( "Release"                               \
                                     if ( buildSettings.ReleaseSpecified() ) \
                                     else "Debug"                          )                       , \
                                    solutionFileName                       ) )
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute the library file
            systemManager.copyFile( buildDebugFileName          , \
                                    distributionDebugFileName   )
            systemManager.copyFile( buildDynamicFileName        , \
                                    distributionDynamicFileName )
            systemManager.copyFile( buildLibraryFileName        , \
                                    distributionLibraryFileName )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
