#------------------------------------------------------------------------------
#
# build_openthreads.py
#
# Summary : Builds the OpenThreads library.
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from FileDistributor import *
from PathFinder      import *
from SystemManager   import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the OpenThreads library."
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "win32_src\\OpenThreads.sln"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "OpenThreads"
        #----------------------------------------------------------------------
        # the name of the x86 debug dynamic build file
        _FILE_NAME_BUILD_DYNAMIC_X86_DEBUG = "bin\\win32\\OpenThreadsWin32d.dll"
        #----------------------------------------------------------------------
        # the name of the x86 debug library build file
        _FILE_NAME_BUILD_LIBRARY_X86_DEBUG = "lib\\win32\\OpenThreadsWin32d.lib"
        #----------------------------------------------------------------------
        # the name of the x86 release dynamic build file
        _FILE_NAME_BUILD_DYNAMIC_X86_RELEASE = "bin\\win32\\OpenThreadsWin32.dll"
        #----------------------------------------------------------------------
        # the name of the x86 release library build file
        _FILE_NAME_BUILD_LIBRARY_X86_RELEASE = "lib\\win32\\OpenThreadsWin32.lib"
        #----------------------------------------------------------------------
        # the name of the x64 debug dynamic build file
        _FILE_NAME_BUILD_DYNAMIC_X64_DEBUG = "bin\\win32\\OpenThreadsWin32d.dll"
        #----------------------------------------------------------------------
        # the name of the x64 debug library build file
        _FILE_NAME_BUILD_LIBRARY_X64_DEBUG = "lib\\win32\\OpenThreadsWin32d.lib"
        #----------------------------------------------------------------------
        # the name of the x64 release dynamic build file
        _FILE_NAME_BUILD_DYNAMIC_X64_RELEASE = "bin\\win32\\OpenThreadsWin32.dll"
        #----------------------------------------------------------------------
        # the name of the x64 release library build file
        _FILE_NAME_BUILD_LIBRARY_X64_RELEASE = "lib\\win32\\OpenThreadsWin32.lib"
        #----------------------------------------------------------------------
        # the name of the x86 debug dynamic distribution file
        _FILE_NAME_DISTRIBUTION_DYNAMIC_X86_DEBUG = "OpenThreads_d.dll"
        #----------------------------------------------------------------------
        # the name of the x86 debug library distribution file
        _FILE_NAME_DISTRIBUTION_LIBRARY_X86_DEBUG = "OpenThreads_d.lib"
        #----------------------------------------------------------------------
        # the name of the x86 release dynamic distribution file
        _FILE_NAME_DISTRIBUTION_DYNAMIC_X86_RELEASE = "OpenThreads.dll"
        #----------------------------------------------------------------------
        # the name of the x86 release library distribution file
        _FILE_NAME_DISTRIBUTION_LIBRARY_X86_RELEASE = "OpenThreads.lib"
        #----------------------------------------------------------------------
        # the name of the x64 debug dynamic distribution file
        _FILE_NAME_DISTRIBUTION_DYNAMIC_X64_DEBUG = "OpenThreads_d.dll"
        #----------------------------------------------------------------------
        # the name of the x64 debug library distribution file
        _FILE_NAME_DISTRIBUTION_LIBRARY_X64_DEBUG = "OpenThreads_d.lib"
        #----------------------------------------------------------------------
        # the name of the x64 release dynamic distribution file
        _FILE_NAME_DISTRIBUTION_DYNAMIC_X64_RELEASE = "OpenThreads.dll"
        #----------------------------------------------------------------------
        # the name of the x64 release library distribution file
        _FILE_NAME_DISTRIBUTION_LIBRARY_X64_RELEASE = "OpenThreads.lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_LIBRARY_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_LIBRARY_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\OpenThreads"
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
        
            fileDistributor = FileDistributor()
            systemManager   = SystemManager()
            pathFinder      = PathFinder()
            
            # process command-line arguments
            buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)
            
            # initialize environment variables
            if ( buildSettings.X64Specified() ) :
            
                os.environ["PLATFORM"] = "X64"
                
            else :
            
                os.environ["PLATFORM"] = "X86"
                
            # determine path names
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)

            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )
                               
            # build the solution
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( ( "\"%s\" "                  + \
                                     "/p:Configuration=\"%s\" " + \
                                     "\"%s\""                   ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                     ( "Release"                               \
                                       if ( buildSettings.ReleaseSpecified() ) \
                                       else "Debug"                          )                     , \
                                     solutionFileName                                              ) )
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute files
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :

                 systemManager.copyFile( os.path.join( buildPathName                                                            , \
                                                       Program._FILE_NAME_BUILD_DYNAMIC_X64_RELEASE                               ) , \
                                         os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                       Program._FILE_NAME_DISTRIBUTION_DYNAMIC_X64_RELEASE                        ) );
                 systemManager.copyFile( os.path.join( buildPathName                                                            , \
                                                       Program._FILE_NAME_BUILD_LIBRARY_X64_RELEASE                               ) , \
                                         os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                       Program._FILE_NAME_DISTRIBUTION_LIBRARY_X64_RELEASE                        ) );
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 systemManager.copyFile( os.path.join( buildPathName                                                            , \
                                                       Program._FILE_NAME_BUILD_DYNAMIC_X86_RELEASE                               ) , \
                                         os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                       Program._FILE_NAME_DISTRIBUTION_DYNAMIC_X86_RELEASE                        ) );
                 systemManager.copyFile( os.path.join( buildPathName                                                            , \
                                                       Program._FILE_NAME_BUILD_LIBRARY_X86_RELEASE                               ) , \
                                         os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                       Program._FILE_NAME_DISTRIBUTION_LIBRARY_X86_RELEASE                        ) );

            elif ( buildSettings.X64Specified() ) :
            
                 systemManager.copyFile( os.path.join( buildPathName                                                            , \
                                                       Program._FILE_NAME_BUILD_DYNAMIC_X64_DEBUG                               ) , \
                                         os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                       Program._FILE_NAME_DISTRIBUTION_DYNAMIC_X64_DEBUG                        ) );
                 systemManager.copyFile( os.path.join( buildPathName                                                            , \
                                                       Program._FILE_NAME_BUILD_LIBRARY_X64_DEBUG                               ) , \
                                         os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                       Program._FILE_NAME_DISTRIBUTION_LIBRARY_X64_DEBUG                        ) );
                 
            else :
            
                 systemManager.copyFile( os.path.join( buildPathName                                                            , \
                                                       Program._FILE_NAME_BUILD_DYNAMIC_X86_DEBUG                               ) , \
                                         os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                       Program._FILE_NAME_DISTRIBUTION_DYNAMIC_X86_DEBUG                        ) );
                 systemManager.copyFile( os.path.join( buildPathName                                                            , \
                                                       Program._FILE_NAME_BUILD_LIBRARY_X86_DEBUG                               ) , \
                                         os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                       Program._FILE_NAME_DISTRIBUTION_LIBRARY_X86_DEBUG                        ) );

        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
