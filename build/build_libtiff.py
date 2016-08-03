#------------------------------------------------------------------------------
#
# build_libtiff.py
#
# Summary : Builds the LibTIFF library.
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
        DESCRIPTION = "Builds the LibTIFF library."
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the dynamic file
        _FILE_NAME_DYNAMIC = "libtiff_i.lib"
        #----------------------------------------------------------------------
        # the name of the library file
        _FILE_NAME_LIBRARY = "libtiff.lib"
        #----------------------------------------------------------------------
        # the name of the debug file
        _FILE_NAME_DEBUG = "libtiff.pdb"
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
        _PATH_NAME_BUILD = "LibTIFF"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_LIBRARY_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_LIBRARY_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\LibTIFF"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the x86 debug LibJPEG library file
        _FILE_NAME_LIBJPEG_LIBRARY_X86_DEBUG = "..\\sdk\\x86\\lib\\libjpeg_d.lib"
        #----------------------------------------------------------------------
        # the name of the x86 release LibJPEG library file
        _FILE_NAME_LIBJPEG_LIBRARY_X86_RELEASE = "..\\sdk\\x86\\lib\\libjpeg.lib"
        #----------------------------------------------------------------------
        # the name of the x64 debug LibJPEG library file
        _FILE_NAME_LIBJPEG_LIBRARY_X64_DEBUG = "..\\sdk\\x64\\lib\\libjpeg_d.lib"
        #----------------------------------------------------------------------
        # the name of the x64 release LibJPEG library file
        _FILE_NAME_LIBJPEG_LIBRARY_X64_RELEASE = "..\\sdk\\x64\\lib\\libjpeg.lib"
        #----------------------------------------------------------------------
        # the name of the LibJPEG include path
        _PATH_NAME_LIBJPEG_INCLUDE = "..\\src\\LibJPEG"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the x86 debug ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X86_DEBUG = "..\\sdk\\x86\\lib\\zLib_d.lib"
        #----------------------------------------------------------------------
        # the name of the x86 release ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X86_RELEASE = "..\\sdk\\x86\\lib\\zLib.lib"
        #----------------------------------------------------------------------
        # the name of the x64 debug ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X64_DEBUG = "..\\sdk\\x64\\lib\\zLib_d.lib"
        #----------------------------------------------------------------------
        # the name of the x64 release ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X64_RELEASE = "..\\sdk\\x64\\lib\\zLib.lib"
        #----------------------------------------------------------------------
        # the name of the ZLib include path
        _PATH_NAME_ZLIB_INCLUDE = "..\\src\\ZLib"
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
            binaryPathName         = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64) \
                                       if ( buildSettings.X64Specified() )                                     \
                                       else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName          = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD          )
            sourcePathName         = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE         )
            zLibIncludePathName    = systemManager.getCurrentRelativePathName(Program._PATH_NAME_ZLIB_INCLUDE   )
            libJpegIncludePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBJPEG_INCLUDE)
            
            # determine file names
            buildDynamicFileName = os.path.join( buildPathName              , \
                                                 "libtiff"                  , \
                                                 Program._FILE_NAME_DYNAMIC )
            buildLibraryFileName = os.path.join( buildPathName              , \
                                                 "libtiff"                  , \
                                                 Program._FILE_NAME_LIBRARY )
            buildDebugFileName   = os.path.join( buildPathName              , \
                                                 "libtiff"                  , \
                                                 Program._FILE_NAME_DEBUG   )
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                             Program._FILE_NAME_DYNAMIC                                               )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                             Program._FILE_NAME_LIBRARY                                               )
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                             Program._FILE_NAME_DEBUG                                                 )
                 zLibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X64_RELEASE   )
                 libJpegLibraryFileName      = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBJPEG_LIBRARY_X64_RELEASE)
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                             Program._FILE_NAME_DYNAMIC                                               )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                             Program._FILE_NAME_LIBRARY                                               )
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                             Program._FILE_NAME_DEBUG                                                 )
                 zLibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X86_RELEASE   )
                 libJpegLibraryFileName      = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBJPEG_LIBRARY_X86_RELEASE)
                 
            elif ( buildSettings.X64Specified() ) :
            
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                             Program._FILE_NAME_DYNAMIC                                               )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                             Program._FILE_NAME_LIBRARY                                               )
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X64) , \
                                                             Program._FILE_NAME_DEBUG                                                 )
                 zLibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X64_DEBUG   )
                 libJpegLibraryFileName      = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBJPEG_LIBRARY_X64_DEBUG)
                 
            else :
            
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                             Program._FILE_NAME_DYNAMIC                                               )
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                             Program._FILE_NAME_LIBRARY                                               )
                 distributionDebugFileName   = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBRARY_X86) , \
                                                             Program._FILE_NAME_DEBUG                                                 )
                 zLibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X86_DEBUG   )
                 libJpegLibraryFileName      = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBJPEG_LIBRARY_X86_DEBUG)
            
            # initialize LibJPEG environment variables
            os.environ["JPEG_SUPPORT"] = "1"
            os.environ["JPEG_INCLUDE"] = ( "-I\""                 + \
                                           libJpegIncludePathName + \
                                           "\""                   )
            os.environ["JPEG_LIB"    ] = libJpegLibraryFileName
            if ( not os.path.exists( os.path.join( zLibIncludePathName , \
                                                   "zconf.h"           ) ) ) :
                                                   
                systemManager.copyFile( os.path.join( zLibIncludePathName , \
                                                      "zconf.h.in"        ) ,
                                        os.path.join( zLibIncludePathName , \
                                                      "zconf.h"           ) )
            
            # initialize ZLib environment variables
            os.environ["ZIP_SUPPORT" ] = "1"
            os.environ["ZLIB_INCLUDE"] = ( "-I\""              + \
                                           zLibIncludePathName + \
                                           "\""                )
            os.environ["ZLIB_LIB"    ] = zLibLibraryFileName

            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName , \
                                         buildPathName  )
                               
            # execute Nmake
            nmakeCommandLine = ( "\"%s\" /C /f \"%s\" _MSC_VER=1900" % \
                                 ( pathFinder.getNmakeFileName( buildSettings.X64Specified() ) , \
                                   Program._FILE_NAME_MAKEFILE                                 ) )
            if ( buildSettings.ReleaseSpecified() ) :
            
                 nmakeCommandLine += " nodebug=1"
            
            systemManager.changeDirectory(buildPathName)
            nmakeResult = systemManager.execute(nmakeCommandLine)
   
            # copy the library, debug, and binary files, if Nmake executed successfully
            if (nmakeResult == 0) :
            
                # copy the binary files
                buildBinaryFileNames = glob.glob( os.path.join( buildPathName                , \
                                                                "tools"                      , \
                                                                Program._FILE_PATTERN_BINARY ) );
                for buildBinaryFileName in buildBinaryFileNames :
                
                    binaryFileName = os.path.join( binaryPathName , \
                                                   ( os.path.basename(buildBinaryFileName) \
                                                     if ( buildSettings.ReleaseSpecified() ) \
                                                     else systemManager.getDebugFileName( os.path.basename(buildBinaryFileName) ) ) )
                    systemManager.copyFile( buildBinaryFileName , \
                                            binaryFileName      )
                
                # copy the debug file
                systemManager.copyFile( buildDebugFileName        , \
                                        distributionDebugFileName )

                # copy the library files
                systemManager.copyFile( buildDynamicFileName        , \
                                        distributionDynamicFileName )
                systemManager.copyFile( buildLibraryFileName        , \
                                        distributionLibraryFileName )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    
#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
