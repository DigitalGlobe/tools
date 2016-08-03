#------------------------------------------------------------------------------
#
# build_opencv.py
#
# Summary : Builds the OpenCV library.
#
#------------------------------------------------------------------------------

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
        DESCRIPTION = "Builds the OpenCV library."
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "OpenCV.sln"
        #----------------------------------------------------------------------
        # the pattern for binary files
        _FILE_PATTERN_BINARY = "*.exe"
        #----------------------------------------------------------------------
        # the pattern for debug files
        _FILE_PATTERN_DEBUG = "*.pdb"
        #----------------------------------------------------------------------
        # the pattern for library files
        _FILE_PATTERN_LIBRARY = "*.lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built debug binary files
        _PATH_NAME_BINARY_DEBUG = "bin\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built release binary files
        _PATH_NAME_BINARY_RELEASE = "bin\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "OpenCV"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit binary files
        _PATH_NAME_DISTRIBUTION_BINARY_X86 = "..\\sdk\\x86\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit binary files
        _PATH_NAME_DISTRIBUTION_BINARY_X64 = "..\\sdk\\x64\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_LIBRARY_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_LIBRARY_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built debug library files
        _PATH_NAME_LIBRARY_DEBUG = "lib\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built release library files
        _PATH_NAME_LIBRARY_RELEASE = "lib\\Release"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\OpenCV"
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
            
            # determine path names
            buildPathName   = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildBinaryPathName         = os.path.join( buildPathName                      , \
                                                             Program._PATH_NAME_BINARY_RELEASE  )
                 buildLibraryPathName        = os.path.join( buildPathName                      , \
                                                             Program._PATH_NAME_LIBRARY_RELEASE )
                 distributionBinaryPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_BINARY_X64 )
                 distributionLibraryPathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_LIBRARY_X64)
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildBinaryPathName         = os.path.join( buildPathName                      , \
                                                             Program._PATH_NAME_BINARY_RELEASE  )
                 buildLibraryPathName        = os.path.join( buildPathName                      , \
                                                             Program._PATH_NAME_LIBRARY_RELEASE )
                 distributionBinaryPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_BINARY_X86 )
                 distributionLibraryPathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_LIBRARY_X86)
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildBinaryPathName         = os.path.join( buildPathName                    , \
                                                             Program._PATH_NAME_BINARY_DEBUG  )
                 buildLibraryPathName        = os.path.join( buildPathName                    , \
                                                             Program._PATH_NAME_LIBRARY_DEBUG )
                 distributionBinaryPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_BINARY_X64 )
                 distributionLibraryPathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_LIBRARY_X64)
                 
            else :
            
                 buildBinaryPathName         = os.path.join( buildPathName                    , \
                                                             Program._PATH_NAME_BINARY_DEBUG  )
                 buildLibraryPathName        = os.path.join( buildPathName                    , \
                                                             Program._PATH_NAME_LIBRARY_DEBUG )
                 distributionBinaryPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_BINARY_X86 )
                 distributionLibraryPathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_LIBRARY_X86)
                 
            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.makeDirectory(buildPathName)
            
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

            # run MSBuild
            msBuildCommandLine = ( ( "\"%s\" "              + \
                                     "/p:Configuration=%s " + \
                                     "\"%s\""               ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                   ( "Release"                               \
                                     if ( buildSettings.ReleaseSpecified() ) \
                                     else "Debug"                          )                       , \
                                   Program._FILE_NAME_SOLUTION                                     ) )
            msBuildResult = systemManager.execute(msBuildCommandLine)
            if (msBuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute the debug files
            systemManager.distributeFiles( buildLibraryPathName             , \
                                           distributionLibraryPathName      , \
                                           Program._FILE_PATTERN_DEBUG      , \
                                           buildSettings.ReleaseSpecified() , \
                                           True                             )

            # distribute the library files
            systemManager.distributeFiles( buildLibraryPathName             , \
                                           distributionLibraryPathName      , \
                                           Program._FILE_PATTERN_LIBRARY    , \
                                           buildSettings.ReleaseSpecified() , \
                                           True                             )

            # distribute the binary files
            systemManager.distributeFiles( buildBinaryPathName             , \
                                           distributionBinaryPathName      , \
                                           Program._FILE_PATTERN_BINARY    , \
                                           buildSettings.ReleaseSpecified() , \
                                           True                             )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
