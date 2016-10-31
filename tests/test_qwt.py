#------------------------------------------------------------------------------
#
# test_qwt.py
#
# Summary : Builds the QWT library tests.
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
        _PATH_NAME_SOURCE_X86Debug =  "..\\tests\\build-QWT-Desktop_Qt_5_7_0_MSVC2015_32bit-Debug"
        _PATH_NAME_SOURCE_X86Release = "..\\tests\\build-qwt-Desktop_Qt_5_7_0_MSVC2015_32bit-Release"
        _PATH_NAME_SOURCE_X64Debug = "..\\tests\\build-qwt-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug"
        _PATH_NAME_SOURCE_X64Release = "..\\tests\\build-qwt-Desktop_Qt_5_7_0_MSVC2015_64bit-Release"
        _PATH_NAME_DESTINATION = "..\\sdk"
        _PATH_NAME_SDK_DIR = "C:\\DigitalGlobeTemp\\tools\\sdk"
        #Path to your Jom.exe (compiler) directories - modify as necessary for your system
        _PATH_NAME_JOM64_DIR = "C:\\Qt\\Qt5.7.0\\Tools\\QtCreator\\bin\\"
        _PATH_NAME_JOM86_DIR = "C:\\QtX86\\Qt5.7.0\\Tools\\QtCreator\\bin\\"
        #Path to QMake directories - modify as necessary for your system
        _PATH_NAME_QMAKE64_DIR = "C:\\Qt\\Qt5.7.0\\5.7\\msvc2015_64\\bin\\"
        _PATH_NAME_QMAKE86_DIR = "C:\\QtX86\\Qt5.7.0\\5.7\\msvc2015\\bin\\"
        #Path to qwt.pro - modify as necessary for your system
        _PATH_TO_QWT_PRO = "C:\\DigitalGlobeTemp\\tools\\tests\\QWT\\qwt.pro"

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
            
            sourcePathName  = ""
            buildType = ""
            executableDir = ""
            dllsDir = ""
            
            # get the paths
            if ( buildSettings.X64Specified() and buildSettings.ReleaseSpecified()) :
                sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE_X64Release)
                buildType = "\\Release"
                executableDir =  Program._PATH_NAME_SOURCE_X64Release + buildType + "\\QWT.exe"
                cmd = Program._PATH_NAME_QMAKE64_DIR + "qmake.exe " + Program._PATH_TO_QWT_PRO + " -r -spec win32-msvc2015"
                
            elif ( buildSettings.X64Specified()) :
                sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE_X64Debug)
                buildType = "\\Debug"
                executableDir =  Program._PATH_NAME_SOURCE_X64Debug + buildType + "\\QWT.exe"
                cmd =  Program._PATH_NAME_QMAKE64_DIR + "qmake.exe " + Program._PATH_TO_QWT_PRO + " -r -spec win32-msvc2015 " + "CONFIG+=debug " + "CONFIG+=qml_debug"
                
            elif (buildSettings.ReleaseSpecified()) :
                sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE_X86Release)
                buildType = "\\Release"
                executableDir =  Program._PATH_NAME_SOURCE_X86Release + buildType + "\\QWT.exe"
                cmd = Program._PATH_NAME_QMAKE86_DIR + "qmake.exe " + Program._PATH_TO_QWT_PRO + " -r -spec win32-msvc2015"
                
            else:
                sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE_X86Debug)
                buildType = "\\Debug"
                executableDir =  Program._PATH_NAME_SOURCE_X86Debug + buildType + "\\QWT.exe"
                cmd = Program._PATH_NAME_QMAKE86_DIR + "qmake.exe " + Program._PATH_TO_QWT_PRO + " -r -spec win32-msvc2015 " + "CONFIG+=debug " + "CONFIG+=qml_debug"
                                     
            buildDirName = systemManager.getCurrentRelativePathName(sourcePathName)
                                                        
            #get the sdk directory            
            dllsDir = Program._PATH_NAME_SDK_DIR
                
            # extend command line based on options
            if ( buildSettings.X64Specified() ) :                
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X64
                cmd2 = Program._PATH_NAME_JOM64_DIR
            else :                                   
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X86
                cmd2 = Program._PATH_NAME_JOM86_DIR
                
            dllsDir = dllsDir + distribLibs + "\\"
            
            systemManager.changeDirectory(sourcePathName)         
            
            #execute QMake
            res = systemManager.execute(cmd)
            
            # get path to grab the required correct qwt.dll
            if ( buildSettings.X64Specified() and buildSettings.ReleaseSpecified()) :
                dllsDir = dllsDir + "qwt.dll"
                
            elif ( buildSettings.X64Specified()) :
                dllsDir = dllsDir + "qwt_d.dll"
                
            elif (buildSettings.ReleaseSpecified()) :
                dllsDir = dllsDir + "qwt.dll"
                
            else:
                dllsDir = dllsDir + "qwt_d.dll"
                                        
            #modify C:\\DigitalGlobeTemp\\tools as necessary for your setup
            exePath = sourcePathName + buildType + "\\QWT.exe"
            dllsPath = sourcePathName + buildType
        
            #copy required qwt.dll from appropriate sdk folder to Qt output folder
            shutil.copy(dllsDir, dllsPath)
            
            #add compiler        
            cmd2 = cmd2 + "jom.exe -j 4 "
 
            #this code to build QWT.exe is failing if QWT.exe is not already 
            #present on the local machine           
            res = systemManager.execute(cmd2)
            
            #launch the executable            
            os.startfile(exePath)
            
                           
                        
#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
