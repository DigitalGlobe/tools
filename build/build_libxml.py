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
from PathFinder import *
from SystemManager import *
from XmlUtils import *

class Program :
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the libxml library."
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "libxml"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\libxml"
        #----------------------------------------------------------------------
                
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
       
        _LIBNAME = 'libxml2'
        _DEBUG_SUFFIX = '_d'

        _ICONV_INCLUDE = "..\\..\\include\\libiconv\\"
        _ICONV_LIBRARY_BASE = "libiconv"
        
        _ZLIB_INCLUDE = "..\\..\\include\\zlib\\"
        _ZLIB_LIBRARY_BASE = "zlib"
        
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = 'src'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\szip'
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

            
            iconvLibraryFileName = Program._ICONV_LIBRARY_BASE + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            zlibLibraryFileName = Program._ZLIB_LIBRARY_BASE + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"

            # remove build dir
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)

            systemManager.copyDirectory( sourcePathName, buildPathName)
        
            # start building
            systemManager.changeDirectory(os.path.join(buildPathName, 'win32'))
            
            scriptCommandLine = ( "cscript.exe configure.js vcmanifest=yes zlib=yes compiler=msvc prefix=\"%s\" lib=\"%s\" cruntime=%s debug=%s") % \
                                  ( buildPathName, sdkOutDir, \
                                    ( "/MD" if  buildSettings.ReleaseSpecified() else "/MDd"),  \
                                    ( "no" if  buildSettings.ReleaseSpecified()  else "yes") ) 

            print ('cmd: ' + scriptCommandLine)
            scriptResult      = systemManager.execute(scriptCommandLine)
            
            if (scriptResult != 0) :
                sys.exit(-1)
                            
            # run Nmake
            nmakeCommandLine = 'nmake -f Makefile'
            if ( buildSettings.X64Specified() ) :
            
                nmakeCommandLine += ' MACHINE="/MACHINE:X64\"'
            else :
            
                nmakeCommandLine += ' MACHINE="/MACHINE:X86"'

            if ( buildSettings.ReleaseSpecified() ) :
                nmakeCommandLine += 'LINKOPT=/DEBUG:NONE'
            else :
                nmakeCommandLine += 'LINKOPT=/DEBUG:FULL OPTFLAGS=/Zi'
                nmakeCommandLine += ' XML_SUFFIX=' + Program._DEBUG_SUFFIX

   
            nmakeCommandLine += ' ZLIB_INC=' + os.path.join(buildPathName, Program._ZLIB_INCLUDE)
            nmakeCommandLine += ' ZLIB_LIB=' + zlibLibraryFileName 
            nmakeCommandLine += ' ICONV_INC=' + os.path.join(buildPathName, Program._ICONV_INCLUDE) 
            nmakeCommandLine += ' ICONV_LIB=' + iconvLibraryFileName 
          
            print ('cmd: ' + nmakeCommandLine)
            nmakeResult = systemManager.execute(nmakeCommandLine)
            
            if (nmakeResult != 0) :
                sys.exit(-1)
                
                
            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                              os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                              '*.h',                                                                 \
                              True, False) 

            buildOutDir   = os.path.join( buildPathName, 'win32\\bin.msvc' )

            dllName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".dll"
            libName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
            pdbName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"
            
            systemManager.copyFile( os.path.join( buildOutDir, libName ) , \
                                    os.path.join( sdkOutDir , libName) )
            systemManager.copyFile( os.path.join( buildOutDir, dllName ) , \
                                    os.path.join( sdkOutDir , dllName) )
            if not buildSettings.ReleaseSpecified():
                systemManager.copyFile( os.path.join( buildOutDir, pdbName ) , \
                                        os.path.join( sdkOutDir , pdbName) )
            
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
