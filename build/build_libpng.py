#------------------------------------------------------------------------------
#
# build_libpng.py
#
# Summary : Builds the LibPNG library.
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
        DESCRIPTION = "Builds the LibPNG library."
        #----------------------------------------------------------------------
        # the name of the build debug library file
        _FILE_NAME_LIBRARY_BUILD_DEBUG = "libpng16_staticd.lib"
        #----------------------------------------------------------------------
        # the name of the build release library file
        _FILE_NAME_LIBRARY_BUILD_RELEASE = "libpng16_static.lib"
        #----------------------------------------------------------------------
        # the name of the distribution debug library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG = "libpng_d.lib"
        #----------------------------------------------------------------------
        # the name of the distribution release library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE = "libpng.lib"
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "libpng.sln"
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
        _PATH_NAME_BUILD = "LibPNG"
        #----------------------------------------------------------------------
        # the name of the path that will contain debug build files
        _PATH_NAME_BUILD_DEBUG = "Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain release build files
        _PATH_NAME_BUILD_RELEASE = "Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\LibPNG"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the x86 debug ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X86_DEBUG = "..\\sdk\\x86\\lib\\zlib_d.lib"
        #----------------------------------------------------------------------
        # the name of the x86 release ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X86_RELEASE = "..\\sdk\\x86\\lib\\zlib.lib"
        #----------------------------------------------------------------------
        # the name of the x64 debug ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X64_DEBUG = "..\\sdk\\x64\\lib\\zlib_d.lib"
        #----------------------------------------------------------------------
        # the name of the x64 release ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X64_RELEASE = "..\\sdk\\x64\\lib\\zlib.lib"
        #----------------------------------------------------------------------
        # the name of the ZLib include path
        _PATH_NAME_ZLIB_INCLUDE = "..\\src\\ZLib"
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
            binaryPathName      = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64) \
                                    if ( buildSettings.X64Specified() )                                     \
                                    else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName       = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD       )
            sourcePathName      = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE      )
            zlibIncludePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_ZLIB_INCLUDE)
            
            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X64_RELEASE)
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X86_RELEASE)
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X64_DEBUG)
                 
            else :
            
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X86_DEBUG)
                 
            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.makeDirectory(buildPathName)
                               
            # run CMake
            cmakeCommandLine = ( ( "\"%s\" "                    + \
                                   "-DZLIB_INCLUDE_DIR=\"%s\" " + \
                                   "-DZLIB_LIBRARY=\"%s\" "     + \
                                   "\"%s\""                     ) % \
                                 ( PathFinder.FILE_NAME_CMAKE , \
                                   zlibIncludePathName        , \
                                   zlibLibraryFileName        , \
                                   sourcePathName             ) )
            systemManager.changeDirectory(buildPathName)
            cmakeResult = systemManager.execute(cmakeCommandLine)
            if (cmakeResult != 0) :
            
                sys.exit(-1)

            # build the solution
            msBuildCommandLine = ( ( "\"%s\" "              + \
                                     "/p:Configuration=%s " + \
                                     "/t:png_static "       + \
                                     "\"%s\""               ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                   ( "Release"                               \
                                     if ( buildSettings.ReleaseSpecified() ) \
                                     else "Debug"                          )                       , \
                                   Program._FILE_NAME_SOLUTION                                     ) )
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute the library file
            systemManager.copyFile( buildLibraryFileName        , \
                                    distributionLibraryFileName )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
