#------------------------------------------------------------------------------
#
# build_osg.py
#
# Summary : Builds OpenSceneGraph
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
        DESCRIPTION = "Builds OpenSceneGraph."
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        _PATH_NAME_BUILD = "OpenSceneGraph"
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        _PATH_NAME_SOURCE = "..\\src\\OpenSceneGraph"

        # the name of the path for all include files
        _PATH_NAME_INCLUDE = 'include'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\osg'
        #----------------------------------------------------------------------
        
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
        
        
            # Call CMAKE to generate the correct VS solution files
            if ( buildSettings.X64Specified() ) :
                cmakeCommand = "cmake -G \"Visual Studio 14 2015 Win64\"" 
                target="x64"
            else:
                cmakeCommand = "cmake -G \"Visual Studio 14 2015\""
                target="x86"

            cmakeCommand = cmakeCommand + ' -DCMAKE_SOURCE_DIR="' + buildPathName + '"'

            if buildSettings.ReleaseSpecified():
                cmakeCommand = cmakeCommand +  " -DCMAKE_BUILD_TYPE=\"Release\" "
                libSuffix = ""
            else:
                cmakeCommand = cmakeCommand +  " -DCMAKE_BUILD_TYPE=\"Debug\" "
                libSuffix = "_d"

            #Add CMAKE Vars for 3rd party libs
            #GDAL
            cmakeCommand = cmakeCommand + ' -DGDAL_INCLUDE_DIRS="' + buildPathName + '../../include/gdal/include" '
            cmakeCommand = cmakeCommand + ' -DGDAL_LIBRARY="' + buildPathName + '../../sdk/' + target + 'gdal' + libSuffix + '.lib'
            #FreeType
            cmakeCommand = cmakeCommand + " -DFREETYPE_INCLUDE_DIRS=\"../FreeType/include/freetype/\" "
            cmakeCommand = cmakeCommand + " -DFREETYPE_LIBRARY=\"../../sdk/" + target + "/lib/freetype" + libSuffix + ".lib\" "
            #JPEG
            cmakeCommand = cmakeCommand + " -DJPEG_INCLUDE_DIR=\"../libjpeg/\" "
            cmakeCommand = cmakeCommand + " -DJPEG_LIBRARY=\"../../sdk/" + target + "/lib/libjpeg" + libSuffix + ".lib\" "
            #Tiff
            cmakeCommand = cmakeCommand + ' -DTIFF_INCLUDE_DIRS="' + buildPathName + '../../include/libtiff/include" '
            cmakeCommand = cmakeCommand + ' -DTIFF_LIBRARY="' + buildPathName + '../../sdk/' + target + 'libtiff' + libSuffix + '.lib'


            print( "cmake command: " + cmakeCommand)        
        
            # Run CMAKE
            res = systemManager.execute(cmakeCommand)

            # MSBUILD                
            cmd = "msbuild OpenSceneGraph.sln "
            
            # extend command line based on options
            if ( buildSettings.X64Specified() ) :
                cmd = cmd + "/p:platform=x64 "
                distribPath = Program._PATH_NAME_DISTRIBUTION_X64
            else :
                cmd = cmd + "/p:platform=Win32 "
                distribPath = Program._PATH_NAME_DISTRIBUTION_X86


            if buildSettings.ReleaseSpecified():
                cmd = cmd + "/p:configuration=Release "

            else:
                cmd = cmd + "/p:configuration=Debug "
            
            libOutDir = buildPathName + "\\lib"  # libs
            exeOutDir = buildPathName + "\\bin"  # exes and dlls
            #also need plugin dir
            
            print("lib output dir: " + libOutDir)
            print("exe output dir: " + exeOutDir)

            sdkOutDir = buildPathName + "\\..\\" + distribPath

            cmdClean = cmd + "/t:clean"
            
            print("command is: " + cmd)
            print("clean command is: " + cmdClean)


            # execute clean
            print("executing clean")
            systemManager.changeDirectory(buildPathName)
            res = systemManager.execute(cmdClean)

            # build
            print("executing compile")
            res = systemManager.execute(cmd)

            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                                          os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                                          '*',                                                                 \
                                          True, False, True) 

            
            # copy output to appropriate bin dir
            print("Copy files into SDK dir")

            #libs 
            for file in glob.glob(libOutDir + "\\*.lib"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)
            for file in glob.glob(libOutDir + "\\*.exp"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)

            #lib plugins
            for file in glob.glob(libOutDir + "\\osgPlugins-3.4.0\\*.lib"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)
            for file in glob.glob(libOutDir + "\\osgPlugins-3.4.0\\*.exp"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)

            #DLL's
            for file in glob.glob(exeOutDir + "\\*.dll"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)

            #DLL Plugins
            for file in glob.glob(exeOutDir + "\\osgPlugins-3.4.0\\*.dll"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)



#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
