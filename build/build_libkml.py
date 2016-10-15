#------------------------------------------------------------------------------
#
# build_libkml.py
#
# Summary : Builds the libkml libraries.
#
#------------------------------------------------------------------------------

import glob
import os
import shutil


from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
        DESCRIPTION = "Builds LibKml libs."
        _PATH_NAME_BUILD = "LibKml"
        _PATH_NAME_DISTRIBUTION_X86 = "\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "\\x64\\lib"
        _PATH_NAME_SOURCE = "..\\src\\LibKml"
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

            #copy libkml to the Build area
            systemManager.copyDirectory( sourcePathName, buildPathName)
        
            # start building
            systemManager.changeDirectory(buildPathName)
            cmd = "msbuild msvc\\libkml.sln "
            
            pathName = sourcePathName + "\\msvc"
                      
            compileOutDir = buildPathName + "\\msvc"
            # build command line based on options
            if ( buildSettings.X64Specified() ) :
                cmd = cmd + "/p:platform=x64 "
                compileOutDir = compileOutDir + "\\x64"
                pathName = pathName + "\\x64"
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X64
            else :
                cmd = cmd + "/p:platform=Win32 "
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X86

 
            if buildSettings.ReleaseSpecified():
                cmd = cmd + "/p:configuration=Release "
                compileOutDir = compileOutDir + "\\Release"
                pathName = pathName + "\\Release"
            else :
                cmd = cmd + "/p:configuration=Debug "
                compileOutDir = compileOutDir + "\\Debug"
                pathName = pathName + "\\Debug"
            
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
                print(pathName)
                print (file)
                shutil.copy(file, pathName)
#                shutil.move(file, pathName)
            for file in glob.glob(compileOutDir + "\\*.pdb"):
                print( "copying " + file + " -> " + sdkOutLibsDir)
                shutil.copy(file, sdkOutLibsDir)                
                
            #now we need to compile the 3rd party minizip stuff 
            #needed for the libkml driver
                
            cmd = "msbuild third_party\\zlib-1.2.3\\contrib\\minizip\\minizip.sln "
            
            compileOutDir = buildPathName + "\\third_party\\zlib-1.2.3\\contrib\\minizip"
            
            pathName = sourcePathName + "\\third_party\\zlib-1.2.3\\contrib\\minizip"
            
            # build command line based on options
            if ( buildSettings.X64Specified() ) :
                compileOutDir = compileOutDir + "\\x64"
                pathName = pathName + "\\x64"
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X64
            else :
                compileOutDir = compileOutDir + "\\Win32"
                pathName = pathName + "\\Win32"
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X86

 
            if buildSettings.ReleaseSpecified():
                cmd = cmd + "/p:configuration=Release "
                compileOutDir = compileOutDir + "\\Release"
                pathName = pathName + "\\Release"
            else:
                cmd = cmd + "/p:configuration=Debug "
                compileOutDir = compileOutDir + "\\Debug"
                pathName = pathName + "\\Debug"
            
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
                print(pathName)
                print (file)
                shutil.copy(file, pathName)
            for file in glob.glob(compileOutDir + "\\*.pdb"):
                print( "copying " + file + " -> " + sdkOutLibsDir)
                shutil.copy(file, sdkOutLibsDir)
                
                
            #now we need to compile the 3rd party uriparser stuff 
            #needed for the libkml driver
                
            cmd = "msbuild third_party\\uriparser-0.7.5\\win32\Visual_Studio_2005\\uriparser.sln "
            
            compileOutDir = buildPathName + "\\third_party\\uriparser-0.7.5\\win32\\Visual_Studio_2005"
            
            pathName = sourcePathName + "\\third_party\\uriparser-0.7.5\\win32\\Visual_Studio_2005"
                        
            # build command line based on options
            if ( buildSettings.X64Specified() ) :
                compileOutDir = compileOutDir + "\\x64"
                pathName = pathName + "\\x64"
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X64
            else :
                distribLibs = Program._PATH_NAME_DISTRIBUTION_X86

 
            if buildSettings.ReleaseSpecified():
                cmd = cmd + "/p:configuration=Release "
                compileOutDir = compileOutDir + "\\Release"
                pathName = pathName + "\\Release"
            else:
                cmd = cmd + "/p:configuration=Debug "
                compileOutDir = compileOutDir + "\\Debug"
                pathName = pathName + "\\Debug"
           
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
                print(pathName)
                print (file)
                shutil.copy(file, pathName)
            for file in glob.glob(compileOutDir + "\\*.pdb"):
                print( "copying " + file + " -> " + sdkOutLibsDir)
                shutil.copy(file, sdkOutLibsDir)
                


#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
