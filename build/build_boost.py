#------------------------------------------------------------------------------
#
# build_boost.py
#
# Summary : Builds the Boost libraries.
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
        DESCRIPTION = "Builds the Boost libraries."
        #----------------------------------------------------------------------
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit binary files
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit binary files
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "Boost"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\Boost"
        #----------------------------------------------------------------------
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = 'boost'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\boost'
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
                print("64bit Build")

                # append path for 64-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x64"                 ) )
                
            else:
                print("32bit Build")

                # append path for 32-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x86"                 ) )

            # determine path names
            print("Getting Paths")
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            print("build path: " + buildPathName)
            print("source path: " + sourcePathName)

            # initialize directories
            print("removing previous build dir")
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)

            print("copying source to build dir")
            systemManager.copyDirectory( sourcePathName, buildPathName)
            
            # Build the Boost build executable, b2.exe
            print("building boost builder")
            buildBoostResult = systemManager.execute("bootstrap.bat")

            print("generating final compile command")
            buildBoostCommandLine =  "b2 toolset=msvc-14.0 -a --abbreviate-paths architecture=x86 --prefix=../../build/Boost install link=shared runtime-link=shared -j4"
                                         
            # extend command line based on options
            if ( buildSettings.X64Specified() ) :
                buildBoostCommandLine = buildBoostCommandLine + " address-model=64 --libdir=../../sdk/x64/lib"
            else :
                buildBoostCommandLine = buildBoostCommandLine + " address-model=32 --libdir=../../sdk/x86/lib"

            if buildSettings.ReleaseSpecified():
                buildBoostCommandLine = buildBoostCommandLine + " variant=release"
            else:
                buildBoostCommandLine = buildBoostCommandLine + " variant=debug"

            print("command is: " + buildBoostCommandLine)

            # execute final build command for this option set
            # this will move files to the proper, no additional copying necessary
            print("executing compile")
            systemManager.changeDirectory(buildPathName)
            buildBoostResult = systemManager.execute(buildBoostCommandLine)
        
            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                                          os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                                          '*.hpp',                                                                 \
                                          True, False, True) 
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                                          os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                                          '*.h',                                                                 \
                                          True, False, True) 
        

        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
