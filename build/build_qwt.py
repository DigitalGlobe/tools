#------------------------------------------------------------------------------
#
# build_qwt.py
#
# Summary : Builds the QWT libraries.
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
        DESCRIPTION = "Builds QWT libs and exes."
        _PATH_NAME_BINARY_X86 = "\\x86\\bin"
        _PATH_NAME_BINARY_X64 = "\\x64\\bin"
        _PATH_NAME_BUILD = "QWT"
        _PATH_NAME_DISTRIBUTION_X86 = "\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "\\x64\\lib"
        _PATH_NAME_SOURCE_X86Debug = "..\\src\\build-qwt-Desktop_Qt_5_7_0_MSVC2015_32bit-Debug"
        _PATH_NAME_SOURCE_X86Release = "..\\src\\build-qwt-Desktop_Qt_5_7_0_MSVC2015_32bit-Release"
        _PATH_NAME_SOURCE_X64Debug = "..\\src\\build-qwt-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug"
        _PATH_NAME_SOURCE_X64Release = "..\\src\\build-qwt-Desktop_Qt_5_7_0_MSVC2015_64bit-Release"
        _PATH_NAME_DESTINATION = "..\\sdk"
        _PATH_NAME_SDK_DIR = "..\\sdk"
        #Path to your Jom.exe (compiler) directories - modify as necessary for your system
        _PATH_NAME_JOM64_DIR = "C:\\Qt\\Qt5.7.0\\Tools\\QtCreator\\bin\\"
        _PATH_NAME_JOM86_DIR = "C:\\QtX86\\Qt5.7.0\\Tools\\QtCreator\\bin\\"
        #Path to QMake directories - modify as necessary for your system
        #_PATH_NAME_QMAKE64_DIR = "C:\\Qt\\Qt5.7.0\\5.7\\msvc2015_64\\bin\\"
        _PATH_NAME_QMAKE64_DIR = "..\\build\\Qt\\build\\x64\\bin\\"
        _PATH_NAME_QMAKE86_DIR = "..\\build\\Qt\\build\\x86\\bin\\"
        #Path to qwt.pro - modify as necessary for your system
        _PATH_TO_QWT_PRO = "C:\\DigitalGlobeTemp\\tools\\src\\QWT\\qwt.pro"
        _PATH_NAME_SOURCE = "..\\src\\QWT"


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

            sdkDirName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SDK_DIR)
            if ( buildSettings.X64Specified() ) :
                print("64bit Build")
                pth = os.path.join( systemManager.getProgramFilesPathName(False) , "Windows Kits\\10\\bin\\x64") 
                os.environ["PATH"] = pth + ";" + os.environ["PATH"]
                libPath = sdkDirName + Program._PATH_NAME_DISTRIBUTION_X64
                os.environ["LIBPATH"] = libPath 
                #+ ";" + os.environ["LIBPATH"]
                                                                             
            else:
                print("32bit Build")
                pth = os.path.join( systemManager.getProgramFilesPathName(False) , "Windows Kits\\10\\bin\\x86") 
                os.environ["PATH"] = pth + ";" + os.environ["PATH"]


            # get the paths
            print("Getting Paths")
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            #qt = os.environ.get("QT_PATH")
            #print( "QT Path in Env: " + qt)
            #if ( len(qt) > 0):
            #	if ( buildSettings.X64Specified() ) :
            #        qmakePath = qt + "\\5.7\\msvc2015_64\\bin\\qmake.exe " + buildPathName + "\\qwt.pro"
           # 	else:
           #         qmakePath = qt + "\\5.7\\msvc2015\\bin\\qmake.exe " + buildPathName + "\\qwt.pro"
           # else:
            #    print("Calculating Qt Path")
            #    if ( buildSettings.X64Specified() ) :
            #        qmakePath =      systemManager.getCurrentRelativePathName(Program._PATH_NAME_QMAKE64_DIR) + "\\qmake.exe " + buildPathName + "\\qwt.pro"
            #    else:
            #        qmakePath =      systemManager.getCurrentRelativePathName(Program._PATH_NAME_QMAKE86_DIR) + "\\qmake.exe " + buildPathName + "\\qwt.pro"		        
            
            print("Calculating Qt Path")
            if ( buildSettings.X64Specified() ) :
                qmakePath =      systemManager.getCurrentRelativePathName( "..\\QT\\5.7\\x64\\bin") + "\\qmake.exe " + buildPathName + "\\qwt.pro"
            else:
                qmakePath =      systemManager.getCurrentRelativePathName( "..\\QT\\5.7\\x86\\bin") + "\\qmake.exe " + buildPathName + "\\qwt.pro"		        
            
		            
            compileOutLib = ""
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
            
          	# generate the qmake command
            cmdMake = ""          
            if ( buildSettings.X64Specified()):
            	cmdMake = qmakePath + " -r -spec win32-msvc2015 "
            else:
            	cmdMake = qmakePath + " -r -spec win32-msvc2015 "
            	
            if (buildSettings.ReleaseSpecified()):
            	copyMask = "*.*"
            else:
            	copyMask = "*_d.*"
            	cmdMake = cmdMake + "CONFIG+=debug " + "CONFIG+=qml_debug"
            	
            print("qmake: " + cmdMake)
            
			#do qmake
            res = systemManager.execute( cmdMake)



            #do the build
            cmd = "nmake"
            res = systemManager.execute( cmd)

			#copy files
            if ( buildSettings.X64Specified() ) :
                distribPath = Program._PATH_NAME_DISTRIBUTION_X64
            else:
                distribPath = Program._PATH_NAME_DISTRIBUTION_X86
            
            sdkOutDir = sdkDirName + distribPath
			
            print( "copying files: " + buildPathName + "\\lib\\" + copyMask)
			
            for file in glob.glob(buildPathName + "\\lib\\" + copyMask):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)
			

                        
#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
