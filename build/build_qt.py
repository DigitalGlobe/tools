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
        
        _FIREBIRD_BASE = '..\\..\\firebird'
        _FIREBIRD_INCLUDE = 'include'
        _FIREBIRD_LIB_PATH = 'lib'
        _FIREBIRD_LIB = 'fbclient_ms'
        
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

            # Append  qtbase/bin to the path
            qtBaseBin = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD) + "\\qtbase\\bin"
            systemManager.appendToPathEnvironmentVariable( qtBaseBin)
            
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

            systemManager.makeDirectory(buildPathName)
            # print("copying source to build dir")
            # systemManager.copyDirectory( sourcePathName, buildPathName)
        
            # start building
            systemManager.changeDirectory(buildPathName)

            # nmake --------------------------------------------------------------------------------
            #cmd = "nmake clean"
            #print( "command: " + cmd)
            #result = systemManager.execute( cmd)
            #if (result != 0) :
            #    sys.exit(-1)


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

            firebirdBase = os.path.join(os.path.join(buildPathName, Program._FIREBIRD_BASE), ('x64' if buildSettings.X64Specified() else 'x86'))
            firebirdInclude = os.path.join(firebirdBase, Program._FIREBIRD_INCLUDE)
            firebirdLib = os.path.join(firebirdBase, Program._FIREBIRD_LIB_PATH)
            
            os.environ['QMAKE_INCDIR_IBASE'] = firebirdInclude
            os.environ['QMAKE_LIBDIR_IBASE'] = firebirdLib
            os.environ['QMAKE_LIBS_IBASE'] = Program._FIREBIRD_LIB

            
            cmdConfigure = os.path.join(sourcePathName, 'configure')
            cmdConfigure += ' -prefix ' + binDir + ' ' + buildType + ' -mp ' + \
                ' -opensource -confirm-license -opengl dynamic -no-compile-examples -nomake examples -nomake tests -no-icu ' \
                ' -skip 3d -skip datavis3d -skip webengine -platform ' + plat + \
                ' -plugin-sql-ibase -I "' + firebirdInclude + '" -L "' + firebirdLib + '"' #  -l ' + Program._FIREBIRD_LIB

                # ' -D "QMAKE_INCDIR_IBASE=' + firebirdInclude + '"' + \
                # ' -D "QMAKE_LIBDIR_IBASE=' + firebirdLib + '"' + \
                # ' -D "QMAKE_LIBS_IBASE=' + Program._FIREBIRD_LIB + '"'

                # -I "' + firebirdInclude + '" -L "' + firebirdLib + '" -l ' + Program._FIREBIRD_LIB
                
            print( "configure: " + cmdConfigure)
            # sys.exit(-1)
            
            result = systemManager.execute( cmdConfigure)            
            if (result != 0) :
                sys.exit(-1)

			# nmake --------------------------------------------------------------------------------
            cmd = "nmake"
            print( "command: " + cmd)
            result = systemManager.execute( cmd)
            if (result != 0) :
                sys.exit(-1)
            
            # install -------------------------------------------------------------------------------
            cmdInstall = "nmake install"
            print( "command: " + cmdInstall)
            result = systemManager.execute( cmdInstall)
            if (result != 0) :
                sys.exit(-1)

            
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
