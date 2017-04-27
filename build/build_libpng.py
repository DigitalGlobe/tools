#------------------------------------------------------------------------------
#
# build_libpng.py
#
# Summary : Builds the LibPNG library.
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder      import *
from SystemManager   import *
from FileDistributor import *
from XmlUtils import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the libpng library."
        #----------------------------------------------------------------------
        # the name of the dynamic solution file
        _FILE_NAME_SOLUTION = "png.vcxproj"

        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "libPNG"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\libpng"
        #----------------------------------------------------------------------
        
        # the name of the path that contains the cmake files
        _PATH_NAME_CMAKE_SOURCE = "."
                
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

        _LIBNAME = 'libpng'
        _DEBUG_SUFFIX = '_d'

        _QT_DIR_X86 = '..\\..\\QT\\5.7\\x86\\lib\cmake\\qt5'
        _QT_DIR_X64 = '..\\..\\QT\\5.7\\x64\\lib\cmake\\qt5'

        _ZLIB_INCLUDE_DIR = '..\\..\\include\\zlib'
        
        _ZLIB_LIBRARY_DEBUG_x86 = '..\\..\\sdk\\x86\\lib\\zlib_d.lib'
        _ZLIB_LIBRARY_RELEASE_x86 = '..\\..\\sdk\\x86\\lib\\zlib.lib'
        
        _ZLIB_LIBRARY_DEBUG_x64 = '..\\..\\sdk\\x64\\lib\\zlib_d.lib'
        _ZLIB_LIBRARY_RELEASE_x64 = '..\\..\\sdk\\x64\\lib\\zlib.lib'
    
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = '.'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\libpng'
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # constructors
    
        #----------------------------------------------------------------------
        # Constructs this program.
        #
        # Parameters :
        #     self : this program
        def __init__(self) :
        
            pass
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # public methods
    
        #----------------------------------------------------------------------
        # The main method of the program.
        #
        # Parameters :
        #     self : this program
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
            systemManager.appendToPathEnvironmentVariable( Program._QT_DIR_X64 if buildSettings.X64Specified() else Program._QT_DIR_X86  + '\\bin')

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

            buildSourceName  = os.path.join( buildPathName , Program._PATH_NAME_CMAKE_SOURCE)

            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 zlibFileName                = os.path.join( buildPathName , \
                                                             Program._ZLIB_LIBRARY_RELEASE_x64                               )
            elif ( buildSettings.ReleaseSpecified() ) :                                                             
                 zlibFileName                = os.path.join( buildPathName , \
                                                             Program._ZLIB_LIBRARY_RELEASE_x86                               )                 
            elif ( buildSettings.X64Specified() ) :
                 zlibFileName                = os.path.join( buildPathName , \
                                                             Program._ZLIB_LIBRARY_DEBUG_x64                               )
            else :
                 zlibFileName                = os.path.join( buildPathName , \
                                                             Program._ZLIB_LIBRARY_DEBUG_x86                               )

            zlibIncludeFileName = os.path.join( buildPathName , Program._ZLIB_INCLUDE_DIR                               )

                                                             
            # run CMake
            if ( buildSettings.X64Specified() ) :
            
                cmakeCommandLine = ( ( "%s "                                + \
                                       "-DZLIB_INCLUDE_DIR=\"" + zlibIncludeFileName + "\" " + \
                                       "-DZLIB_LIBRARY=\"" + zlibFileName + "\" " + \
                                       "-G\"Visual Studio 14 2015 Win64\" " + \
                                       "\"%s\""                             ) % \
                                     ( PathFinder.FILE_NAME_CMAKE , \
                                       buildSourceName             ) )
                                       
            else :
            
                cmakeCommandLine = ( ( "%s "                                + \
                                       "-DZLIB_INCLUDE_DIR=\"" + zlibIncludeFileName + "\" " + \
                                       "-DZLIB_LIBRARY=\"" + zlibFileName + "\" " + \
                                       "-G\"Visual Studio 14 2015\" " + \
                                       "\"%s\""                             ) % \
                                     ( PathFinder.FILE_NAME_CMAKE , \
                                       buildSourceName             ) )
                                       
                            
            print('cmake: ' + cmakeCommandLine)
            systemManager.changeDirectory(buildPathName)
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
            
            dllName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".dll"
            libName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            pdbName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"

            buildOutDir   = os.path.join( buildPathName, 'build' )
            propfile   = os.path.join( buildPathName, 'linker.props' )
            
            msBuildCommandLine += ' /p:OutDir=' + buildOutDir
            msBuildCommandLine += ' /p:TargetExtension=dll'
            msBuildCommandLine += ' /p:SolutionDir=' + buildPathName + '\\' 
            msBuildCommandLine += ' /p:TargetName=' + Program._LIBNAME + ('' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX)
            msBuildCommandLine += ' /p:Configuration=' + conf
            msBuildCommandLine += ' /p:BuildProjectReferences=false'


            linkerprops = {'OutputFile':os.path.join( buildOutDir, dllName )}
            linkerprops['ImportLibrary'] = os.path.join( buildOutDir, libName )
            if buildSettings.ReleaseSpecified():
                linkerprops['DebugSymbols'] = 'false'
            else:
                linkerprops['DebugSymbols'] = 'true'
                linkerprops['DebugType'] = 'full'
                linkerprops['ProgramDatabaseFile'] = '$(OutDir)\\' + pdbName
              
            xmlUtils.buildDll(conf, platform, {}, linkerprops, propfile)

            msBuildCommandLine +=  ' /p:ForceImportBeforeCppTargets="' + propfile + '"'
            

            print('cmd: ' + msBuildCommandLine)
                       
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
                sys.exit(-1)

            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                              os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                              '*.h',                                                                 \
                              True, False) 
                
            systemManager.copyFile( os.path.join( buildOutDir, libName ) , \
                                    os.path.join( sdkOutDir , libName) )
            systemManager.copyFile( os.path.join( buildOutDir, dllName ) , \
                                    os.path.join( sdkOutDir , dllName) )
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )

            #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
