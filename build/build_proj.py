#------------------------------------------------------------------------------
#
# build_proj4.py
#
# Summary : Builds the PROJ.4 library.
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :
    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the Proj4 library."
        #----------------------------------------------------------------------
        
        _LIBNAME = 'proj'
        _DEBUG_SUFFIX = '_d'
        
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "PROJ.4"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\PROJ.4"
        #----------------------------------------------------------------------

        
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = 'src'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\proj4'
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
            
            # process command-line arguments
            buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)
            
            # initialize environment variables
            systemManager.initializeIncludeEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.initializeLibraryEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.appendToPathEnvironmentVariable( pathFinder.getVisualStudioBinPathName( buildSettings.X64Specified() ) )
            if ( buildSettings.X64Specified() ) :
            
                # append path for 64-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x64"                 ) )
                
            else:
            
                # append path for 32-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x86"                 ) )

            # determine path names
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            buildOutDir   = os.path.join( buildPathName, 'src' )
            sdkOutDir = buildPathName + "\\..\\" + (Program._PATH_NAME_DISTRIBUTION_X64 if buildSettings.X64Specified() else Program._PATH_NAME_DISTRIBUTION_X86)

            # initialize directories
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName ,
                                         buildPathName  )
                   
            vcvars = '"' + systemManager.getVisualStudioPath() + '"\\vcvarsall.bat ' + ("amd64"             \
                                                                                           if ( buildSettings.X64Specified() ) \
                                                                                        else "x86"                          )

                                                                                                                                                                                      
            dllName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".dll"
            libName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            staticLibName = Program._LIBNAME + '_static' + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            pdbName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"
            staticLibName = Program._LIBNAME + '_static' + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"

            # execute Nmake
            if ( buildSettings.X64Specified() ) :
            
                nmakeCommandLine = "nmake -f Makefile.vc MACHINE=\"/MACHINE:X64\" "
            else :
            
                nmakeCommandLine = "nmake -f Makefile.vc MACHINE=\"/MACHINE:X86\" "

            if ( buildSettings.ReleaseSpecified() ) :
                nmakeCommandLine += 'LINKOPT=/DEBUG:NONE'
            else :
                nmakeCommandLine += 'LINKOPT=/DEBUG:FULL OPTFLAGS=/Zi'
   
            nmakeCommandLine += ' EXE_PROJ="' + libName + '"' + ' PROJ_DLL="' + dllName + '"' + ' PROJ_IMP="' + libName + '"' + ' PROJ_LIB="' + staticLibName + '"'

            print('cmd: ' + nmakeCommandLine)
            systemManager.changeDirectory(buildPathName)
            nmakeResult = systemManager.execute(vcvars + " && " + nmakeCommandLine)
                
            if nmakeResult != 0:
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
            systemManager.copyFile( os.path.join( buildOutDir, staticLibName ) , \
                                    os.path.join( sdkOutDir , staticLibName) )
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------

