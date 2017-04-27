#------------------------------------------------------------------------------
#
# build_libtiff.py
#
# Summary : Builds the LibTIFF library.
#
#------------------------------------------------------------------------------

import glob
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
        DESCRIPTION = "Builds the libtiff library."
        #----------------------------------------------------------------------
        
        _LIBNAME = 'libtiff'
        _LIBNAME_STATIC = 'libtiff-static'
        _DEBUG_SUFFIX = '_d'
        
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "libTIFF"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\libTIFF"
        #----------------------------------------------------------------------

        #----------------------------------------------------------------------
        # the pattern for binary files
        _FILE_PATTERN_BINARY = "*.exe"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit binary files
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit binary files
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        
        # the name of the path that will contain built 32-bit library files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit library files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = 'libtiff'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\libtiff'
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        _JPEG_INCLUDE = "..\\..\\include\\LibJPEG"
        _JPEG_LIB_BASE = "libjpeg"
        #----------------------------------------------------------------------
        _ZLIB_INCLUDE = "..\\..\\include\\zlib"
        _ZLIB_LIB_BASE = "zlib"

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
                # append path for 64-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x64"                 ) )
            else:
                # append path for 32-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x86"                 ) )

            # determine path names
            binaryPathName = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64) \
                               if ( buildSettings.X64Specified() )                                     \
                               else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            sdkOutDir = buildPathName + "\\..\\" + (Program._PATH_NAME_DISTRIBUTION_X64 if buildSettings.X64Specified() else Program._PATH_NAME_DISTRIBUTION_X86)

            # initialize directories
            systemManager.changeDirectory(sourcePathName)
            
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName, buildPathName  )
            
            systemManager.changeDirectory(buildPathName)

            vcvars = '"' + systemManager.getVisualStudioPath() + '"\\vcvarsall.bat ' + ("amd64"             \
                                                                                           if ( buildSettings.X64Specified() ) \
                                                                                        else "x86"                          )
                                                                                                                                                                                      
            dllName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".dll"
            libName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            staticLibName = Program._LIBNAME_STATIC + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            pdbName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"
            
            os.environ['LIBTIFF'] = staticLibName
            os.environ['LIBIMPL'] = libName
            os.environ['DLLNAME'] = dllName
            
            # initialize LibJPEG environment variables
            #os.environ["JPEG_SUPPORT"] = '1'
            #os.environ["JPEG_INCLUDE"] = '-I' + os.path.join( buildPathName, Program._JPEG_INCLUDE)
            #os.environ["JPEG_LIB"    ] = os.path.join( sdkOutDir, Program._JPEG_LIB_BASE + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib")
            
            #os.environ["ZIP_SUPPORT"] = '1'
            #os.environ["ZLIB_INCLUDE"] = '-I' + os.path.join( buildPathName, Program._ZLIB_INCLUDE)
            #os.environ["ZLIB_LIB"    ] = os.path.join( sdkOutDir, Program._ZLIB_LIB_BASE + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib")
            
            # run Nmake
            nmakeCommandLine = 'nmake -f Makefile.vc _MSC_VER=1900'
            if ( buildSettings.X64Specified() ) :  
                pass
            else :
                pass
                
                
            if ( buildSettings.ReleaseSpecified() ) :
                nmakeCommandLine += " nodebug=1"
                nmakeCommandLine += ' LINKOPT="/DEBUG:NONE /dll" LIBOPT="" OPTFLAGS="-Ox -MD -EHsc -W3 -D_CRT_SECURE_NO_DEPRECATE"'
            else :
                nmakeCommandLine += ' LINKOPT="/DEBUG:FULL /dll /pdb:' + pdbName + '" LIBOPT="/debug" OPTFLAGS="-Zi -MDd"'

            nmakeCommandLine += ' LIBTIFF=' + staticLibName
            nmakeCommandLine += ' LIBIMPL=' + libName
            nmakeCommandLine += ' DLLNAME=' + dllName
            
            # initialize LibJPEG environment variables
            nmakeCommandLine += ' JPEG_SUPPORT=1'
            nmakeCommandLine += ' JPEG_INCLUDE=-I' + os.path.join( buildPathName, Program._JPEG_INCLUDE)
            nmakeCommandLine += ' JPEG_LIB=' + os.path.join( sdkOutDir, Program._JPEG_LIB_BASE + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib")
            
            nmakeCommandLine += ' ZIP_SUPPORT=1'
            nmakeCommandLine += ' ZLIB_INCLUDE=-I' + os.path.join( buildPathName, Program._ZLIB_INCLUDE)
            nmakeCommandLine += ' ZLIB_LIB=' + os.path.join( sdkOutDir, Program._ZLIB_LIB_BASE + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib")
                
            print('cmd: ' + nmakeCommandLine)

            nmakeResult = systemManager.execute(vcvars + " && " + nmakeCommandLine)
            if nmakeResult != 0:
                sys.exit(-1)
             
            buildOutDir   = os.path.join( buildPathName, 'libtiff' )
             
            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                                          os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                                          '*.h',                                                                 \
                                          True, False) 
                
            systemManager.copyFile( os.path.join( buildOutDir, libName ) , \
                                    os.path.join( sdkOutDir , libName) )
            systemManager.copyFile( os.path.join( buildOutDir, dllName ) , \
                                    os.path.join( sdkOutDir , dllName) )
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
