#------------------------------------------------------------------------------
#
# build_libgeotiff.py
#
# Summary : Builds the LibGeoTIFF library.
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
        DESCRIPTION = "Builds the LibGeoTIFF library."
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the library file
        _FILE_NAME_LIBRARY = "geotiff.lib"
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
        _PATH_NAME_BUILD = "LibGeoTIFF"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_LIBRARY_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_LIBRARY_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\LibGeoTIFF"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the environment variable for the LibTIFF include
        _ENVIRONMENT_VARIABLE_LIBTIFF_INCLUDE = "TIFF_INC"
        #----------------------------------------------------------------------
        # the name of the environment variable for the LibTIFF library
        _ENVIRONMENT_VARIABLE_LIBTIFF_LIBRARY = "TIFF_LIB"
        #----------------------------------------------------------------------
        # the name of the 32-bit debug LibTIFF library file
        _FILE_NAME_LIBTIFF_LIBRARY_X86_DEBUG = "..\\sdk\\x86\\lib\\libtiff_d.lib"
        #----------------------------------------------------------------------
        # the name of the 32-bit release LibTIFF library file
        _FILE_NAME_LIBTIFF_LIBRARY_X86_RELEASE = "..\\sdk\\x86\\lib\\libtiff.lib"
        #----------------------------------------------------------------------
        # the name of the 64-bit debug LibTIFF library file
        _FILE_NAME_LIBTIFF_LIBRARY_X64_DEBUG = "..\\sdk\\x64\\lib\\libtiff_d.lib"
        #----------------------------------------------------------------------
        # the name of the 64-bit release LibTIFF library file
        _FILE_NAME_LIBTIFF_LIBRARY_X64_RELEASE = "..\\sdk\\x64\\lib\\libtiff.lib"
        #----------------------------------------------------------------------
        # the name of the LibTIFF include path
        _PATH_NAME_LIBTIFF_INCLUDE = "..\\src\\LibTIFF\\libtiff"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the environment variable for the PROJ4 include
        _ENVIRONMENT_VARIABLE_PROJ4_INCLUDE = "PROJ_INC"
        #----------------------------------------------------------------------
        # the name of the environment variable for the PROJ4 library
        _ENVIRONMENT_VARIABLE_PROJ4_LIBRARY = "PROJ_LIB"
        #----------------------------------------------------------------------
        # the name of the 32-bit debug PROJ4 library file
        _FILE_NAME_PROJ4_LIBRARY_X86_DEBUG = "..\\sdk\\x86\\lib\\proj_d.lib"
        #----------------------------------------------------------------------
        # the name of the 32-bit release PROJ4 library file
        _FILE_NAME_PROJ4_LIBRARY_X86_RELEASE = "..\\sdk\\x86\\lib\\proj.lib"
        #----------------------------------------------------------------------
        # the name of the 64-bit debug PROJ4 library file
        _FILE_NAME_PROJ4_LIBRARY_X64_DEBUG = "..\\sdk\\x64\\lib\\proj_d.lib"
        #----------------------------------------------------------------------
        # the name of the 64-bit release PROJ4 library file
        _FILE_NAME_PROJ4_LIBRARY_X64_RELEASE = "..\\sdk\\x64\\lib\\proj.lib"
        #----------------------------------------------------------------------
        # the name of the PROJ4 include path
        _PATH_NAME_PROJ4_INCLUDE = "..\\src\\PROJ.4\\src"
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
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            # determine file names
            buildLibraryFileName = os.path.join( buildPathName              , \
                                                 Program._FILE_NAME_LIBRARY )
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                            Program._FILE_NAME_LIBRARY                                               )
                libTiffLibraryFileName      = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBTIFF_LIBRARY_X64_RELEASE)
                proj4LibraryFileName        = systemManager.getCurrentRelativePathName(Program._FILE_NAME_PROJ4_LIBRARY_X64_RELEASE  )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                            Program._FILE_NAME_LIBRARY                                               )
                libTiffLibraryFileName      = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBTIFF_LIBRARY_X86_RELEASE)
                proj4LibraryFileName        = systemManager.getCurrentRelativePathName(Program._FILE_NAME_PROJ4_LIBRARY_X86_RELEASE  )
                 
            elif ( buildSettings.X64Specified() ) :
            
                distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                            systemManager.getDebugFileName(Program._FILE_NAME_LIBRARY)               )
                libTiffLibraryFileName      = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBTIFF_LIBRARY_X64_DEBUG)
                proj4LibraryFileName        = systemManager.getCurrentRelativePathName(Program._FILE_NAME_PROJ4_LIBRARY_X64_DEBUG  )
                 
            else :
            
                distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                            systemManager.getDebugFileName(Program._FILE_NAME_LIBRARY)               )
                libTiffLibraryFileName      = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBTIFF_LIBRARY_X86_DEBUG)
                proj4LibraryFileName        = systemManager.getCurrentRelativePathName(Program._FILE_NAME_PROJ4_LIBRARY_X86_DEBUG  )
            
            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )
                                         
            # initialize LibTIFF environment variables
            os.environ[Program._ENVIRONMENT_VARIABLE_LIBTIFF_INCLUDE] = ( "-I"                                                                         + \
                                                                          systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBTIFF_INCLUDE) )
            os.environ[Program._ENVIRONMENT_VARIABLE_LIBTIFF_LIBRARY] = libTiffLibraryFileName
            
            # initialize PROJ4 environment variables
            os.environ[Program._ENVIRONMENT_VARIABLE_PROJ4_INCLUDE] = ( "-I"                                                                       + \
                                                                        systemManager.getCurrentRelativePathName(Program._PATH_NAME_PROJ4_INCLUDE) )
            os.environ[Program._ENVIRONMENT_VARIABLE_PROJ4_LIBRARY] = proj4LibraryFileName
            
            # initialize Nmake environment variables
            os.environ["COMPILE_FLAGS"] = ( "/Ox"                               \
                                            if ( buildSettings.X64Specified() ) \
                                            else "/Zi" )
            
            # execute Nmake
            nmakeCommandLine = ( "\"%s\" /C /f \"%s\" _MSC_VER=1900" % \
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
