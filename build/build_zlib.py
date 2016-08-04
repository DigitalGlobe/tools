#------------------------------------------------------------------------------
#
# build_zlib.py
#
# Summary : Builds the ZLib library.
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
        
        #----------------------------------------------------------------------
        # the name of the build debug debug file
        _FILE_NAME_DEBUG_BUILD_DEBUG = "zlib.pdb"
        #----------------------------------------------------------------------
        # the name of the distribution debug debug file
        _FILE_NAME_DEBUG_DISTRIBUTION_DEBUG = "zlib_d.pdb"
        #----------------------------------------------------------------------
        # the name of the build release debug file
        _FILE_NAME_DEBUG_BUILD_RELEASE = "zlib.pdb"
        #----------------------------------------------------------------------
        # the name of the distribution release debug file
        _FILE_NAME_DEBUG_DISTRIBUTION_RELEASE = "zlib.pdb"
        #----------------------------------------------------------------------
        # the name of the build debug dynamic file
        _FILE_NAME_DYNAMIC_BUILD_DEBUG = "zlib1.dll"
        #----------------------------------------------------------------------
        # the name of the distribution debug dynamic file
        _FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG = "zlib_d.dll"
        #----------------------------------------------------------------------
        # the name of the build release dynamic file
        _FILE_NAME_DYNAMIC_BUILD_RELEASE = "zlib1.dll"
        #----------------------------------------------------------------------
        # the name of the distribution release dynamic file
        _FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE = "zlib.dll"
        #----------------------------------------------------------------------
        # the name of the build debug library file
        _FILE_NAME_LIBRARY_BUILD_DEBUG = "zlib.lib"
        #----------------------------------------------------------------------
        # the name of the distribution debug library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG = "zlib_d.lib"
        #----------------------------------------------------------------------
        # the name of the build release library file
        _FILE_NAME_LIBRARY_BUILD_RELEASE = "zlib.lib"
        #----------------------------------------------------------------------
        # the name of the distribution release library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE = "zlib.lib"
        #----------------------------------------------------------------------
        # the pattern for binary files
        _FILE_PATTERN_BINARY = "*.exe"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit binary files
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit binary files
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "ZLib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\ZLib"
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
            
            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DEBUG_BUILD_RELEASE                                        ) 
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DYNAMIC_BUILD_RELEASE                                      ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION_RELEASE                                 )
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE                               )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DEBUG_BUILD_RELEASE                                        ) 
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DYNAMIC_BUILD_RELEASE                                      ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION_RELEASE                                 )
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE                               )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DEBUG_BUILD_DEBUG                                          )
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DYNAMIC_BUILD_DEBUG                                        )
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        )
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION_DEBUG                                   )
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG                                 )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 
            else :
            
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DEBUG_BUILD_DEBUG                                          )
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DYNAMIC_BUILD_DEBUG                                        ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        ) 
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION_DEBUG                                   )
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG                                 )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
            
            # initialize directories
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName ,
                                         buildPathName  )
                                         
            # execute Nmake
            if ( buildSettings.X64Specified() ) :
            
                nmakeCommandLine = "nmake -f win32/Makefile.msc AS=ml64 LOC=\"-DASMV -DASMINF -I.\" OBJA=\"inffasx64.obj gvmat64.obj inffas8664.obj\""
            
            else :
            
                nmakeCommandLine = "nmake.exe -f win32/Makefile.msc LOC=\"-DASMV -DASMINF\" OBJA=\"inffas32.obj match686.obj\"";

            systemManager.changeDirectory(buildPathName)
            nmakeResult = systemManager.execute(nmakeCommandLine)
                
            # distribute the debug files
            systemManager.copyFile( buildDebugFileName        , \
                                    distributionDebugFileName )

            # distribute the dynamic files
            systemManager.copyFile( buildDynamicFileName        , \
                                    distributionDynamicFileName )
                
            # distribute the library files
            systemManager.copyFile( buildLibraryFileName        , \
                                    distributionLibraryFileName )
                
            # distribute the binary files
            buildBinaryFileNames = glob.glob( os.path.join( buildPathName                , \
                                                            Program._FILE_PATTERN_BINARY ) );
            for buildBinaryFileName in buildBinaryFileNames :
            
                binaryFileName = os.path.join( binaryPathName , \
                                               os.path.basename(buildBinaryFileName) )
                systemManager.copyFile( buildBinaryFileName , \
                                        binaryFileName      )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
