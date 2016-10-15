#------------------------------------------------------------------------------
#
# build_aprutil.py
#
# Summary : Builds the APR Util library.
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
        DESCRIPTION = "Builds the APR Util library."
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "aprutil.sln"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "AprUtil"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\AprUtil"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X86 = "Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X86 = "Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug binary
        # files
        _PATH_NAME_BUILD_BINARY_DEBUG_X64 = "x64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release binary
        # files
        _PATH_NAME_BUILD_BINARY_RELEASE_X64 = "x64\\Release"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X86 = "Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X86 = "Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_DEBUG_X64 = "x64\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_RELEASE_X64 = "x64\\Release"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_ALTERNATE_DEBUG_X86 = "xml\\expat\\lib\\LibD"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_ALTERNATE_RELEASE_X86 = "xml\\expat\\lib\\LibR"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug library
        # files
        _PATH_NAME_BUILD_LIBRARY_ALTERNATE_DEBUG_X64 = "xml\\expat\\lib\\LibD"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release library
        # files
        _PATH_NAME_BUILD_LIBRARY_ALTERNATE_RELEASE_X64 = "xml\\expat\\lib\\LibR"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug binary
        # files
        _PATH_NAME_DISTRIBUTION_BINARY_DEBUG_X86 = "..\\..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release binary
        # files
        _PATH_NAME_DISTRIBUTION_BINARY_RELEASE_X86 = "..\\..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug binary
        # files
        _PATH_NAME_DISTRIBUTION_BINARY_DEBUG_X64 = "..\\..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release binary
        # files
        _PATH_NAME_DISTRIBUTION_BINARY_RELEASE_X64 = "..\\..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit debug library
        # files
        _PATH_NAME_DISTRIBUTION_LIBRARY_DEBUG_X86 = "..\\..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit release library
        # files
        _PATH_NAME_DISTRIBUTION_LIBRARY_RELEASE_X86 = "..\\..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit debug library
        # files
        _PATH_NAME_DISTRIBUTION_LIBRARY_DEBUG_X64 = "..\\..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit release library
        # files
        _PATH_NAME_DISTRIBUTION_LIBRARY_RELEASE_X64 = "..\\..\\sdk\\x64\\lib"
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
            systemManager.initializeIncludeEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.initializeLibraryEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.appendToPathEnvironmentVariable( pathFinder.getVisualStudioBinPathName( buildSettings.X64Specified() ) )
            os.environ["PLATFORM"] = ( "x64"                               \
                                       if ( buildSettings.X64Specified() ) \
                                       else "x86"                          )
            
            # determine path names and path names
            buildPathName                 = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName                = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            buildBinaryPathName           = os.path.join( buildPathName , \
                                                          fileDistributor.determinePathName( buildSettings.X64Specified()                 , \
                                                                                             buildSettings.ReleaseSpecified()             , \
                                                                                             Program._PATH_NAME_BUILD_BINARY_DEBUG_X86    , \
                                                                                             Program._PATH_NAME_BUILD_BINARY_RELEASE_X86  , \
                                                                                             Program._PATH_NAME_BUILD_BINARY_DEBUG_X64    , \
                                                                                             Program._PATH_NAME_BUILD_BINARY_RELEASE_X64  ) )
            buildLibraryPathName          = os.path.join( buildPathName , \
                                                          fileDistributor.determinePathName( buildSettings.X64Specified()                 , \
                                                                                             buildSettings.ReleaseSpecified()             , \
                                                                                             Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X86   , \
                                                                                             Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X86 , \
                                                                                             Program._PATH_NAME_BUILD_LIBRARY_DEBUG_X64   , \
                                                                                             Program._PATH_NAME_BUILD_LIBRARY_RELEASE_X64 ) )
            buildLibraryAlternatePathName = os.path.join( buildPathName , \
                                                          fileDistributor.determinePathName( buildSettings.X64Specified()                 , \
                                                                                             buildSettings.ReleaseSpecified()             , \
                                                                                             Program._PATH_NAME_BUILD_LIBRARY_ALTERNATE_DEBUG_X86   , \
                                                                                             Program._PATH_NAME_BUILD_LIBRARY_ALTERNATE_RELEASE_X86 , \
                                                                                             Program._PATH_NAME_BUILD_LIBRARY_ALTERNATE_DEBUG_X64   , \
                                                                                             Program._PATH_NAME_BUILD_LIBRARY_ALTERNATE_RELEASE_X64 ) )
            distributionBinaryPathName    = os.path.join( buildPathName , \
                                                          fileDistributor.determinePathName( buildSettings.X64Specified()                        , \
                                                                                             buildSettings.ReleaseSpecified()                    , \
                                                                                             Program._PATH_NAME_DISTRIBUTION_BINARY_DEBUG_X86    , \
                                                                                             Program._PATH_NAME_DISTRIBUTION_BINARY_RELEASE_X86  , \
                                                                                             Program._PATH_NAME_DISTRIBUTION_BINARY_DEBUG_X64    , \
                                                                                             Program._PATH_NAME_DISTRIBUTION_BINARY_RELEASE_X64  ) )
            distributionLibraryPathName   = os.path.join( buildPathName , \
                                                          fileDistributor.determinePathName( buildSettings.X64Specified()                        , \
                                                                                             buildSettings.ReleaseSpecified()                    , \
                                                                                             Program._PATH_NAME_DISTRIBUTION_LIBRARY_DEBUG_X86   , \
                                                                                             Program._PATH_NAME_DISTRIBUTION_LIBRARY_RELEASE_X86 , \
                                                                                             Program._PATH_NAME_DISTRIBUTION_LIBRARY_DEBUG_X64   , \
                                                                                             Program._PATH_NAME_DISTRIBUTION_LIBRARY_RELEASE_X64 ) )


            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )
                               
            # build the solution
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( ( "\"%s\" "                  + \
                                     "/p:Configuration=\"%s\" " + \
                                     "/t:xml;libaprutil "       + \
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
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                systemManager.copyFile( os.path.join( buildLibraryPathName          , \
                                                      "libaprutil-1.lib"            ) , \
                                        os.path.join( distributionLibraryPathName   , \
                                                      "aprutil.lib"                 ) )
                systemManager.copyFile( os.path.join( buildLibraryPathName          , \
                                                      "libaprutil-1.dll"            ) , \
                                        os.path.join( distributionLibraryPathName   , \
                                                      "aprutil.dll"                 ) )
                 
                systemManager.copyFile( os.path.join( buildLibraryAlternatePathName , \
                                                      "xml.lib"                     ) , \
                                        os.path.join( distributionLibraryPathName   , \
                                                      "aprxml.lib"                  ) )
#                systemManager.copyFile( os.path.join( buildLibraryAlternatePathName , \
#                                                      "xml.dll"                     ) , \
#                                        os.path.join( distributionLibraryPathName   , \
#                                                      "aprxml.dll"                  ) )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                systemManager.copyFile( os.path.join( buildLibraryPathName          , \
                                                      "libaprutil-1.lib"            ) , \
                                        os.path.join( distributionLibraryPathName   , \
                                                      "aprutil.lib"                 ) )
                systemManager.copyFile( os.path.join( buildLibraryPathName          , \
                                                      "libaprutil-1.dll"            ) , \
                                        os.path.join( distributionLibraryPathName   , \
                                                      "aprutil.dll"                 ) )
                 
                systemManager.copyFile( os.path.join( buildLibraryAlternatePathName , \
                                                      "xml.lib"                     ) , \
                                        os.path.join( distributionLibraryPathName   , \
                                                      "aprxml.lib"                  ) )
