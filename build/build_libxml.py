#------------------------------------------------------------------------------
#
# build_libxml.py
#
# Summary : Builds the LibXML library.
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder      import *
from SystemManager   import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the LibXML library."
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the build 32-bit debug debug file
        _FILE_NAME_DEBUG_BUILD_DEBUG_X86 = "win32\\bin.msvc\\libxml2.pdb"
        #----------------------------------------------------------------------
        # the name of the build 64-bit debug debug file
        _FILE_NAME_DEBUG_BUILD_DEBUG_X64 = "win32\\bin.msvc\\libxml2.pdb"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the build 32-bit debug dynamic file
        _FILE_NAME_DYNAMIC_BUILD_DEBUG_X86 = "win32\\bin.msvc\\libxml2.dll"
        #----------------------------------------------------------------------
        # the name of the build 32-bit release dynamic file
        _FILE_NAME_DYNAMIC_BUILD_RELEASE_X86 = "win32\\bin.msvc\\libxml2.dll"
        #----------------------------------------------------------------------
        # the name of the build 64-bit debug dynamic file
        _FILE_NAME_DYNAMIC_BUILD_DEBUG_X64 = "win32\\bin.msvc\\libxml2.dll"
        #----------------------------------------------------------------------
        # the name of the build 64-bit release dynamic file
        _FILE_NAME_DYNAMIC_BUILD_RELEASE_X64 = "win32\\bin.msvc\\libxml2.dll"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the build 32-bit debug library file
        _FILE_NAME_LIBRARY_BUILD_DEBUG_X86 = "win32\\bin.msvc\\libxml2.lib"
        #----------------------------------------------------------------------
        # the name of the build 32-bit release library file
        _FILE_NAME_LIBRARY_BUILD_RELEASE_X86 = "win32\\bin.msvc\\libxml2.lib"
        #----------------------------------------------------------------------
        # the name of the build 64-bit debug library file
        _FILE_NAME_LIBRARY_BUILD_DEBUG_X64 = "win32\\bin.msvc\\libxml2.lib"
        #----------------------------------------------------------------------
        # the name of the build 64-bit release library file
        _FILE_NAME_LIBRARY_BUILD_RELEASE_X64 = "win32\\bin.msvc\\libxml2.lib"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the distribution debug file
        _FILE_NAME_DEBUG_DISTRIBUTION = "libxml_d.pdb"
        #----------------------------------------------------------------------
        # the name of the distribution debug dynamic file
        _FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG = "libxml_d.dll"
        #----------------------------------------------------------------------
        # the name of the distribution release dynamic file
        _FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE = "libxml.dll"
        #----------------------------------------------------------------------
        # the name of the distribution debug library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG = "libxml_d.lib"
        #----------------------------------------------------------------------
        # the name of the distribution release library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE = "libxml.lib"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "win32\\VC10\\libxml2.sln"
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
        _PATH_NAME_BUILD = "LibXML"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\LibXML"
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
            
                os.environ["PLATFORM"] = "X64"
                
            print(os.environ["PATH"])
            
            # determine path names
            binaryPathName       = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64) \
                                     if ( buildSettings.X64Specified() )                                     \
                                     else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName        = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD       )
            sourcePathName       = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE      )
            iconvIncludePathName = os.path.normpath( os.path.join( sourcePathName          , \
                                                                   "..\\LibICONV\\include" ) )
            
            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DYNAMIC_BUILD_RELEASE_X64                                  ) 
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE                               )
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE_X64                                  ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 iconvLibraryFileName        = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             "libiconv.lib"                                                                )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DYNAMIC_BUILD_RELEASE_X86                                  ) 
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE                               )
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE_X86                                  ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 iconvLibraryFileName        = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             "libiconv.lib"                                                                )
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DEBUG_BUILD_DEBUG_X64                                      ) 
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION                                         )
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DYNAMIC_BUILD_DEBUG_X64                                    ) 
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG                                 )
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG_X64                                    ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 iconvLibraryFileName        = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             "libiconv_d.lib"                                                              )
                 
            else :
            
                 buildDebugFileName          = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DEBUG_BUILD_DEBUG_X86                                      ) 
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DEBUG_DISTRIBUTION                                         )
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_DYNAMIC_BUILD_DEBUG_X86                                    ) 
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG                                 )
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG_X86                                    ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 iconvLibraryFileName        = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             "libiconv_d.lib"                                                              )
                 
            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )
                                         
            # run the configuration script
            systemManager.changeDirectory( os.path.join( buildPathName ,
                                                         "win32"       ) )
            scriptCommandLine = ( "cscript.exe configure.js compiler=msbc prefix=\"%s\" include=\"%s\" lib=\"%s\" debug=%s" % \
                                  ( buildPathName        , \
                                    iconvIncludePathName , \
                                    buildPathName        , \
                                    ( "no"                                    \
                                      if ( buildSettings.ReleaseSpecified() ) \
                                      else "yes"                              ) )                                           )
            scriptResult      = systemManager.execute(scriptCommandLine)
            if (scriptResult != 0) :

                sys.exit(-1)
                
            # copy the LibICONV library
            systemManager.makeDirectory( os.path.join( buildPathName , \
                                                       "lib"         ) )
            systemManager.copyFile( iconvLibraryFileName             , \
                                    os.path.join( buildPathName    , \
                                                  "lib\\iconv.lib" )   )
                                         
            # run Nmake
            nmakeCommandLine = "nmake"
            nmakeResult      = systemManager.execute(nmakeCommandLine)
            
            # distribute files
            if ( not buildSettings.ReleaseSpecified() ) :
            
                systemManager.copyFile( buildDebugFileName        , \
                                        distributionDebugFileName )

            systemManager.copyFile( buildDynamicFileName        , \
                                    distributionDynamicFileName )
            systemManager.copyFile( buildLibraryFileName        , \
                                    distributionLibraryFileName )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
