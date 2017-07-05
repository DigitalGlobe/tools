#------------------------------------------------------------------------------
#
# build_muparser.py
#
# Summary : Builds the muparser expression parser
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
        DESCRIPTION = "Builds muparser."
        
        #----------------------------------------------------------------------
        # the name of the dynamic solution file
        _FILE_NAME_SOLUTION = "build\\msvc2013\\muparser_muParser.vcxproj"
        
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "muparser"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\muparser"
        #----------------------------------------------------------------------
                
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        
        _LIBNAME = 'muparser'
        _DEBUG_SUFFIX = '_d'

        # the name of the path for all include files
        _PATH_NAME_INCLUDE = 'include'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\muparser'
        #----------------------------------------------------------------------

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
                                    
            
            conf = 'Release Static' if ( buildSettings.ReleaseSpecified() ) else 'Debug Static'
            platform  = 'x64' if buildSettings.X64Specified() else 'Win32'
            # build the solution
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( "\"%s\" "                  + \
                                     "/p:platform=%s " + \
                                     "\"%s\""                   ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified()), platform, solutionFileName )
            
            libName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            pdbName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"

            buildOutDir   = os.path.join( buildPathName, 'build' )
            propfile   = os.path.join( buildPathName, 'linker.props' )
            
            msBuildCommandLine += ' /p:OutDir=' + buildOutDir
            msBuildCommandLine += ' /p:TargetExtension=lib'
            msBuildCommandLine += ' /p:SolutionDir=' + buildPathName + '\\' 
            msBuildCommandLine += ' /p:TargetName=' + libName
            msBuildCommandLine += ' /p:Configuration="' + conf + '"'
            msBuildCommandLine += ' /p:BuildProjectReferences=false'
            # msBuildCommandLine += ' /p:CharacterSet="unicode"'

            linkerprops = {}
            linkerprops['OutputFile'] = os.path.join( buildOutDir, libName )
            if buildSettings.ReleaseSpecified():
                linkerprops['DebugSymbols'] = 'false'
            else:
                linkerprops['DebugSymbols'] = 'true'
                linkerprops['DebugType'] = 'full'
              
            if buildSettings.ReleaseSpecified():
                compprops = {'DebugInformationFormat':'None'}
            else:
                compprops = {'DebugInformationFormat':'ProgramDatabase', 'ProgramDataBaseFileName':os.path.join( buildOutDir, pdbName )}

            xmlUtils.buildLib(conf, platform, compprops, linkerprops, propfile)

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
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
