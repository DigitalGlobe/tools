#------------------------------------------------------------------------------
#
# build_quazip.py
#
# Summary : Builds the QuaZIP library.
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from FileDistributor import *
from PathFinder      import *
from SystemManager   import *
from UtilityFile     import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the QuaZIP library."
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "quazip\quazip\\quazip.vcxproj"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "QuaZIP"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X86 = "quazip\\quazip\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X86 = "quazip\\quazip\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X64 = "quazip\\quazip\\x64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X64 = "quazip\\quazip\\x64\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X86 = "quazip\\quazip\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X86 = "quazip\\quazip\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X64 = "quazip\\quazip\\x64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X64 = "quazip\\quazip\\x64\\Release"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\QuaZIP"
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
            os.environ["PLATFORM"] = ( "x64"                               \
                                       if ( buildSettings.X64Specified() ) \
                                       else "Win32"                        )
            
            # determine path names and path names
            buildPathName        = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName       = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            buildBinaryPathName  = os.path.join( buildPathName , \
                                                 fileDistributor.determinePathName( buildSettings.X64Specified()                 , \
                                                                                    buildSettings.ReleaseSpecified()             , \
                                                                                    Program._PATH_NAME_BUILD_BINARY_DEBUG_X86    , \
                                                                                    Program._PATH_NAME_BUILD_BINARY_RELEASE_X86  , \
                                                                                    Program._PATH_NAME_BUILD_BINARY_DEBUG_X64    , \
                                                                                    Program._PATH_NAME_BUILD_BINARY_RELEASE_X64  ) )
            buildLibraryPathName = os.path.join( buildPathName , \
                                                 fileDistributor.determinePathName( buildSettings.X64Specified()                 , \
                                                                                    buildSettings.ReleaseSpecified()             , \
                                                                                    Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X86   , \
                                                                                    Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X86 , \
                                                                                    Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X64   , \
                                                                                    Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X64 ) )

            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )


            # set up the Qt property files
            if ( buildSettings.X64Specified() ) :
            
                systemManager.copyFile( os.path.join( buildPathName   , \
                                                      "qt5_x64.props" ) ,
                                        os.path.join( buildPathName   , \
                                                      "qt4.props"     ) )
                systemManager.copyFile( os.path.join( buildPathName   , \
                                                      "qt5_x64.props" ) ,
                                        os.path.join( buildPathName   , \
                                                      "qt5.props"     ) )
                                                      
                qtLibraryPathName = os.path.normpath( os.path.join( buildPathName            , \
                                                                    "..\\..\sdk\\x64\\lib\\" ) )

            else :
            
                systemManager.copyFile( os.path.join( buildPathName   , \
                                                      "qt4_x86.props" ) ,
                                        os.path.join( buildPathName   , \
                                                      "qt4.props"     ) )
                systemManager.copyFile( os.path.join( buildPathName   , \
                                                      "qt4_x86.props" ) ,
                                        os.path.join( buildPathName   , \
                                                      "qt5.props"     ) )
                                                      
                qtLibraryPathName = os.path.normpath( os.path.join( buildPathName            , \
                                                                    "..\\..\sdk\\x86\\lib\\" ) )
                                                      
            qtIncludePathName = os.path.normpath( os.path.join( buildPathName               , \
                                                                "..\\\..\src\\\Qt\\\qtbase" ) )
            UtilityFile.replaceText( os.path.join( buildPathName   , \
                                                   "qt4.props"     ) , \
                                     "$(PATH_NAME_QT_INCLUDE)"       , \
                                     qtIncludePathName               )
            UtilityFile.replaceText( os.path.join( buildPathName   , \
                                                   "qt5.props"     ) , \
                                     "$(PATH_NAME_QT_INCLUDE)"       , \
                                     qtIncludePathName               )
            UtilityFile.replaceText( os.path.join( buildPathName   , \
                                                   "qt4.props"     ) , \
                                     "$(PATH_NAME_QT_LIBRARY)"       , \
                                     qtLibraryPathName               )
            UtilityFile.replaceText( os.path.join( buildPathName   , \
                                                   "qt5.props"     ) , \
                                     "$(PATH_NAME_QT_LIBRARY)"       , \
                                     qtLibraryPathName               )

            # build the solution
            systemManager.changeDirectory(buildPathName)
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( ( "\"%s\" "                  + \
                                     "/p:Configuration=\"%s\" " + \
                                     "\"%s\""                   ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                     ( "Release"                               \
                                       if ( buildSettings.ReleaseSpecified() ) \
                                       else "Debug"                            )                   , \
                                     solutionFileName                                              ) )
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute files
            fileDistributor.distributeAllFiles( buildBinaryPathName              , \
                                                buildLibraryPathName             , \
                                                buildLibraryPathName             , \
                                                buildLibraryPathName             , \
                                                buildSettings.X64Specified()     , \
                                                buildSettings.ReleaseSpecified() , \
                                                True                             )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
