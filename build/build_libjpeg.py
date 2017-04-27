#------------------------------------------------------------------------------
#
# build_libjpeg.py
#
# Summary : Builds the LibJPEG library.
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
        DESCRIPTION = "Builds the ZLib library."
        #----------------------------------------------------------------------
        
        _LIBNAME = 'libjpeg'
        _DEBUG_SUFFIX = '_d'
        
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "libJPEG"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\libJPEG"
        #----------------------------------------------------------------------

        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit binary files
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit binary files
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = '.'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\libjpeg'
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
            binaryPathName = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64) \
                               if ( buildSettings.X64Specified() )                                     \
                               else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            sdkOutDir = buildPathName + "\\..\\" + (Program._PATH_NAME_DISTRIBUTION_X64 if buildSettings.X64Specified() else Program._PATH_NAME_DISTRIBUTION_X86)

            # initialize directories
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName ,
                                         buildPathName  )
                   
            vcvars = '"' + systemManager.getVisualStudioPath() + '"\\vcvarsall.bat ' + ("amd64"             \
                                                                                           if ( buildSettings.X64Specified() ) \
                                                                                        else "x86"                          )
                                                                                                                                                                                      
            libName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"

            # run Nmake
            nmakeCommandLine = 'nmake -f Makefile.vc'

            if ( buildSettings.ReleaseSpecified() ) :
                nmakeCommandLine += ' nodebug=1'
            else :
                pass
                
            nmakeCommandLine += ' LIBNAME=' + libName

            
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
                
            systemManager.copyFile( os.path.join( buildPathName, libName ) , \
                                    os.path.join( sdkOutDir , libName) )
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