#                systemManager.copyFile( os.path.join( buildLibraryAlternatePathName , \
#                                                      "xml.dll"                     ) , \
#                                        os.path.join( distributionLibraryPathName   , \
#                                                      "aprxml.dll"                  ) )
                 
            elif ( buildSettings.X64Specified() ) :
            
                systemManager.copyFile( os.path.join( buildLibraryPathName        , \
                                                      "libaprutil-1.lib"          ) , \
                                        os.path.join( distributionLibraryPathName , \
                                                      "aprutil_d.lib"             ) )
                systemManager.copyFile( os.path.join( buildLibraryPathName        , \
                                                      "libaprutil-1.dll"          ) , \
                                        os.path.join( distributionLibraryPathName , \
                                                      "aprutil_d.dll"             ) )
                 
                systemManager.copyFile( os.path.join( buildLibraryAlternatePathName , \
                                                      "xml.lib"                     ) , \
                                        os.path.join( distributionLibraryPathName   , \
                                                      "aprxml_d.lib"                ) )
#                systemManager.copyFile( os.path.join( buildLibraryAlternatePathName , \
#                                                      "xml.dll"                     ) , \
#                                        os.path.join( distributionLibraryPathName   , \
#                                                      "aprxml_d.dll"                ) )
                 
            else :
            
                systemManager.copyFile( os.path.join( buildLibraryPathName        , \
                                                      "libaprutil-1.lib"          ) , \
                                        os.path.join( distributionLibraryPathName , \
                                                      "aprutil_d.lib"             ) )
                systemManager.copyFile( os.path.join( buildLibraryPathName        , \
                                                      "libaprutil-1.dll"          ) , \
                                        os.path.join( distributionLibraryPathName , \
                                                      "aprutil_d.dll"             ) )
                                                      
                systemManager.copyFile( os.path.join( buildLibraryAlternatePathName , \
                                                      "xml.lib"                     ) , \
                                        os.path.join( distributionLibraryPathName   , \
                                                      "aprxml_d.lib"                ) )
#                systemManager.copyFile( os.path.join( buildLibraryAlternatePathName , \
#                                                      "xml.dll"                     ) , \
#                                        os.path.join( distributionLibraryPathName   , \
#                                                      "aprxml_d.dll"                ) )

        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
