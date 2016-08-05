#------------------------------------------------------------------------------
#
# build_szip.py
#
# Summary : Builds the SZip library.
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
        DESCRIPTION = "Builds the SZip library."
        #----------------------------------------------------------------------
        # the name of the dynamic solution file
        _FILE_NAME_SOLUTION_DYNAMIC = "windows\\dynamic\\szip_dynamic.sln"
        #----------------------------------------------------------------------
        # the name of the static solution file
        _FILE_NAME_SOLUTION_STATIC = "windows\\static\\szip_static.sln"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "SZip"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X86_DYNAMIC = "windows\\dynamic\\lib\\Win32\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X86_DYNAMIC = "windows\\dynamic\\lib\\Win32\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X64_DYNAMIC = "windows\\dynamic\\lib\\X64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X64_DYNAMIC = "windows\\dynamic\\lib\\X64\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug debug
        # files
        _PATH_NAME_BUILD_DEBUG_DEBUG_X86_DYNAMIC = "windows\\dynamic\\lib\\Win32\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release debug
        # files
        _PATH_NAME_BUILD_DEBUG_RELEASE_X86_DYNAMIC = "windows\\dynamic\\lib\\Win32\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug debug
        # files
        _PATH_NAME_BUILD_DEBUG_DEBUG_X64_DYNAMIC = "windows\\dynamic\\lib\\X64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release debug
        # files
        _PATH_NAME_BUILD_DEBUG_RELEASE_X64_DYNAMIC = "windows\\dynamic\\lib\\X64\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X86_DYNAMIC = "windows\\dynamic\\lib\\Win32\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X86_DYNAMIC = "windows\\dynamic\\lib\\Win32\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X64_DYNAMIC = "windows\\dynamic\\lib\\X64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X64_DYNAMIC = "windows\\dynamic\\lib\\X64\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X86_STATIC = "windows\\static\\lib\\Win32\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X86_STATIC = "windows\\static\\lib\\Win32\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X64_STATIC = "windows\\static\\lib\\X64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X64_STATIC = "windows\\static\\lib\\X64\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug debug
        # files
        _PATH_NAME_BUILD_DEBUG_DEBUG_X86_STATIC = "windows\\static\\lib\\Win32\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release debug
        # files
        _PATH_NAME_BUILD_DEBUG_RELEASE_X86_STATIC = "windows\\static\\lib\\Win32\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug debug
        # files
        _PATH_NAME_BUILD_DEBUG_DEBUG_X64_STATIC = "windows\\static\\lib\\X64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release debug
        # files
        _PATH_NAME_BUILD_DEBUG_RELEASE_X64_STATIC = "windows\\static\\lib\\X64\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X86_STATIC = "windows\\static\\lib\\Win32\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X86_STATIC = "windows\\static\\lib\\Win32\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X64_STATIC = "windows\\static\\lib\\X64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X64_STATIC = "windows\\static\\lib\\X64\\Release"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\SZip"
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
            
            # determine path names and path names
            buildPathName               = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName              = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            buildDynamicBinaryPathName  = os.path.join( buildPathName , \
                                                        fileDistributor.determinePathName( buildSettings.X64Specified()                         , \
                                                                                          buildSettings.ReleaseSpecified()                      , \
                                                                                          Program._PATH_NAME_BUILD_BINARY_DEBUG_X86_DYNAMIC     , \
                                                                                          Program._PATH_NAME_BUILD_BINARY_RELEASE_X86_DYNAMIC   , \
                                                                                          Program._PATH_NAME_BUILD_BINARY_DEBUG_X64_DYNAMIC     , \
                                                                                          Program._PATH_NAME_BUILD_BINARY_RELEASE_X64_DYNAMIC   ) )
            buildDynamicDebugPathName   = os.path.join( buildPathName , \
                                                        fileDistributor.determinePathName( buildSettings.X64Specified()                         , \
                                                                                           buildSettings.ReleaseSpecified()                     , \
                                                                                           Program._PATH_NAME_BUILD_DEBUG_DEBUG_X86_DYNAMIC     , \
                                                                                           Program._PATH_NAME_BUILD_DEBUG_RELEASE_X86_DYNAMIC   , \
                                                                                           Program._PATH_NAME_BUILD_DEBUG_DEBUG_X64_DYNAMIC     , \
                                                                                           Program._PATH_NAME_BUILD_DEBUG_RELEASE_X64_DYNAMIC   ) )
            buildDynamicLibraryPathName = os.path.join( buildPathName , \
                                                        fileDistributor.determinePathName( buildSettings.X64Specified()                         , \
                                                                                           buildSettings.ReleaseSpecified()                     , \
                                                                                           Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X86_DYNAMIC   , \
                                                                                           Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X86_DYNAMIC , \
                                                                                           Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X64_DYNAMIC   , \
                                                                                           Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X64_DYNAMIC ) )
            buildStaticBinaryPathName   = os.path.join( buildPathName , \
                                                        fileDistributor.determinePathName( buildSettings.X64Specified()                         , \
                                                                                          buildSettings.ReleaseSpecified()                      , \
                                                                                          Program._PATH_NAME_BUILD_BINARY_DEBUG_X86_STATIC      , \
                                                                                          Program._PATH_NAME_BUILD_BINARY_RELEASE_X86_STATIC    , \
                                                                                          Program._PATH_NAME_BUILD_BINARY_DEBUG_X64_STATIC      , \
                                                                                          Program._PATH_NAME_BUILD_BINARY_RELEASE_X64_STATIC    ) )
            buildStaticDebugPathName    = os.path.join( buildPathName , \
                                                        fileDistributor.determinePathName( buildSettings.X64Specified()                         , \
                                                                                           buildSettings.ReleaseSpecified()                     , \
                                                                                           Program._PATH_NAME_BUILD_DEBUG_DEBUG_X86_STATIC      , \
                                                                                           Program._PATH_NAME_BUILD_DEBUG_RELEASE_X86_STATIC    , \
                                                                                           Program._PATH_NAME_BUILD_DEBUG_DEBUG_X64_STATIC      , \
                                                                                           Program._PATH_NAME_BUILD_DEBUG_RELEASE_X64_STATIC    ) )
            buildStaticLibraryPathName  = os.path.join( buildPathName , \
                                                        fileDistributor.determinePathName( buildSettings.X64Specified()                         , \
                                                                                           buildSettings.ReleaseSpecified()                     , \
                                                                                           Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X86_STATIC    , \
                                                                                           Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X86_STATIC  , \
                                                                                           Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X64_STATIC    , \
                                                                                           Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X64_STATIC  ) )


            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )

            # build the static solution
            staticSolutionFileName = os.path.join( buildPathName                      , \
                                                   Program._FILE_NAME_SOLUTION_STATIC )
            msBuildCommandLine     = ( ( "\"%s\" "                  + \
                                         "/p:Configuration=\"%s\" " + \
                                         "\"%s\""                   ) % \
                                       ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                         ( "Release"                             \
                                           if ( buildSettings.ReleaseSpecified() ) \
                                           else "Debug"                          )                     , \
                                         staticSolutionFileName                                        ) )
            msbuildResult          = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # build the dynamic solution
            dynamicSolutionFileName = os.path.join( buildPathName                      , \
                                                   Program._FILE_NAME_SOLUTION_DYNAMIC )
            msBuildCommandLine      = ( ( "\"%s\" "                  + \
                                          "/p:Configuration=\"%s\" " + \
                                          "\"%s\""                   ) % \
                                        ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                          ( "Release"                             \
                                            if ( buildSettings.ReleaseSpecified() ) \
                                            else "Debug"                          )                     , \
                                          dynamicSolutionFileName                                       ) )
            msbuildResult           = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute dynamic files
            fileDistributor.distributeAllFiles( buildDynamicBinaryPathName       , \
                                                buildDynamicDebugPathName        , \
                                                buildDynamicLibraryPathName      , \
                                                buildDynamicLibraryPathName      , \
                                                buildSettings.X64Specified()     , \
                                                buildSettings.ReleaseSpecified() , \
                                                True                             , \
                                                {                                              \
                                                    "szipd.dll"        : "libszip_dynamic.dll" , \
                                                    "szip.dll"         : "libszip_dynamic.dll" , \
                                                    "szip_dynamic.lib" : "libszip_dynamic.lib" , \
                                                    "szip_dynamic.pdb" : "libszip_dynamic.pdb" \
                                                }                                )

            # distribute static files
            fileDistributor.distributeAllFiles( buildStaticBinaryPathName        , \
                                                buildStaticDebugPathName         , \
                                                buildStaticLibraryPathName       , \
                                                buildStaticLibraryPathName       , \
                                                buildSettings.X64Specified()     , \
                                                buildSettings.ReleaseSpecified() , \
                                                True                             , \
                                                {                                              \
                                                    "szip_static.lib" : "libszip.lib" , \
                                                    "szip_static.pdb" : "libszip.pdb" \
                                                }                                )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
