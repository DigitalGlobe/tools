#------------------------------------------------------------------------------
#
# build_expat.py
#
# Summary : Builds the Expat libraries and exes.
#
#------------------------------------------------------------------------------

import glob
import os
import shutil
#import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
        DESCRIPTION = "Builds Expat libs and exes."
        _PATH_NAME_BINARY_X86 = "\\x86\\bin"
        _PATH_NAME_BINARY_X64 = "\\x64\\bin"
        _PATH_NAME_BUILD = "Expat"
        _PATH_NAME_DISTRIBUTION_X86 = "\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "\\x64\\lib"
        _PATH_NAME_SOURCE = "..\\src\\Expat"
        _PATH_NAME_DESTINATION = "..\\sdk"
        _PATH_NAME_SDK_DIR = "..\\sdk"

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
                # append path for 64-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x64"                 ) )
            else:
                # append path for 32-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x86"                 ) )

            # get the paths
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            buildDirName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SDK_DIR)
           
            # remove build dir
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)

            #copy Expat to the Build area
            systemManager.copyDirectory( sourcePathName, buildPathName)
        
            # start building
            systemManager.changeDirectory(buildPathName)
            cmd = "msbuild Source\\Expat.sln "
            
            compileOutDir = buildPathName + "\\Source"
            # extend command line based on options
            if ( buildSettings.X64Specified() ) :
#                cmd = cmd + "/p:platform=x64 "
                compileOutDir = compileOutDir + "\\x64\\bin"
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X64
                distribExes = Program._PATH_NAME_BINARY_X64
            else :
#                cmd = cmd + "/p:platform=x86 "
                compileOutDir = compileOutDir + "\\Win32\\bin"
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X86
                distribExes = Program._PATH_NAME_BINARY_X86

 
            if buildSettings.ReleaseSpecified():
                cmd = cmd + "/p:configuration=Release "
                compileOutDir = compileOutDir + "\\Release"

            else:
                cmd = cmd + "/p:configuration=Debug "
                compileOutDir = compileOutDir + "\\Debug"
            
            cmdClean = cmd + "/t:clean"
            
            # clean
            systemManager.changeDirectory(buildPathName)
            res = systemManager.execute(cmdClean)
            
            cmd = cmd + "/p:VisualStudioVersion=14.0"

            # compile
            res = systemManager.execute(cmd)
            
            # copy output to appropriate directory
            sdkOutLibsDir = buildDirName + distribLibs
                        
            for file in glob.glob(compileOutDir + "\\*.lib"):
                print( "copying " + file + " -> " + sdkOutLibsDir)
                shutil.copy(file, sdkOutLibsDir)
            for file in glob.glob(compileOutDir + "\\*.dll"):
                print( "copying " + file + " -> " + sdkOutLibsDir)
                shutil.copy(file, sdkOutLibsDir)
            for file in glob.glob(compileOutDir + "\\*.pdb"):
                print( "copying " + file + " -> " + sdkOutLibsDir)
                shutil.copy(file, sdkOutLibsDir)
                
            sdkOutExesDir = buildDirName + distribExes
                        
            for file in glob.glob(compileOutDir + "\\*.exe"):
                print( "copying " + file + " -> " + sdkOutExesDir)
                shutil.copy(file, sdkOutExesDir)



#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
