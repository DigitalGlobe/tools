#------------------------------------------------------------------------------
#
# build_proj4.py
#
# Summary : Builds the PROJ.4 library.
#
#------------------------------------------------------------------------------

import glob
import os

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
        DESCRIPTION = "Builds the PROJ.4 library."
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the library file
        _FILE_NAME_LIBRARY = "proj.lib"
        #----------------------------------------------------------------------
        # the name of the makefile
        _FILE_NAME_MAKEFILE = "makefile.vc"
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
        _PATH_NAME_BUILD = "PROJ.4"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_LIBRARY_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_LIBRARY_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\PROJ.4"
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
            
            # determine path names
            binaryPathName = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64) \
                               if ( buildSettings.X64Specified() )                                     \
                               else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            # determine file names
            buildLibraryFileName = os.path.join( buildPathName              , \
                                                 "src"                      , \
                                                 Program._FILE_NAME_LIBRARY )
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                             Program._FILE_NAME_LIBRARY                                               )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                             Program._FILE_NAME_LIBRARY                                               )
                 
            elif ( buildSettings.X64Specified() ) :
            
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                             systemManager.getDebugFileName(Program._FILE_NAME_LIBRARY)               )
                 
            else :
            
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                             systemManager.getDebugFileName(Program._FILE_NAME_LIBRARY)               )
            

            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )
                               
            # execute Nmake
            nmakeCommandLine = ( "\"%s\" /f \"%s\" _MSC_VER=1900" % \
                                 ( pathFinder.getNmakeFileName( buildSettings.X64Specified() ) , \
                                   Program._FILE_NAME_MAKEFILE                                 ) )
            if ( buildSettings.ReleaseSpecified() ) :
            
                 nmakeCommandLine += " nodebug=1"
            
            systemManager.changeDirectory(buildPathName)
            nmakeResult = systemManager.execute(nmakeCommandLine)
   
            # copy the library and binary files, if Nmake executed successfully
            if (nmakeResult == 0) :
            
                # copy the binary files
                buildBinaryFileNames = glob.glob( os.path.join( buildPathName                , \
                                                                "src"                        , \
                                                                Program._FILE_PATTERN_BINARY ) );
                for buildBinaryFileName in buildBinaryFileNames :
                
                    binaryFileName = os.path.join( binaryPathName , \
                                                   ( os.path.basename(buildBinaryFileName) \
                                                     if ( buildSettings.ReleaseSpecified() ) \
                                                     else systemManager.getDebugFileName( os.path.basename(buildBinaryFileName) ) ) )
                    systemManager.copyFile( buildBinaryFileName , \
                                            binaryFileName      )
                
                # copy the library file
                systemManager.copyFile( buildLibraryFileName        , \
                                        distributionLibraryFileName )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    
#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
