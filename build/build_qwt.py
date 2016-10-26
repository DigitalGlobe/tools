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
        _PATH_NAME_QMAKE64_DIR = "C:\\Qt\\Qt5.7.0\\5.7\\msvc2015_64\\bin\\"
        _PATH_NAME_QMAKE86_DIR = "C:\\QtX86\\Qt5.7.0\\5.7\\msvc2015\\bin\\"
        #Path to qwt.pro - modify as necessary for your system
        _PATH_TO_QWT_PRO = "C:\\DigitalGlobeTemp\\tools\\src\\QWT\\qwt.pro"

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

            compileOutLib = ""
            sourcePathName  = ""
            
            # get the paths
            if ( buildSettings.X64Specified() and buildSettings.ReleaseSpecified()) :
                sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE_X64Release)
                cmd = Program._PATH_NAME_QMAKE64_DIR + "qmake.exe " + Program._PATH_TO_QWT_PRO + " -r -spec win32-msvc2015"
            elif ( buildSettings.X64Specified()) :
                sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE_X64Debug)
                cmd =  Program._PATH_NAME_QMAKE64_DIR + "qmake.exe " + Program._PATH_TO_QWT_PRO + " -r -spec win32-msvc2015 " + "CONFIG+=debug " + "CONFIG+=qml_debug"
            elif (buildSettings.ReleaseSpecified()) :
                sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE_X86Release)
                cmd = Program._PATH_NAME_QMAKE86_DIR + "qmake.exe " + Program._PATH_TO_QWT_PRO + " -r -spec win32-msvc2015"
            else:
                sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE_X86Debug)
                cmd = Program._PATH_NAME_QMAKE86_DIR + "qmake.exe " + Program._PATH_TO_QWT_PRO + " -r -spec win32-msvc2015 " + "CONFIG+=debug " + "CONFIG+=qml_debug"
                                     
            buildDirName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SDK_DIR)
                                            
            compileOutLib = sourcePathName + "\\lib"
                
            # extend command line based on options
            if ( buildSettings.X64Specified() ) :                
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X64
                cmd2 = Program._PATH_NAME_JOM64_DIR
            else :                                   
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X86
                cmd2 = Program._PATH_NAME_JOM86_DIR

            systemManager.changeDirectory(sourcePathName)
            
            res = systemManager.execute(cmd)
                                   
            cmd2 = cmd2 + "jom.exe -j 4"
                        
            res = systemManager.execute(cmd2)
            
            # copy output to appropriate directory
            sdkOutLibsDir = buildDirName + distribLibs
                                    
            for file in glob.glob(compileOutLib + "\\*.lib"):
                print( "copying " + file + " -> " + sdkOutLibsDir)
                shutil.copy(file, sdkOutLibsDir)
                            
            for file in glob.glob(compileOutLib + "\\*.dll"):
                print( "copying " + file + " -> " + sdkOutLibsDir)
                shutil.copy(file, sdkOutLibsDir)
                
            for file in glob.glob(compileOutLib + "\\*.pdb"):
                print( "copying " + file + " -> " + sdkOutLibsDir)
                shutil.copy(file, sdkOutLibsDir)
                        
#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
