#------------------------------------------------------------------------------
#
# build_libzlib.py
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
        # the name of the build debug import file
        _FILE_NAME_IMPORT_BUILD_DEBUG = "zlibd.lib"
        #----------------------------------------------------------------------
        # the name of the build debug library file
        _FILE_NAME_LIBRARY_BUILD_DEBUG = "zlibd.dll"
        #----------------------------------------------------------------------
        # the name of the distribution debug import file
        _FILE_NAME_IMPORT_DISTRIBUTION_DEBUG = "zlib_d.lib"
        #----------------------------------------------------------------------
        # the name of the distribution debug library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG = "zlib_d.dll"
        #----------------------------------------------------------------------
        # the name of the build release import file
        _FILE_NAME_IMPORT_BUILD_RELEASE = "zlib.lib"
        #----------------------------------------------------------------------
        # the name of the build release library file
        _FILE_NAME_LIBRARY_BUILD_RELEASE = "zlib.dll"
        #----------------------------------------------------------------------
        # the name of the distribution release import file
        _FILE_NAME_IMPORT_DISTRIBUTION_RELEASE = "zlib.lib"
        #----------------------------------------------------------------------
        # the name of the distribution release library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE = "zlib.dll"
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "zlib.sln"
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
        # the name of the path that will contain debug build files
        _PATH_NAME_BUILD_DEBUG = "Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain release build files
        _PATH_NAME_BUILD_RELEASE = "Release"
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
            
            # determine path names
            binaryPathName = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64) \
                               if ( buildSettings.X64Specified() )                                     \
                               else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildImportFileName         = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_IMPORT_BUILD_RELEASE                                       ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionImportFileName  = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_IMPORT_DISTRIBUTION_RELEASE                                )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildImportFileName         = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_IMPORT_BUILD_RELEASE                                       ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionImportFileName  = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_IMPORT_DISTRIBUTION_RELEASE                                )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildImportFileName         = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_IMPORT_BUILD_DEBUG                                         ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        ) 
                 distributionImportFileName  = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_IMPORT_DISTRIBUTION_DEBUG                                  )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 
            else :
            
                 buildImportFileName         = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_IMPORT_BUILD_DEBUG                                         ) 
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        ) 
                 distributionImportFileName  = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_IMPORT_DISTRIBUTION_DEBUG                                  )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
            
            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.makeDirectory(buildPathName)
                               
            # run CMake
            cmakeCommandLine = ( "\"%s\" -DCMAKE_BUILD_TYPE=%s \"%s\"" % \
                                 ( PathFinder.FILE_NAME_CMAKE                , \
                                   ( "Release"                               \
                                     if ( buildSettings.ReleaseSpecified() ) \
                                     else "Debug"                          ) , \
                                   sourcePathName             ) )
            systemManager.changeDirectory(buildPathName)
            print(cmakeCommandLine)
            cmakeResult = systemManager.execute(cmakeCommandLine)
            if (cmakeResult != 0) :
            
                sys.exit(-1)

            # build the solution
            msBuildCommandLine = ( "\"%s\" /p:Configuration=%s \"%s\"" % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                   ( "Release"                               \
                                     if ( buildSettings.ReleaseSpecified() ) \
                                     else "Debug"                          )                       , \
                                   Program._FILE_NAME_SOLUTION                                     ) )
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute the library files
            systemManager.copyFile( buildImportFileName         , \
                                    distributionImportFileName  )
            systemManager.copyFile( buildLibraryFileName        , \
                                    distributionLibraryFileName )

            # copy the binary files
            buildBinaryFileNames = glob.glob( os.path.join( buildPathName                               , \
                                                            ( Program._PATH_NAME_BUILD_RELEASE        \
                                                              if ( buildSettings.ReleaseSpecified() ) \
                                                              else Program._PATH_NAME_BUILD_DEBUG     ) , \
                                                            Program._FILE_PATTERN_BINARY                ) );
            for buildBinaryFileName in buildBinaryFileNames :
            
                binaryFileName = os.path.join( binaryPathName , \
                                               ( os.path.basename(buildBinaryFileName) \
                                                 if ( buildSettings.ReleaseSpecified() ) \
                                                 else systemManager.getDebugFileName( os.path.basename(buildBinaryFileName) ) ) )
                systemManager.copyFile( buildBinaryFileName , \
                                        binaryFileName      )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
