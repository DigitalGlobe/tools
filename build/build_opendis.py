#------------------------------------------------------------------------------
#
# build_opendis.py
#
# Summary : Builds the OpenDIS library.
#
#------------------------------------------------------------------------------

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
        DESCRIPTION = "Builds the OpenDIS library."
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "Compile\\vs2005\\DIS.sln"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the debug debug X86 build file
        _FILE_NAME_BUILD_DEBUG_DEBUG_X86 = "bin\\DIS_debug.pdb"
        #----------------------------------------------------------------------
        # the name of the debug debug X64 build file
        _FILE_NAME_BUILD_DEBUG_DEBUG_X64 = "Compile\\vs2005\\x64\\Debug\\DIS_debug.pdb"
        #----------------------------------------------------------------------
        # the name of the debug debug X86 distribution file
        _FILE_NAME_DISTRIBUTION_DEBUG_DEBUG_X86 = "..\\sdk\\x86\\lib\\DIS_d.pdb"
        #----------------------------------------------------------------------
        # the name of the debug release X86 distribution file
        _FILE_NAME_DISTRIBUTION_DEBUG_RELEASE_X86 = "..\\sdk\\x86\\lib\\DIS.pdb"
        #----------------------------------------------------------------------
        # the name of the debug debug X64 distribution file
        _FILE_NAME_DISTRIBUTION_DEBUG_DEBUG_X64 = "..\\sdk\\x64\\lib\\DIS_d.pdb"
        #----------------------------------------------------------------------
        # the name of the debug release X64 distribution file
        _FILE_NAME_DISTRIBUTION_DEBUG_RELEASE_X64 = "..\\sdk\\x64\\lib\\DIS.pdb"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the dynamic debug X86 build file
        _FILE_NAME_BUILD_DYNAMIC_DEBUG_X86 = "bin\\DIS_debug.dll"
        #----------------------------------------------------------------------
        # the name of the dynamic release X86 build file
        _FILE_NAME_BUILD_DYNAMIC_RELEASE_X86 = "bin\\DIS.dll"
        #----------------------------------------------------------------------
        # the name of the dynamic debug X64 build file
        _FILE_NAME_BUILD_DYNAMIC_DEBUG_X64 = "Compile\\vs2005\\x64\\Debug\\DIS_debug.dll"
        #----------------------------------------------------------------------
        # the name of the dynamic release X64 build file
        _FILE_NAME_BUILD_DYNAMIC_RELEASE_X64 = "Compile\\vs2005\\x64\\Release\\DIS.dll"
        #----------------------------------------------------------------------
        # the name of the dynamic debug X86 distribution file
        _FILE_NAME_DISTRIBUTION_DYNAMIC_DEBUG_X86 = "..\\sdk\\x86\\lib\\DIS_d.dll"
        #----------------------------------------------------------------------
        # the name of the dynamic release X86 distribution file
        _FILE_NAME_DISTRIBUTION_DYNAMIC_RELEASE_X86 = "..\\sdk\\x86\\lib\\DIS.dll"
        #----------------------------------------------------------------------
        # the name of the dynamic debug X64 distribution file
        _FILE_NAME_DISTRIBUTION_DYNAMIC_DEBUG_X64 = "..\\sdk\\x64\\lib\\DIS_d.dll"
        #----------------------------------------------------------------------
        # the name of the dynamic release X64 distribution file
        _FILE_NAME_DISTRIBUTION_DYNAMIC_RELEASE_X64 = "..\\sdk\\x64\\lib\\DIS.dll"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the library debug X86 build file
        _FILE_NAME_BUILD_LIBRARY_DEBUG_X86 = "lib\\DIS_debug.lib"
        #----------------------------------------------------------------------
        # the name of the library release X86 build file
        _FILE_NAME_BUILD_LIBRARY_RELEASE_X86 = "lib\\DIS.lib"
        #----------------------------------------------------------------------
        # the name of the library debug X64 build file
        _FILE_NAME_BUILD_LIBRARY_DEBUG_X64 = "Compile\\vs2005\\x64\\Debug\\DIS_debug.lib"
        #----------------------------------------------------------------------
        # the name of the library release X64 build file
        _FILE_NAME_BUILD_LIBRARY_RELEASE_X64 = "Compile\\vs2005\\x64\\Release\\DIS.lib"
        #----------------------------------------------------------------------
        # the name of the library debug X86 distribution file
        _FILE_NAME_DISTRIBUTION_LIBRARY_DEBUG_X86 = "..\\sdk\\x86\\lib\\DIS_d.lib"
        #----------------------------------------------------------------------
        # the name of the library release X86 distribution file
        _FILE_NAME_DISTRIBUTION_LIBRARY_RELEASE_X86 = "..\\sdk\\x86\\lib\\DIS.lib"
        #----------------------------------------------------------------------
        # the name of the library debug X64 distribution file
        _FILE_NAME_DISTRIBUTION_LIBRARY_DEBUG_X64 = "..\\sdk\\x64\\lib\\DIS_d.lib"
        #----------------------------------------------------------------------
        # the name of the library release X64 distribution file
        _FILE_NAME_DISTRIBUTION_LIBRARY_RELEASE_X64 = "..\\sdk\\x64\\lib\\DIS.lib"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the path that will contain built debug binary files
        _PATH_NAME_BINARY_DEBUG = "bin\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built release binary files
        _PATH_NAME_BINARY_RELEASE = "bin\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "OpenDIS"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\OpenDIS"
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
            if ( buildSettings.X64Specified() ) :
            
                os.environ["PLATFORM"] = "X64"
            
            # determine path and file names
            rootPathName    = systemManager.getCurrentRelativePathName(".")
            buildPathName   = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildDynamicFileName        = os.path.join( buildPathName                                       , \
                                                             Program._FILE_NAME_BUILD_DYNAMIC_RELEASE_X64        )
                 distributionDynamicFileName = os.path.join( rootPathName                                        , \
                                                             Program._FILE_NAME_DISTRIBUTION_DYNAMIC_RELEASE_X64 )
                 buildLibraryFileName        = os.path.join( buildPathName                                       , \
                                                             Program._FILE_NAME_BUILD_LIBRARY_RELEASE_X64        )
                 distributionLibraryFileName = os.path.join( rootPathName                                        , \
                                                             Program._FILE_NAME_DISTRIBUTION_LIBRARY_RELEASE_X64 )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildDynamicFileName        = os.path.join( buildPathName                                       , \
                                                             Program._FILE_NAME_BUILD_DYNAMIC_RELEASE_X86        )
                 distributionDynamicFileName = os.path.join( rootPathName                                        , \
                                                             Program._FILE_NAME_DISTRIBUTION_DYNAMIC_RELEASE_X86 )
                 buildLibraryFileName        = os.path.join( buildPathName                                       , \
                                                             Program._FILE_NAME_BUILD_LIBRARY_RELEASE_X86        )
                 distributionLibraryFileName = os.path.join( rootPathName                                        , \
                                                             Program._FILE_NAME_DISTRIBUTION_LIBRARY_RELEASE_X86 )
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildDebugFileName          = os.path.join( buildPathName                                     , \
                                                             Program._FILE_NAME_BUILD_DEBUG_DEBUG_X64          )
                 distributionDebugFileName   = os.path.join( rootPathName                                      , \
                                                             Program._FILE_NAME_DISTRIBUTION_DEBUG_DEBUG_X64   )
                 buildDynamicFileName        = os.path.join( buildPathName                                     , \
                                                             Program._FILE_NAME_BUILD_DYNAMIC_DEBUG_X64        )
                 distributionDynamicFileName = os.path.join( rootPathName                                      , \
                                                             Program._FILE_NAME_DISTRIBUTION_DYNAMIC_DEBUG_X64 )
                 buildLibraryFileName        = os.path.join( buildPathName                                     , \
                                                             Program._FILE_NAME_BUILD_LIBRARY_DEBUG_X64        )
                 distributionLibraryFileName = os.path.join( rootPathName                                      , \
                                                             Program._FILE_NAME_DISTRIBUTION_LIBRARY_DEBUG_X64 )

            else :
            
                 buildDebugFileName          = os.path.join( buildPathName                                     , \
                                                             Program._FILE_NAME_BUILD_DEBUG_DEBUG_X86          )
                 distributionDebugFileName   = os.path.join( rootPathName                                      , \
                                                             Program._FILE_NAME_DISTRIBUTION_DEBUG_DEBUG_X86   )
                 buildDynamicFileName        = os.path.join( buildPathName                                     , \
                                                             Program._FILE_NAME_BUILD_DYNAMIC_DEBUG_X86        )
                 distributionDynamicFileName = os.path.join( rootPathName                                      , \
                                                             Program._FILE_NAME_DISTRIBUTION_DYNAMIC_DEBUG_X86 )
                 buildLibraryFileName        = os.path.join( buildPathName                                     , \
                                                             Program._FILE_NAME_BUILD_LIBRARY_DEBUG_X86        )
                 distributionLibraryFileName = os.path.join( rootPathName                                      , \
                                                             Program._FILE_NAME_DISTRIBUTION_LIBRARY_DEBUG_X86 )

            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )

            # run MSBuild
            msBuildCommandLine = ( ( "\"%s\" "              + \
                                     "/t:DIS "              + \
                                     "/p:Configuration=%s " + \
                                     "\"%s\""               ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                   ( "Release"                               \
                                     if ( buildSettings.ReleaseSpecified() ) \
                                     else "Debug"                          )                       , \
                                   Program._FILE_NAME_SOLUTION                                     ) )
            systemManager.changeDirectory(buildPathName)
            msBuildResult = systemManager.execute(msBuildCommandLine)
            if (msBuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute files
            if ( not buildSettings.ReleaseSpecified() ) :
            
                systemManager.copyFile( buildDebugFileName          , \
                                        distributionDebugFileName   )

            systemManager.copyFile( buildDynamicFileName        , \
                                    distributionDynamicFileName )
            systemManager.copyFile( buildLibraryFileName        , \
                                    distributionLibraryFileName )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
