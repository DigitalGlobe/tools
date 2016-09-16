#------------------------------------------------------------------------------
#
# build_galib.py
#
# Summary : Builds the GALib  / Genetic Algorithm Components
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
        DESCRIPTION = "Builds the GALib library."
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        _PATH_NAME_BUILD = "GALib"
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        _PATH_NAME_SOURCE = "..\\src\\GALib"

        def __init__(self) :
        
            pass
        #----------------------------------------------------------------------
        

        def main(self) :
        
            systemManager = SystemManager()
            pathFinder    = PathFinder()
            
            # process command-line arguments
            buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)
            
            # initialize environment variables
            systemManager.initializeIncludeEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.initializeLibraryEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.appendToPathEnvironmentVariable( pathFinder.getVisualStudioBinPathName( buildSettings.X64Specified() ) )

            # MSBuild is under "Program Files (x86)"
            systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "MSBuild\\14.0\\Bin") )

            compileOutDir = ""
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
        
            # start building
            systemManager.changeDirectory(buildPathName)
            cmd = "nmake /f makefile.vcpp "
            
            buildOutDir = buildPathName + "\\ga"

            # extend command line based on options
            if ( buildSettings.X64Specified() ) :
                distribPath = Program._PATH_NAME_DISTRIBUTION_X64
                cmd = cmd + "WIN64=YES "
            else :
                distribPath = Program._PATH_NAME_DISTRIBUTION_X86

            suffix = ""
            copyPDB = False
            if buildSettings.ReleaseSpecified():
                cmd = cmd + " RELEASE_BUILD=YES "
                suffix = ""
            else:
                suffix = "_d"
                copyPDB = True

            cmdDLL = cmd + " SHARED_BUILD=YES LIBRARY_BUILD=YES"

            
            print("build output dir: " + buildOutDir)

            sdkOutDir = buildPathName + "\\..\\" + distribPath
            cmdClean = cmd + " clean"
            
            print("command is: " + cmd)
            print("clean command is: " + cmdClean)
            print("compile output path is: " + compileOutDir)
            print("distrib path is: " + distribPath)
            print("sdk out dir is: " + sdkOutDir)



            # execute clean
            print("executing clean")
            systemManager.changeDirectory(buildPathName)
            res = systemManager.execute(cmdClean)

            # build lib
            print("executing compile")
            res = systemManager.execute(cmd)

            # copy output to appropriate bin dir
            #print("Copy files into SDK dir")
            shutil.copy(buildOutDir + "\\ga" + suffix + ".lib", sdkOutDir)
            if copyPDB:
                shutil.copy(buildOutDir + "\\ga" + suffix + ".pdb", sdkOutDir)
            
            


#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
