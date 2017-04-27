#------------------------------------------------------------------------------
#
# build_qxrunner.py
#
# Summary : Builds the qxrunner library.
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
        DESCRIPTION = "Builds the qxrunner library."
        #----------------------------------------------------------------------
        # the name of the dynamic solution file
        _FILE_NAME_SOLUTION = "src\\qxrunner\\qxrunner.vcxproj"
        _FILE_NAME_SOLUTION_2 = "src\\qxcppunit\\qxcppunit.vcxproj"
        
        # the name of the path that contains QT
        _PATH_NAME_QT_SRC = "..\\src\\qt"
        #----------------------------------------------------------------------
        # the name of the path that contains QT
        _PATH_NAME_QT_EXEC_X86 = "..\\..\\..\\..\\qt\\5.7\\x86"
        #----------------------------------------------------------------------
        # the name of the path that contains QT
        _PATH_NAME_QT_EXEC_X64 = "..\\..\\..\\..\\qt\\5.7\\x64"
        #----------------------------------------------------------------------

        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "qxrunner"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\qxrunner"
        #----------------------------------------------------------------------
                
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        
        _LIBNAME = 'qxrunner'
        _LIBNAME_2 = 'qxcppunit'
        _DEBUG_SUFFIX = '_d'

        _CPPUNIT_INCLUDE = '..\\..\\include'
        _CPPUNIT_LIB = 'libcppunit'
        
        _QT_BASE = '..\\..\\Qt\\5.7\\'
        
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = 'include\\qxrunner'
        _PATH_NAME_INCLUDE_2 = 'include\\qxcppunit'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\qxrunner'
        _PATH_NAME_DISTRIBUTION_INCLUDE_2 = '..\\..\\include\\qxcppunit'
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
            systemManager.appendToPathEnvironmentVariable(Program._PATH_NAME_QT_EXEC_X64         \
                                                           if ( buildSettings.X64Specified() ) \
                                                           else Program._PATH_NAME_QT_EXEC_X86 )
                                                           
                                                           
            os.environ["QTDIR"] = ( Program._PATH_NAME_QT_EXEC_X64         \
                                   if ( buildSettings.X64Specified() ) \
                                   else Program._PATH_NAME_QT_EXEC_X86 )

                                  
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
            includePathName = os.path.join( buildPathName, Program._PATH_NAME_INCLUDE)
            
            sdkOutDir = buildPathName + "\\..\\" + (Program._PATH_NAME_DISTRIBUTION_X64 if buildSettings.X64Specified() else Program._PATH_NAME_DISTRIBUTION_X86)

            qtBase = os.path.join( buildPathName, Program._QT_BASE)
            qtVer = os.path.join( qtBase, 'x64' if buildSettings.X64Specified() else 'x86')
            qtInclude = os.path.join( qtVer, 'include')
            qtWidgetsInclude = os.path.join( qtInclude, 'QtWidgets')
            qtLib = os.path.join( qtVer, 'lib')

            
            # remove build dir
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)

            #copy UriParser to the Build area
            systemManager.copyDirectory( sourcePathName, buildPathName)
        
            # start building
            systemManager.changeDirectory(buildPathName)
                
            conf = "Release_DLL" if ( buildSettings.ReleaseSpecified() ) else "Debug_DLL"
            platform  = 'x64' if buildSettings.X64Specified() else 'Win32'
            # build the solution
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( "\"%s\" "                  + \
                                     "/p:platform=%s " + \
                                     "\"%s\""                   ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified()), platform, solutionFileName )
            
            msBuildCommandLine += ' /p:TargetName=' + Program._LIBNAME + ('' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX)
            msBuildCommandLine += ' /p:Configuration=' + conf

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
                        
            linkerprops['AdditionalLibraryDirectories'] = '"' + sdkOutDir + '";"' + qtLib + '"; %(AdditionalLibraryDirectories)'
  
            compprops = {'AdditionalIncludeDirectories':'.;"' + os.path.join(buildPathName, Program._CPPUNIT_INCLUDE) + \
            '";"' + qtInclude + '";"' + qtWidgetsInclude + '";%(AdditionalIncludeDirectories)'}
                
            xmlUtils.buildDll(conf, platform, compprops, linkerprops, propfile)

            msBuildCommandLine +=  ' /p:ForceImportBeforeCppTargets="' + propfile + '"'

            print('cmd: ' + msBuildCommandLine)
                   
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
                sys.exit(-1)


            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                              os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                              '*.h',                                                                 \
                              True, False, True) 
                
            systemManager.copyFile( os.path.join( buildOutDir, libName ) , \
                                    os.path.join( sdkOutDir , libName) )
            systemManager.copyFile( os.path.join( buildOutDir, dllName ) , \
                                    os.path.join( sdkOutDir , dllName) )
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )


            # build the solution
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION_2 )
            msBuildCommandLine = ( "\"%s\" "                  + \
                                     "/p:platform=%s " + \
                                     "\"%s\""                   ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified()), platform, solutionFileName )
            
            msBuildCommandLine += ' /p:TargetName=' + Program._LIBNAME_2 + ('' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX)
            msBuildCommandLine += ' /p:Configuration=' + conf

            dllName = Program._LIBNAME_2 + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".dll"
            libName = Program._LIBNAME_2 + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            pdbName = Program._LIBNAME_2 + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"

            buildOutDir   = os.path.join( buildPathName, 'build' )
            propfile   = os.path.join( buildPathName, 'linker.props' )
            
            msBuildCommandLine += ' /p:OutDir=' + buildOutDir
            msBuildCommandLine += ' /p:TargetExtension=dll'
            msBuildCommandLine += ' /p:SolutionDir=' + buildPathName + '\\' 
            msBuildCommandLine += ' /p:TargetName=' + Program._LIBNAME_2 + ('' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX)
            msBuildCommandLine += ' /p:Configuration=' + conf
            msBuildCommandLine += ' /p:BuildProjectReferences=false'

            linkerprops = {'OutputFile':os.path.join( buildOutDir, dllName )}
            linkerprops['ImportLibrary'] = os.path.join( buildOutDir, libName )
            if buildSettings.ReleaseSpecified():
                linkerprops['DebugSymbols'] = 'false'
            else:
                linkerprops['DebugSymbols'] = 'true'
                linkerprops['DebugType'] = 'full'

            linkerprops['AdditionalLibraryDirectories'] = '"' + sdkOutDir + '";"' + qtLib + '"; %(AdditionalLibraryDirectories)'
  
            compprops = {'AdditionalIncludeDirectories':'.;"' + os.path.join(buildPathName, Program._CPPUNIT_INCLUDE) + \
            '";"' + qtInclude + '";"' + qtWidgetsInclude + '";%(AdditionalIncludeDirectories)'}
          
            xmlUtils.buildDll(conf, platform, compprops, linkerprops, propfile)

            msBuildCommandLine +=  ' /p:ForceImportBeforeCppTargets="' + propfile + '"'

            print('cmd: ' + msBuildCommandLine)
                       
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
                sys.exit(-1)
                
            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE_2))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE_2),              \
                              os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE_2), \
                              '*.h',                                                                 \
                              True, False, True) 

                
            systemManager.copyFile( os.path.join( buildOutDir, libName ) , \
                                    os.path.join( sdkOutDir , libName) )
            systemManager.copyFile( os.path.join( buildOutDir, dllName ) , \
                                    os.path.join( sdkOutDir , dllName) )
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
