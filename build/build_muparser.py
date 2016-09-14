#------------------------------------------------------------------------------
#
# build_muparser.py
#
# Summary : Builds the muparser expression parser
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
        DESCRIPTION = "Builds muparser."
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        _PATH_NAME_BUILD = "muparser"
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        _PATH_NAME_SOURCE = "..\\src\\muparser"

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
            cmd = "msbuild build\\msvc2013\\muparser.sln "
            cmdLib = cmd;
            
            buildOutDir = ""

            # extend command line based on options
            if ( buildSettings.X64Specified() ) :
                cmd = cmd + "/p:platform=x64 "
                cmdLib = cmdLib + "/p:platform=x64 "
                distribPath = Program._PATH_NAME_DISTRIBUTION_X64
                baseOutName = "muparser64"

            else :
                cmd = cmd + "/p:platform=Win32 "
                cmdLib = cmdLib + "/p:platform=Win32 "
                distribPath = Program._PATH_NAME_DISTRIBUTION_X86
                baseOutName = "muparser32"

            if buildSettings.ReleaseSpecified():
                cmd = cmd + "/p:configuration=\"Release DLL\" "
                cmdLib = cmdLib + "/p:configuration=\"Release Static\" "
                suffix = ""
            else:
                cmd = cmd + "/p:configuration=\"Debug DLL\" "
                cmdLib = cmdLib + "/p:configuration=\"Debug Static\" "
                suffix = "_d"

            buildOutDir = buildPathName + "\\lib"
            
            print("build output dir: " + buildOutDir)

            sdkOutDir = buildPathName + "\\..\\" + distribPath

            cmdClean = cmd + "/t:clean"
            
            print("command is: " + cmd)
            print("clean command is: " + cmdClean)
            print("buid output path is: " + buildOutDir)
            print("sdk out is: " + sdkOutDir)


            # execute clean
            print("executing clean")
            systemManager.changeDirectory(buildPathName)
            res = systemManager.execute(cmdClean)

            # build DLL
            print("executing compile")
            res = systemManager.execute(cmd)

            # copy output to appropriate bin dir
            print("Copy files into SDK dir")
            shutil.copy(buildOutDir + "\\" + baseOutName + suffix + ".dll", sdkOutDir); 


            # Build Static
            print("executing compile")
            res = systemManager.execute(cmdLib)

            print("Copy files into SDK dir")
            shutil.copy(buildOutDir + "\\" + baseOutName + suffix + ".lib", sdkOutDir); 
            shutil.copy(buildOutDir + "\\" + baseOutName + suffix + ".pdb", sdkOutDir);                         
            
            
            
            
            
            
            
            


#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
