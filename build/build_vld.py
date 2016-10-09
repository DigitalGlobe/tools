#------------------------------------------------------------------------------
#
# build_vld.py
#
# Summary : Builds Virtual Leak Detector
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
        DESCRIPTION = "Builds LibGIST."
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        _PATH_NAME_BUILD = "vld"
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        _PATH_NAME_SOURCE = "..\\src\\vld"

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
            cmd = "msbuild vld_vs14_wo_mfc.sln "
            cmdStatic = "msbuild vld_static.sln "
            
            buildOutDir = ""
            buildOutConfig = ""
            buildOutPlat = ""

            # extend command line based on options
            if ( buildSettings.X64Specified() ) :
                cmd = cmd + "/p:platform=x64 "
                cmdStatic = cmdStatic + "/p:platform=x64 "
                distribPath = Program._PATH_NAME_DISTRIBUTION_X64
                buildOutPlat = "x64"
                FileSuffix = "x64"
            else :
                cmd = cmd + "/p:platform=Win32 "
                cmdStatic = cmdStatic + "/p:platform=Win32 "
                distribPath = Program._PATH_NAME_DISTRIBUTION_X86
                buildOutPlat = "Win32"
                FileSuffix = "x86"

            if buildSettings.ReleaseSpecified():
                cmd = cmd + "/p:configuration=\"Release\" "
                cmdStatic = cmdStatic + "/p:configuration=\"Release\" "
                suffix = ""
                buildOutConfig = "Release"
            else:
                cmd = cmd + "/p:configuration=\"Debug\" "
                cmdStatic = cmdStatic + "/p:configuration=\"Debug\" "
                suffix = "_d"
                buildOutConfig = "Debug"

            buildOutDir = buildPathName + "\\src\\bin\\" + buildOutPlat + "\\" + buildOutConfig + "-v140"
            
            print("build output dir: " + buildOutDir)

            sdkOutDir = buildPathName + "\\..\\" + distribPath

            cmdClean = cmd + "/t:clean"
            cmdStaticClean = cmdStatic + "/t: clean"
            
            print("command is: " + cmd)
            print("clean command is: " + cmdClean)
            print("buid output path is: " + buildOutDir)
            print("sdk out is: " + sdkOutDir)


			#DLL
            # execute clean
            print("executing clean")
            systemManager.changeDirectory(buildPathName)
            res = systemManager.execute(cmdClean)

            # build -------------------------------------------------------------------------------
            print("executing compile")
            res = systemManager.execute(cmd)

            # copy output to appropriate bin dir
            print("Copy files into SDK dir")
            for file in glob.glob(buildOutDir + "\\vld_" + FileSuffix + suffix + ".dll"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)
            for file in glob.glob(buildOutDir + "\\vld_" + FileSuffix + suffix + ".lib"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)
            for file in glob.glob(buildOutDir + "\\vld_" + FileSuffix + suffix + ".pdb"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)


			#Static -------------------------------------------------------------------------------
            # execute clean
            print("executing clean")
            systemManager.changeDirectory(buildPathName)
            res = systemManager.execute(cmdClean)

            # build 
            print("executing compile")
            res = systemManager.execute(cmdStatic)

            # copy output to appropriate bin dir
            print("Copy files into SDK dir")
            for file in glob.glob(buildOutDir + "\\vldstatic_" + FileSuffix + suffix + ".dll"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)
            for file in glob.glob(buildOutDir + "\\vldstatic_" + FileSuffix + suffix + ".lib"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)
            for file in glob.glob(buildOutDir + "\\vldstatic_" + FileSuffix + suffix + ".pdb"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)
			
            

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
