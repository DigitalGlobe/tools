#------------------------------------------------------------------------------
#
# build_gdal.py
#
# Summary : Builds the GDAL Library
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
        DESCRIPTION = "Builds the GDAL library"
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        _PATH_NAME_BUILD = "GDAL"
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        _PATH_NAME_SOURCE = "..\\src\\GDAL"

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
            cmd = "nmake -f makefile.vc MSVC_VER=1900 "
            suffix = ""
            
            # extend command line based on options
            if ( buildSettings.X64Specified() ) :
                cmd = cmd + "WIN64=YES "
                distribPath = Program._PATH_NAME_DISTRIBUTION_X64
            else :
                distribPath = Program._PATH_NAME_DISTRIBUTION_X86


            if buildSettings.ReleaseSpecified():
                suffix = ""
            else:
                cmd = cmd + "DEBUG=1 "
                suffix = "_d"
            
            sdkOutDir = buildPathName + "\\..\\" + distribPath

            cmdClean = cmd + "clean"
            
            print("command is: " + cmd)
            print("clean command is: " + cmdClean)


            #restoring required nmake.opt file
            shutil.copy( "nmake_old.opt", "nmake.opt")

            # execute clean
            print("executing clean")
            systemManager.changeDirectory(buildPathName)
            res = systemManager.execute(cmdClean)

            # build
            #print("executing compile")
            res = systemManager.execute(cmd)

            # copy output to appropriate bin dir
            print("Copy files into SDK dir")
            shutil.copy( buildPathName + "\\gdal201.dll", sdkOutDir + "\\gdal201" + suffix + ".dll")
            shutil.copy( buildPathName + "\\gdal_i.exp", sdkOutDir + "\\gdal_i" + suffix + ".exp")
            shutil.copy( buildPathName + "\\gdal_i.lib", sdkOutDir + "\\gdal_i" + suffix + ".lib")
            shutil.copy( buildPathName + "\\gdal.lib", sdkOutDir + "\\gdal" + suffix + ".lib")

            #for file in glob.glob(compileOutDir + "\\*.lib"):
            #    print( "copying " + file + " -> " + sdkOutDir)
            #    shutil.copy(file, sdkOutDir)
            #for file in glob.glob(compileOutDir + "\\*.dll"):
            #    print( "copying " + file + " -> " + sdkOutDir)
            #    shutil.copy(file, sdkOutDir)
            #for file in glob.glob(compileOutDir + "\\*.pdb"):
            #    print( "copying " + file + " -> " + sdkOutDir)
            #    shutil.copy(file, sdkOutDir)



#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
