#------------------------------------------------------------------------------
#
# build_podofo.py
#
# Summary : Builds the PoDoFo library.
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
        DESCRIPTION = "Builds the PoDoFo library."
        #----------------------------------------------------------------------
        # the name of the build debug dynamic file
        _FILE_NAME_DYNAMIC_BUILD_DEBUG = "podofo.dll"
        #----------------------------------------------------------------------
        # the name of the build release dynamic file
        _FILE_NAME_DYNAMIC_BUILD_RELEASE = "podofo.dll"
        #----------------------------------------------------------------------
        # the name of the distribution debug dynamic file
        _FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG = "podofo_d.dll"
        #----------------------------------------------------------------------
        # the name of the distribution release dynamic file
        _FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE = "podofo.dll"
        #----------------------------------------------------------------------
        # the name of the build debug library file
        _FILE_NAME_LIBRARY_BUILD_DEBUG = "podofo.lib"
        #----------------------------------------------------------------------
        # the name of the build release library file
        _FILE_NAME_LIBRARY_BUILD_RELEASE = "podofo.lib"
        #----------------------------------------------------------------------
        # the name of the distribution debug library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG = "podofo_d.lib"
        #----------------------------------------------------------------------
        # the name of the distribution release library file
        _FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE = "podofo.lib"
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "PoDoFo.sln"
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
        _PATH_NAME_BUILD = "PoDoFo"
        #----------------------------------------------------------------------
        # the name of the path that will contain debug build files
        _PATH_NAME_BUILD_DEBUG = "src\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain release build files
        _PATH_NAME_BUILD_RELEASE = "src\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\PoDoFo"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the x86 debug FreeType library file
        _FILE_NAME_FREETYPE_LIBRARY_X86_DEBUG = "..\\sdk\\x86\\lib\\freeType_d.lib"
        #----------------------------------------------------------------------
        # the name of the x86 release FreeType library file
        _FILE_NAME_FREETYPE_LIBRARY_X86_RELEASE = "..\\sdk\\x86\\lib\\freeType.lib"
        #----------------------------------------------------------------------
        # the name of the x64 debug FreeType library file
        _FILE_NAME_FREETYPE_LIBRARY_X64_DEBUG = "..\\sdk\\x64\\lib\\freeType_d.lib"
        #----------------------------------------------------------------------
        # the name of the x64 release FreeType library file
        _FILE_NAME_FREETYPE_LIBRARY_X64_RELEASE = "..\\sdk\\x64\\lib\\freeType.lib"
        #----------------------------------------------------------------------
        # the name of the FreeType include path
        _PATH_NAME_FREETYPE_INCLUDE = "..\\src\\FreeType\\include"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the x86 debug LibPNG library file
        _FILE_NAME_LIBPNG_LIBRARY_X86_DEBUG = "..\\sdk\\x86\\lib\\libpng_d.lib"
        #----------------------------------------------------------------------
        # the name of the x86 release LibPNG library file
        _FILE_NAME_LIBPNG_LIBRARY_X86_RELEASE = "..\\sdk\\x86\\lib\\libpng.lib"
        #----------------------------------------------------------------------
        # the name of the x64 debug LibPNG library file
        _FILE_NAME_LIBPNG_LIBRARY_X64_DEBUG = "..\\sdk\\x64\\lib\\libpng_d.lib"
        #----------------------------------------------------------------------
        # the name of the x64 release LibPNG library file
        _FILE_NAME_LIBPNG_LIBRARY_X64_RELEASE = "..\\sdk\\x64\\lib\\libpng.lib"
        #----------------------------------------------------------------------
        # the name of the LibPNG include path
        _PATH_NAME_LIBPNG_INCLUDE = "..\\src\\LibPNG"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the x86 debug ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X86_DEBUG = "..\\sdk\\x86\\lib\\zlib_d.lib"
        #----------------------------------------------------------------------
        # the name of the x86 release ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X86_RELEASE = "..\\sdk\\x86\\lib\\zlib.lib"
        #----------------------------------------------------------------------
        # the name of the x64 debug ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X64_DEBUG = "..\\sdk\\x64\\lib\\zlib_d.lib"
        #----------------------------------------------------------------------
        # the name of the x64 release ZLib library file
        _FILE_NAME_ZLIB_LIBRARY_X64_RELEASE = "..\\sdk\\x64\\lib\\zlib.lib"
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
            
            # determine path names
            binaryPathName          = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64) \
                                        if ( buildSettings.X64Specified() )                                     \
                                        else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName           = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD           )
            sourcePathName          = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE          )
            freeTypeIncludePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_FREETYPE_INCLUDE)
            libPngIncludePathName   = systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBPNG_INCLUDE  )
            zlibIncludePathName     = systemManager.getCurrentRelativePathName(Program._PATH_NAME_ZLIB_INCLUDE    )
            
            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_DYNAMIC_BUILD_RELEASE                                      ) 
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE                               )
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 freeTypeLibraryFileName     = systemManager.getCurrentRelativePathName(Program._FILE_NAME_FREETYPE_LIBRARY_X64_RELEASE)
                 libPngLibraryFileName       = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBPNG_LIBRARY_X64_RELEASE  )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X64_RELEASE    )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_DYNAMIC_BUILD_RELEASE                                      ) 
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_RELEASE                               )
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_RELEASE                                              ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_RELEASE                                      ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_RELEASE                               )
                 freeTypeLibraryFileName     = systemManager.getCurrentRelativePathName(Program._FILE_NAME_FREETYPE_LIBRARY_X86_RELEASE)
                 libPngLibraryFileName       = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBPNG_LIBRARY_X86_RELEASE  )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X86_RELEASE    )
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_DYNAMIC_BUILD_DEBUG                                        ) 
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG                                 )
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 freeTypeLibraryFileName     = systemManager.getCurrentRelativePathName(Program._FILE_NAME_FREETYPE_LIBRARY_X64_DEBUG)
                 libPngLibraryFileName       = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBPNG_LIBRARY_X64_DEBUG  )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X64_DEBUG    )
                 
            else :
            
                 buildDynamicFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_DYNAMIC_BUILD_DEBUG                                        ) 
                 distributionDynamicFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_DYNAMIC_DISTRIBUTION_DEBUG                                 )
                 buildLibraryFileName        = os.path.join( buildPathName                                                                 , \
                                                             Program._PATH_NAME_BUILD_DEBUG                                                ,
                                                             Program._FILE_NAME_LIBRARY_BUILD_DEBUG                                        ) 
                 distributionLibraryFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                             Program._FILE_NAME_LIBRARY_DISTRIBUTION_DEBUG                                 )
                 freeTypeLibraryFileName     = systemManager.getCurrentRelativePathName(Program._FILE_NAME_FREETYPE_LIBRARY_X86_DEBUG)
                 libPngLibraryFileName       = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBPNG_LIBRARY_X86_DEBUG  )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X86_DEBUG    )
                 
            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.makeDirectory(buildPathName)
            
            # run CMake
            if ( buildSettings.X64Specified() ) :
            
                cmakeCommandLine = ( ( "%s "                                + \
                                       "-DFREETYPE_INCLUDE_DIR=\"%s\" "     + \
                                       "-DFREETYPE_LIBRARY=\"%s\" "         + \
                                       "-DFREETYPE_LIBRARY_DEBUG=\"%s\" "   + \
                                       "-DFREETYPE_LIBRARY_RELEASE=\"%s\" " + \
                                       "-DPNG_INCLUDE_DIR=\"%s\" "          + \
                                       "-DPNG_LIBRARY=\"%s\" "              + \
                                       "-DPNG_LIBRARY_DEBUG=\"%s\" "        + \
                                       "-DPNG_LIBRARY_RELEASE=\"%s\" "      + \
                                       "-DZLIB_INCLUDE_DIR=\"%s\" "         + \
                                       "-DZLIB_LIBRARY=\"%s\" "             + \
                                       "-DZLIB_LIBRARY_DEBUG=\"%s\" "       + \
                                       "-DZLIB_LIBRARY_RELEASE=\"%s\" "     + \
                                       "-DPODOFO_BUILD_SHARED:BOOL=FALSE "  + \
                                       "-G\"Visual Studio 14 2015 Win64\" " + \
                                       "\"%s\""                             ) % \
                                     ( PathFinder.FILE_NAME_CMAKE , \
                                       freeTypeIncludePathName    , \
                                       freeTypeLibraryFileName    , \
                                       freeTypeLibraryFileName    , \
                                       freeTypeLibraryFileName    , \
                                       libPngIncludePathName      , \
                                       libPngLibraryFileName      , \
                                       libPngLibraryFileName      , \
                                       libPngLibraryFileName      , \
                                       zlibIncludePathName        , \
                                       zlibLibraryFileName        , \
                                       zlibLibraryFileName        , \
                                       zlibLibraryFileName        , \
                                       sourcePathName             ) )
                                       
            else :
            
                cmakeCommandLine = ( ( "%s "                                + \
                                       "-DFREETYPE_INCLUDE_DIR=\"%s\" "     + \
                                       "-DFREETYPE_LIBRARY=\"%s\" "         + \
                                       "-DFREETYPE_LIBRARY_DEBUG=\"%s\" "   + \
                                       "-DFREETYPE_LIBRARY_RELEASE=\"%s\" " + \
                                       "-DPNG_INCLUDE_DIR=\"%s\" "          + \
                                       "-DPNG_LIBRARY=\"%s\" "              + \
                                       "-DPNG_LIBRARY_DEBUG=\"%s\" "        + \
                                       "-DPNG_LIBRARY_RELEASE=\"%s\" "      + \
                                       "-DZLIB_INCLUDE_DIR=\"%s\" "         + \
                                       "-DZLIB_LIBRARY=\"%s\" "             + \
                                       "-DZLIB_LIBRARY_DEBUG=\"%s\" "       + \
                                       "-DZLIB_LIBRARY_RELEASE=\"%s\" "     + \
                                       "-DPODOFO_BUILD_SHARED:BOOL=FALSE "  + \
                                       "\"%s\""                             ) % \
                                     ( PathFinder.FILE_NAME_CMAKE , \
                                       freeTypeIncludePathName    , \
                                       freeTypeLibraryFileName    , \
                                       freeTypeLibraryFileName    , \
                                       freeTypeLibraryFileName    , \
                                       libPngIncludePathName      , \
                                       libPngLibraryFileName      , \
                                       libPngLibraryFileName      , \
                                       libPngLibraryFileName      , \
                                       zlibIncludePathName        , \
                                       zlibLibraryFileName        , \
                                       zlibLibraryFileName        , \
                                       zlibLibraryFileName        , \
                                       sourcePathName             ) )
                                       
                                       
            systemManager.changeDirectory(buildPathName)
            cmakeResult = systemManager.execute(cmakeCommandLine)
            if (cmakeResult != 0) :
            
                sys.exit(-1)

            # build the solution
            msBuildCommandLine = ( ( "\"%s\" "              + \
                                     "/p:Configuration=%s " + \
                                     "/t:podofo_shared "    + \
                                     "\"%s\""               ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                   ( "Release"                               \
                                     if ( buildSettings.ReleaseSpecified() ) \
                                     else "Debug"                          )                       , \
                                   Program._FILE_NAME_SOLUTION                                     ) )
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute the library file
            systemManager.copyFile( buildDynamicFileName        , \
                                    distributionDynamicFileName )
            systemManager.copyFile( buildLibraryFileName        , \
                                    distributionLibraryFileName )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
