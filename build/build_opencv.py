#------------------------------------------------------------------------------
#
# build_opencv.py
#
# Summary : Builds the OpenCV library.
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *
from XmlUtils import *

class Program :
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the OpenCV library."
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "OpenCV.sln"
        
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "OpenCV"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\OpenCV"
        #----------------------------------------------------------------------
                
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        
        _LIBNAME = 'opencv'
        _DEBUG_SUFFIX = '_d'

        # the name of the path for all include files
        _PATH_NAME_LIBS = 'lib'
        
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = 'modules'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\opencv'

        def __init__(self) :
        
            pass
        #----------------------------------------------------------------------
        

        def main(self) :
            systemManager = SystemManager()
            pathFinder    = PathFinder()
            xmlUtils = XmlUtils()
            
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
            sdkOutDir = buildPathName + "\\..\\" + (Program._PATH_NAME_DISTRIBUTION_X64 if buildSettings.X64Specified() else Program._PATH_NAME_DISTRIBUTION_X86)

            # remove build dir
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)

            #copy UriParser to the Build area
            systemManager.copyDirectory( sourcePathName, buildPathName)
        
            # start building
            systemManager.changeDirectory(buildPathName)
                   
            # run CMake
            if ( buildSettings.X64Specified() ) :
                cmakeCommandLine = '"%s" -DX86_64=1 -G "Visual Studio 14 2015 Win64" "%s"' % (PathFinder.FILE_NAME_CMAKE, sourcePathName)
            else:
                cmakeCommandLine = '"%s" "%s"' % (PathFinder.FILE_NAME_CMAKE, sourcePathName)
                
            cmakeCommandLine += ' -DBUILD_DOCS=0 -DBUILD_WITH_STATIC_CRT=0 -DBUILD_opencv_apps=0'
            
            print("cmake: " + cmakeCommandLine)
            
            cmakeResult = systemManager.execute(cmakeCommandLine)
            if (cmakeResult != 0) :
                sys.exit(-1)
                   
            conf = "Release" if ( buildSettings.ReleaseSpecified() ) else "Debug"
            platform  = 'x64' if buildSettings.X64Specified() else 'Win32'
            # build the solution
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( "\"%s\" "                  + \
                                     "/p:platform=%s " + \
                                     "\"%s\""                   ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified()), platform, solutionFileName )
            
            buildOutDir   = os.path.join( buildPathName, 'lib\\' + conf )
            buildBinDir   = os.path.join( buildPathName, 'bin\\' + conf )
            propfile   = os.path.join( buildPathName, 'linker.props' )
            
            # msBuildCommandLine += ' /p:OutDir=' + buildOutDir
            msBuildCommandLine += ' /p:SolutionDir=' + buildPathName + '\\' 
            msBuildCommandLine += ' /p:Configuration=' + conf

            print('cmd: ' + msBuildCommandLine)
                       
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
                sys.exit(-1)

            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                                          os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                                          '*.h',                                                                 \
                                          True, False, True) 
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                                          os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                                          '*.hpp',                                                                 \
                                          True, False, True) 
                                          
            systemManager.distributeFiles(buildOutDir,              \
                                          sdkOutDir, \
                                          'opencv*.lib',                                                                 \
                                          True, False, True) 
            systemManager.distributeFiles(buildBinDir,              \
                                          sdkOutDir, \
                                          'opencv*.dll',                                                                 \
                                          True, False, True) 
            if not buildSettings.ReleaseSpecified():
                systemManager.distributeFiles(buildOutDir,              \
                                              sdkOutDir, \
                                              'opencv*.pdb',                                                                 \
                                              True, False, True) 
                


#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------