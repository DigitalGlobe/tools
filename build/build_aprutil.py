#------------------------------------------------------------------------------
#
# build_aprutil.py
#
# Summary : Builds the APR Util library.
#
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
#
# build_apr.py
#
# Summary : Builds the APR library.
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
        DESCRIPTION = "Builds the APR Util library."
        #----------------------------------------------------------------------
        # the name of the dynamic solution file
        _FILE_NAME_SOLUTION = "libexpat.vcxproj"
        _FILE_NAME_SOLUTION_2 = "libaprutil-1.vcxproj"
        _FILE_NAME_SOLUTION_3 = "aprutil-1.vcxproj"

        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "APRUtil"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\APRUtil"
        #----------------------------------------------------------------------
        
        # the name of the path that contains the cmake files
        _PATH_NAME_CMAKE_SOURCE = "."
                
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

        _LIBNAME = 'libaprutil'
        _LIBNAME_STATIC = 'aprutil'
        _DEBUG_SUFFIX = '_d'

        _QT_DIR_X86 = '..\\..\\QT\\5.7\\x86\\lib\cmake\\qt5'
        _QT_DIR_X64 = '..\\..\\QT\\5.7\\x64\\lib\cmake\\qt5'
    
        _APR_INCLUDE_DIR = '..\\..\\include\\apr'
        
        _APR_LIBRARY_DEBUG_x86 = '..\\..\\sdk\\x86\\lib\\libapr_d.lib'
        _APR_LIBRARY_RELEASE_x86 = '..\\..\\sdk\\x86\\lib\\libapr.lib'
        
        _APR_LIBRARY_DEBUG_x64 = '..\\..\\sdk\\x64\\lib\\libapr_d.lib'
        _APR_LIBRARY_RELEASE_x64 = '..\\..\\sdk\\x64\\lib\\libapr.lib'
        
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = '.'
        _PATH_NAME_INCLUDE_2 = 'include'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\aprutil'
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
                 aprFileName                = os.path.join( buildPathName , \
                                                             Program._APR_LIBRARY_RELEASE_x64                               )
            elif ( buildSettings.ReleaseSpecified() ) :                                                             
                 aprFileName                = os.path.join( buildPathName , \
                                                             Program._APR_LIBRARY_RELEASE_x86                               )                 
            elif ( buildSettings.X64Specified() ) :
                 aprFileName                = os.path.join( buildPathName , \
                                                             Program._APR_LIBRARY_DEBUG_x64                               )
            else :
                 aprFileName                = os.path.join( buildPathName , \
                                                             Program._APR_LIBRARY_DEBUG_x86                               )

            aprIncludeFileName = os.path.join( buildPathName , Program._APR_INCLUDE_DIR                               )

                                                             
            # run CMake
            if ( buildSettings.X64Specified() ) :
            
                cmakeCommandLine = ( ( "%s "                                + \
                                       "-G\"Visual Studio 14 2015 Win64\" " + \
                                       "-DAPR_INCLUDE_DIR=\"" + aprIncludeFileName + "\" " + \
                                       "-DAPR_LIBRARIES=\"" + aprFileName + "\" " + \
                                       "\"%s\""                             ) % \
                                     ( PathFinder.FILE_NAME_CMAKE , \
                                       buildSourceName             ) )
                                       
            else :
            
                cmakeCommandLine = ( ( "%s "                                + \
                                       "-G\"Visual Studio 14 2015\" " + \
                                       "-DAPR_INCLUDE_DIR=\"" + aprIncludeFileName + "\" " + \
                                       "-DAPR_LIBRARIES=\"" + aprFileName + "\" " + \
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
            
            # build local libexpat
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( "\"%s\" "                  + \
                                     "/p:platform=%s " ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified()), platform )
            

            buildOutDir   = os.path.join( buildPathName, 'build-local' )
            propfile   = os.path.join( buildPathName, 'linker.props' )
            
            # msBuildCommandLine += ' /p:OutDir=' + buildOutDir
            msBuildCommandLine += ' /p:SolutionDir=' + buildPathName + '\\' 
            msBuildCommandLine += ' /p:Configuration=' + conf
            msBuildCommandLine += ' /p:BuildProjectReferences=false'
            linkerprops = {}
            if buildSettings.ReleaseSpecified():
                linkerprops['DebugSymbols'] = 'false'
            else:
                linkerprops['DebugSymbols'] = 'true'
                linkerprops['DebugType'] = 'full'
              
            msBuildCommandLine +=  ' /p:ForceImportBeforeCppTargets="' + propfile + '"'
            msBuildCommandLine += ' "' + solutionFileName + '"'
            
            print('cmd: ' + msBuildCommandLine)
                        
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
                sys.exit(-1)
            

            # Build the dynamic lib    
            solutionFileName   = os.path.join( buildPathName               , \
                                   Program._FILE_NAME_SOLUTION_2 )
            msBuildCommandLine = ( "\"%s\" "                  + \
                                     "/p:platform=%s " ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified()), platform )
            
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
            linkerprops = {}
            linkerprops['OutputFile'] = os.path.join( buildOutDir, dllName )
            linkerprops['ImportLibrary'] = os.path.join( buildOutDir, libName )
            if buildSettings.ReleaseSpecified():
                linkerprops['DebugSymbols'] = 'false'
            else:
                linkerprops['DebugSymbols'] = 'true'
                linkerprops['DebugType'] = 'full'
                linkerprops['ProgramDatabaseFile'] = '$(OutDir)\\' + pdbName
              
            xmlUtils.buildDll(conf, platform, {}, linkerprops, propfile)

            msBuildCommandLine +=  ' /p:ForceImportBeforeCppTargets="' + propfile + '"'
            msBuildCommandLine += ' "' + solutionFileName + '"'
            print('cmd: ' + msBuildCommandLine)
                        
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
                sys.exit(-1)    
                
            systemManager.copyFile( os.path.join( buildOutDir, libName ) , \
                                    os.path.join( sdkOutDir , libName) )
            systemManager.copyFile( os.path.join( buildOutDir, dllName ) , \
                                    os.path.join( sdkOutDir , dllName) )
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )

            # Build the static lib    
            solutionFileName   = os.path.join( buildPathName               , \
                                   Program._FILE_NAME_SOLUTION_3 )
            msBuildCommandLine = ( "\"%s\" "                  + \
                                     "/p:platform=%s " ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified()), platform )
            
            libName = Program._LIBNAME_STATIC + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            pdbName = Program._LIBNAME_STATIC + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"

            buildOutDir   = os.path.join( buildPathName, 'build' )
            propfile   = os.path.join( buildPathName, 'linker.props' )
            
            msBuildCommandLine += ' /p:OutDir=' + buildOutDir
            msBuildCommandLine += ' /p:TargetExtension=lib'
            msBuildCommandLine += ' /p:SolutionDir=' + buildPathName + '\\' 
            msBuildCommandLine += ' /p:TargetName=' + Program._LIBNAME_STATIC + ('' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX)
            msBuildCommandLine += ' /p:Configuration=' + conf
            msBuildCommandLine += ' /p:BuildProjectReferences=false'
            linkerprops = {}
            linkerprops['OutputFile'] = os.path.join( buildOutDir, libName )
            if buildSettings.ReleaseSpecified():
                linkerprops['DebugSymbols'] = 'false'
            else:
                linkerprops['DebugSymbols'] = 'true'
                linkerprops['DebugType'] = 'full'
                linkerprops['ProgramDatabaseFile'] = '$(OutDir)\\' + pdbName
              
            if buildSettings.ReleaseSpecified():
                compprops = {'DebugInformationFormat':'None'}
            else:
                compprops = {'DebugInformationFormat':'ProgramDatabase', 'ProgramDataBaseFileName':os.path.join( buildOutDir, pdbName )}

            xmlUtils.buildLib(conf, platform, compprops, linkerprops, propfile)

            msBuildCommandLine +=  ' /p:ForceImportBeforeCppTargets="' + propfile + '"'
            msBuildCommandLine += ' "' + solutionFileName + '"'
            print('cmd: ' + msBuildCommandLine)
                        
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
                sys.exit(-1)    
                
            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                              os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                              '*.h',                                                                 \
                              True, False) 
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE_2),              \
                              os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                              '*.h',                                                                 \
                              True, False) 
                
            systemManager.copyFile( os.path.join( buildOutDir, libName ) , \
                                    os.path.join( sdkOutDir , libName) )
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )
                                        
            #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
