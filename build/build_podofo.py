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
from PathFinder import *
from SystemManager import *
from XmlUtils import *

class Program :
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the Podofo library."
        #----------------------------------------------------------------------
        # the name of the dynamic solution file
        _FILE_NAME_SOLUTION = "src\\podofo_shared.vcxproj"
        
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "PoDoFo"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\PoDoFo"
        #----------------------------------------------------------------------
                
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        
        _LIBNAME = 'podofo'
        _DEBUG_SUFFIX = '_d'

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
        _PATH_NAME_LIBPNG_INCLUDE = "..\\include\\LibPNG"
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

        # the name of the path for all include files
        _PATH_NAME_INCLUDE = 'src'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\podofo'
        #----------------------------------------------------------------------

        def __init__(self) :
        
            pass
        #----------------------------------------------------------------------
        

        def main(self) :
            systemManager = SystemManager()
            pathFinder    = PathFinder()
            xmlUtils = XmlUtils()
            
            # process command-line arguments
            buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)
            
            # initialize environment variables
            systemManager.initializeIncludeEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.initializeLibraryEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.appendToPathEnvironmentVariable( pathFinder.getVisualStudioBinPathName( buildSettings.X64Specified() ) )

            # MSBuild is under "Program Files (x86)"
            systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "MSBuild\\14.0\\Bin") )

            compileOutDir = ""
            if ( buildSettings.X64Specified() ) :
                # append path for 64-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x64"                 ) )
            else:
                # append path for 32-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x86"                 ) )

            # get the paths
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            sdkOutDir = buildPathName + "\\..\\" + (Program._PATH_NAME_DISTRIBUTION_X64 if buildSettings.X64Specified() else Program._PATH_NAME_DISTRIBUTION_X86)

            # remove build dir
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)

            #copy UriParser to the Build area
            systemManager.copyDirectory( sourcePathName, buildPathName)
        
            # start building
            systemManager.changeDirectory(buildPathName)
                        
            freeTypeIncludePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_FREETYPE_INCLUDE)
            libPngIncludePathName   = systemManager.getCurrentRelativePathName(Program._PATH_NAME_LIBPNG_INCLUDE  )
            zlibIncludePathName     = systemManager.getCurrentRelativePathName(Program._PATH_NAME_ZLIB_INCLUDE    )
            
            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 freeTypeLibraryFileName     = systemManager.getCurrentRelativePathName(Program._FILE_NAME_FREETYPE_LIBRARY_X64_RELEASE)
                 libPngLibraryFileName       = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBPNG_LIBRARY_X64_RELEASE  )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X64_RELEASE    )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
                 freeTypeLibraryFileName     = systemManager.getCurrentRelativePathName(Program._FILE_NAME_FREETYPE_LIBRARY_X86_RELEASE)
                 libPngLibraryFileName       = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBPNG_LIBRARY_X86_RELEASE  )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X86_RELEASE    )
                 
            elif ( buildSettings.X64Specified() ) :
                 freeTypeLibraryFileName     = systemManager.getCurrentRelativePathName(Program._FILE_NAME_FREETYPE_LIBRARY_X64_DEBUG)
                 libPngLibraryFileName       = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBPNG_LIBRARY_X64_DEBUG  )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X64_DEBUG    )
                 
            else :
                 freeTypeLibraryFileName     = systemManager.getCurrentRelativePathName(Program._FILE_NAME_FREETYPE_LIBRARY_X86_DEBUG)
                 libPngLibraryFileName       = systemManager.getCurrentRelativePathName(Program._FILE_NAME_LIBPNG_LIBRARY_X86_DEBUG  )
                 zlibLibraryFileName         = systemManager.getCurrentRelativePathName(Program._FILE_NAME_ZLIB_LIBRARY_X86_DEBUG    )
                 
            # run CMake
            if ( buildSettings.X64Specified() ) :
            
                cmakeCommandLine = ( ( "%s "                                + \
                                       "-DFREETYPE_INCLUDE_DIR=\"%s\" "     + \
                                       "-DFREETYPE_LIBRARY=\"%s\" "         + \
                                       "-DFREETYPE_LIBRARY_DEBUG=\"%s\" "   + \
                                       "-DFREETYPE_LIBRARY_RELEASE=\"%s\" " + \
                                       "-DZLIB_INCLUDE_DIR=\"%s\" "         + \
                                       "-DZLIB_LIBRARY=\"%s\" "             + \
                                       "-DZLIB_LIBRARY_DEBUG=\"%s\" "       + \
                                       "-DZLIB_LIBRARY_RELEASE=\"%s\" "     + \
                                       "-DCPPUNIT_LIBRARIES= "  + \
                                       "-DPODOFO_BUILD_SHARED:BOOL=FALSE "  + \
                                       "-G\"Visual Studio 14 2015 Win64\" " + \
                                       "\"%s\""                             ) % \
                                     ( PathFinder.FILE_NAME_CMAKE , \
                                       freeTypeIncludePathName    , \
                                       freeTypeLibraryFileName    , \
                                       freeTypeLibraryFileName    , \
                                       freeTypeLibraryFileName    , \
                                       zlibIncludePathName        , \
                                       zlibLibraryFileName        , \
                                       zlibLibraryFileName        , \
                                       zlibLibraryFileName        , \
                                       sourcePathName             ) )
                # cmakeCommandLine = ( ( "%s "                                + \
                # "-DFREETYPE_INCLUDE_DIR=\"%s\" "     + \
                # "-DFREETYPE_LIBRARY=\"%s\" "         + \
                # "-DFREETYPE_LIBRARY_DEBUG=\"%s\" "   + \
                # "-DFREETYPE_LIBRARY_RELEASE=\"%s\" " + \
                # "-DPNG_PNG_INCLUDE_DIR=\"%s\" "          + \
                # "-DPNG_INCLUDE_DIR=\"%s\" "          + \
                # "-DPNG_LIBRARY=\"%s\" "              + \
                # "-DPNG_LIBRARY_DEBUG=\"%s\" "        + \
                # "-DPNG_LIBRARY_RELEASE=\"%s\" "      + \
                # "-DZLIB_INCLUDE_DIR=\"%s\" "         + \
                # "-DZLIB_LIBRARY=\"%s\" "             + \
                # "-DZLIB_LIBRARY_DEBUG=\"%s\" "       + \
                # "-DZLIB_LIBRARY_RELEASE=\"%s\" "     + \
                # "-DCppUnit_NOT_FOUND "  + \
                # "-DPODOFO_BUILD_SHARED:BOOL=FALSE "  + \
                # "-G\"Visual Studio 14 2015 Win64\" " + \
                # "\"%s\""                             ) % \
                # ( PathFinder.FILE_NAME_CMAKE , \
                # freeTypeIncludePathName    , \
                # freeTypeLibraryFileName    , \
                # freeTypeLibraryFileName    , \
                # freeTypeLibraryFileName    , \
                # libPngIncludePathName      , \
                # libPngIncludePathName      , \
                # libPngLibraryFileName      , \
                # libPngLibraryFileName      , \
                # libPngLibraryFileName      , \
                # zlibIncludePathName        , \
                # zlibLibraryFileName        , \
                # zlibLibraryFileName        , \
                # zlibLibraryFileName        , \
                # sourcePathName             ) )
                                       
            else :
            
                cmakeCommandLine = ( ( "%s "                                + \
                                       "-DFREETYPE_INCLUDE_DIR=\"%s\" "     + \
                                       "-DFREETYPE_LIBRARY=\"%s\" "         + \
                                       "-DFREETYPE_LIBRARY_DEBUG=\"%s\" "   + \
                                       "-DFREETYPE_LIBRARY_RELEASE=\"%s\" " + \
                                       "-DZLIB_INCLUDE_DIR=\"%s\" "         + \
                                       "-DZLIB_LIBRARY=\"%s\" "             + \
                                       "-DZLIB_LIBRARY_DEBUG=\"%s\" "       + \
                                       "-DZLIB_LIBRARY_RELEASE=\"%s\" "     + \
                                       "-DCPPUNIT_LIBRARIES= "  + \
                                       "-DPODOFO_BUILD_SHARED:BOOL=FALSE "  + \
                                       "\"%s\""                             ) % \
                                     ( PathFinder.FILE_NAME_CMAKE , \
                                       freeTypeIncludePathName    , \
                                       freeTypeLibraryFileName    , \
                                       freeTypeLibraryFileName    , \
                                       freeTypeLibraryFileName    , \
                                       zlibIncludePathName        , \
                                       zlibLibraryFileName        , \
                                       zlibLibraryFileName        , \
                                       zlibLibraryFileName        , \
                                       sourcePathName             ) )
                # cmakeCommandLine = ( ( "%s "                                + \
                # "-DFREETYPE_INCLUDE_DIR=\"%s\" "     + \
                # "-DFREETYPE_LIBRARY=\"%s\" "         + \
                # "-DFREETYPE_LIBRARY_DEBUG=\"%s\" "   + \
                # "-DFREETYPE_LIBRARY_RELEASE=\"%s\" " + \
                # "-DPNG_PNG_INCLUDE_DIR=\"%s\" "          + \
                # "-DPNG_INCLUDE_DIR=\"%s\" "          + \
                # "-DPNG_LIBRARY=\"%s\" "              + \
                # "-DPNG_LIBRARY_DEBUG=\"%s\" "        + \
                # "-DPNG_LIBRARY_RELEASE=\"%s\" "      + \
                # "-DZLIB_INCLUDE_DIR=\"%s\" "         + \
                # "-DZLIB_LIBRARY=\"%s\" "             + \
                # "-DZLIB_LIBRARY_DEBUG=\"%s\" "       + \
                # "-DZLIB_LIBRARY_RELEASE=\"%s\" "     + \
                # "-DCPPUNIT_LIBRARIES= "  + \
                # "-DPODOFO_BUILD_SHARED:BOOL=FALSE "  + \
                # "\"%s\""                             ) % \
                # ( PathFinder.FILE_NAME_CMAKE , \
                # freeTypeIncludePathName    , \
                # freeTypeLibraryFileName    , \
                # freeTypeLibraryFileName    , \
                # freeTypeLibraryFileName    , \
                # libPngIncludePathName      , \
                # libPngIncludePathName      , \
                # libPngLibraryFileName      , \
                # libPngLibraryFileName      , \
                # libPngLibraryFileName      , \
                # zlibIncludePathName        , \
                # zlibLibraryFileName        , \
                # zlibLibraryFileName        , \
                # zlibLibraryFileName        , \
                                       # sourcePathName             ) )
                        
            
            print('cmake: ' + cmakeCommandLine)
            cmakeResult = systemManager.execute(cmakeCommandLine)
            
            if (cmakeResult != 0) :
                sys.exit(-1)
            
            conf = "Release" if ( buildSettings.ReleaseSpecified() ) else "Debug"
            platform  = 'x64' if buildSettings.X64Specified() else 'Win32'
            # build the solution
            solutionFileName   = os.path.join( buildPathName               , \
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( "\"%s\" "                  + \
                                     "/p:platform=%s " + \
                                     "\"%s\""                   ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified()), platform, solutionFileName )
            
            dllName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".dll"
            libName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            pdbName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"

            buildOutDir   = os.path.join( buildPathName, 'build' )
            propfile   = os.path.join( buildPathName, 'linker.props' )
            
            msBuildCommandLine += ' /p:OutDir=' + buildOutDir
            msBuildCommandLine += ' /p:TargetExtension=dll'
            msBuildCommandLine += ' /p:SolutionDir=' + buildPathName + '\\' 
            msBuildCommandLine += ' /p:TargetName=' + Program._LIBNAME + ('' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX)
            msBuildCommandLine += ' /p:Configuration=' + conf

            linkerprops = {'OutputFile':os.path.join( buildOutDir, dllName )}
            linkerprops['ImportLibrary'] = os.path.join( buildOutDir, libName )
            if buildSettings.ReleaseSpecified():
                linkerprops['DebugSymbols'] = 'false'
            else:
                linkerprops['DebugSymbols'] = 'true'
                linkerprops['DebugType'] = 'full'
                linkerprops['ProgramDatabaseFile'] = '$(OutDir)\\' + pdbName
              
            xmlUtils.buildDll(conf, platform, {}, linkerprops, propfile)

            msBuildCommandLine +=  ' /p:ForceImportBeforeCppTargets="' + propfile + '"'
            

            print('cmd: ' + msBuildCommandLine)
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
                sys.exit(-1)


            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                              os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                              '*.h',                                                                 \
                              True, False, True) 
                
            systemManager.copyFile( os.path.join( buildOutDir, libName ) , \
                                    os.path.join( sdkOutDir , libName) )
            systemManager.copyFile( os.path.join( buildOutDir, dllName ) , \
                                    os.path.join( sdkOutDir , dllName) )
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
