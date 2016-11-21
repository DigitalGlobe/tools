#------------------------------------------------------------------------------
#
# build_hdf5.py
#
# Summary : Builds the HDF5 library.
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
        DESCRIPTION = "Builds the HDF5 library."
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "HDF5.sln"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "HDF5"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X86 = "bin\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X86 = "bin\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X64 = "bin\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X64 = "bin\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X86 = "bin\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X86 = "bin\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X64 = "bin\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X64 = "bin\\Release"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\HDF5"
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
                
            else:
            
                os.environ["PLATFORM"] = "Win32"
                
            # determine path names and path names
            buildPathName        = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName       = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            buildBinaryPathName  = os.path.join( buildPathName , \
                                                 fileDistributor.determinePathName( buildSettings.X64Specified()                 , \
                                                                                    buildSettings.ReleaseSpecified()             , \
                                                                                    Program._PATH_NAME_BUILD_BINARY_DEBUG_X86    , \
                                                                                    Program._PATH_NAME_BUILD_BINARY_RELEASE_X86  , \
                                                                                    Program._PATH_NAME_BUILD_BINARY_DEBUG_X64    , \
                                                                                    Program._PATH_NAME_BUILD_BINARY_RELEASE_X64  ) )
            buildLibraryPathName = os.path.join( buildPathName , \
                                                 fileDistributor.determinePathName( buildSettings.X64Specified()                 , \
                                                                                    buildSettings.ReleaseSpecified()             , \
                                                                                    Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X86   , \
                                                                                    Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X86 , \
                                                                                    Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X64   , \
                                                                                    Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X64 ) )

            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )
                               
            # run CMake
            if ( buildSettings.X64Specified() ) :
            
                cmakeCommandLine = ( ( "\"%s\" "                                        + \
                                       "-DX86_64=1 -G \"Visual Studio 14 2015 Win64\" " +
                                       "\"%s\""                                         ) % \
                                     ( PathFinder.FILE_NAME_CMAKE , \
                                       sourcePathName             )                       )

            else:
            
                cmakeCommandLine = ( ( "\"%s\" "                     + \
                                       "\"%s\""                      ) % \
                                     ( PathFinder.FILE_NAME_CMAKE , \
                                       sourcePathName             )    )
                
            systemManager.changeDirectory(buildPathName)
            cmakeResult = systemManager.execute(cmakeCommandLine)
            if (cmakeResult != 0) :
            
                sys.exit(-1)

            # build the solution
            systemManager.removeFile( os.path.join( buildPathName  ,
                                                    "bin\\debug"   ) );
            systemManager.removeFile( os.path.join( buildPathName  ,
                                                    "bin\\release" ) );
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
            fileDistributor.distributeAllFiles( buildBinaryPathName              , \
                                                buildLibraryPathName             , \
                                                buildLibraryPathName             , \
                                                buildLibraryPathName             , \
                                                buildSettings.X64Specified()     , \
                                                buildSettings.ReleaseSpecified() , \
                                                True                             )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
