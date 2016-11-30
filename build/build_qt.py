#------------------------------------------------------------------------------
#
# build_qt.py
#
# Summary : Builds Qt libs
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
        DESCRIPTION = "Builds Qt."
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        _PATH_NAME_BUILD = "Qt"
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        _PATH_NAME_SOURCE = "..\\src\\Qt"

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
                #systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                #                                                             "Windows Kits\\10\\bin\\x64"                 ) )
                                                                             
                #this was conflicting with another RC on my machine, so it gets pre-pendended to the path instead of appended
                                                                             
                pth = os.path.join( systemManager.getProgramFilesPathName(False) , "Windows Kits\\10\\bin\\x64") 
                os.environ["PATH"] = pth + ";" + os.environ["PATH"]
                                                                             
            else:
                print("32bit Build")

                # append path for 32-bit rc.exe
                #systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                #
                #                                                             "Windows Kits\\10\\bin\\x86"                 ) )
                #this was conflicting with another RC on my machine, so it gets pre-pendended to the path instead of appended
                                                                             
                pth = os.path.join( systemManager.getProgramFilesPathName(False) , "Windows Kits\\10\\bin\\x86") 
                os.environ["PATH"] = pth + ";" + os.environ["PATH"]


			# append gnuwin32 path for flex and bison
			# Qt needs specific versions, so they are placed within the Qt source tree
            gnuToolsPath = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD) + "\\gnuwin32\\bin"
            systemManager.appendToPathEnvironmentVariable( gnuToolsPath)

            # determine path names
            print("Getting Paths")
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            binPathName  = systemManager.getCurrentRelativePathName("..\\Qt\\5.7")
            
            print("build path: " + buildPathName)
            print("source path: " + sourcePathName)
            print("install path: " + binPathName)
            
            
            # initialize directories
            print("removing previous build dir")
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)

            print("copying source to build dir")
            systemManager.copyDirectory( sourcePathName, buildPathName)
        
            # start building
            systemManager.changeDirectory(buildPathName)


			# configure ----------------------------------------------------------------------------
            #binDir = "d:\\qtbase"
            #binDir = buildPathName + "\\build"
            binDir = binPathName
            if ( buildSettings.X64Specified() ) :
                binDir = binDir + "\\x64"
                plat = "win32-msvc2015"
            else:
                binDir = binDir + "\\x86"
                plat = "win32-msvc2015"
                
            if buildSettings.ReleaseSpecified():
                buildType = "-release"
            else:
                buildType = "-debug"	

            cmdConfigure = "configure -prefix " + binDir + " " + buildType + " -opensource -confirm-license -opengl desktop -nomake examples -skip qt3d -skip qtdatavis3d -platform " + plat
            print( "configure: " + cmdConfigure)
            
            res = systemManager.execute( cmdConfigure)



			# nmake --------------------------------------------------------------------------------
            cmd = "nmake "
            print( "command: " + cmd)
            res = systemManager.execute( cmd)
            
            
            # install -------------------------------------------------------------------------------
            cmdInstall = "nmake install"
            print( "command: " + cmdInstall)
            res = systemManager.execute( cmdInstall)

            
            # copy ---------------------------------------------------------------------------------
            # Leaving QT files in their normal structure, not copying them into the sdk dir
#            if ( buildSettings.X64Specified() ) :
#                distribPath = Program._PATH_NAME_DISTRIBUTION_X64
#            else:
#                distribPath = Program._PATH_NAME_DISTRIBUTION_X86
#            
#            sdkOutDir = buildPathName + "\\..\\" + distribPath
#
#
#            buildOutDir = binDir
#            print("Copy files into SDK dir")
#            for file in glob.glob(buildOutDir + "\\bin\\Qt5*.dll"):
#                print( "copying " + file + " -> " + sdkOutDir)
#                shutil.copy(file, sdkOutDir)
#            for file in glob.glob(buildOutDir + "\\bin\\Qt5*.pdb"):
#                print( "copying " + file + " -> " + sdkOutDir)
#                shutil.copy(file, sdkOutDir)
#            for file in glob.glob(buildOutDir + "\\lib\\Qt5*.lib"):
#                print( "copying " + file + " -> " + sdkOutDir)
#                shutil.copy(file, sdkOutDir)
#            for file in glob.glob(buildOutDir + "\\lib\\Qt5*.pdb"):
#                print( "copying " + file + " -> " + sdkOutDir)
#                shutil.copy(file, sdkOutDir)
#            for file in glob.glob(buildOutDir + "\\bin\\lib*.dll"):
#                print( "copying " + file + " -> " + sdkOutDir)
#                shutil.copy(file, sdkOutDir)
#            for file in glob.glob(buildOutDir + "\\bin\\lib*.pdb"):
#                print( "copying " + file + " -> " + sdkOutDir)
#                shutil.copy(file, sdkOutDir)
                
            # may need to also copy in some plugins from their dirs
            

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
