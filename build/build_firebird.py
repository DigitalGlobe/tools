#------------------------------------------------------------------------------
#
# build_firebird.py
#
# Summary : Builds the firebird library.
#
#------------------------------------------------------------------------------


import glob
import os
import sys

from BuildSettingSet import *
from PathFinder      import *
from SystemManager   import *
from FileDistributor import *
from XmlUtils import *

#------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of what the script does
        DESCRIPTION = "Builds the firebird library."
        
        _PATH_NAME_INSTALLATION_DIR_X86='..\\..\\firebird\\x86'
        _PATH_NAME_INSTALLATION_DIR_X64='..\\..\\firebird\\x64'
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "firebird"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\firebird"
        #----------------------------------------------------------------------
        
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
            xmlUtils = XmlUtils()
            
            # process command-line arguments
            buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)
            
            # initialize environment variables
            systemManager.initializeIncludeEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.initializeLibraryEnvironmentVariable( buildSettings.X64Specified() )
            
            systemManager.appendToPathEnvironmentVariable( pathFinder.getVisualStudioBinPathName( buildSettings.X64Specified() ) )
            systemManager.appendToPathEnvironmentVariable( pathFinder.getVisualStudioIDEPathName( buildSettings.X64Specified() ) )
            systemManager.appendToPathEnvironmentVariable( pathFinder.getMSBuildPathName( buildSettings.X64Specified() ) )

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
            
            
            srcInstallDir = os.path.join(buildPathName, 'output_' + ('amd64' if buildSettings.X64Specified() else 'Win32'))
            installDir = os.path.join(buildPathName, ( Program._PATH_NAME_INSTALLATION_DIR_X64 if buildSettings.X64Specified() else Program._PATH_NAME_INSTALLATION_DIR_X86))
            
            # remove build dir
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)

            #copy UriParser to the Build area
            systemManager.copyDirectory( sourcePathName, buildPathName)
        
            buildPathName   = os.path.join( buildPathName, 'builds\\win32' )

            # start building
            systemManager.changeDirectory(buildPathName)

            os.environ['FB_PROCESSOR_ARCHITECTURE'] = 'amd64' if buildSettings.X64Specified() else 'Win32'
            
            cmd = 'make_icu.bat clean ' + ( '' if ( buildSettings.ReleaseSpecified() )  else 'debug')
            print('cmd: ' + cmd)
            
            result = systemManager.execute(cmd)
            if (result != 0) :
                sys.exit(-1)
                
            cmd = 'make_boot.bat clean ' + ( '' if ( buildSettings.ReleaseSpecified() )  else 'debug')
            print('cmd: ' + cmd)
            
            result = systemManager.execute(cmd)
            if (result != 0) :
                sys.exit(-1)
                
            cmd = 'make_all.bat clean ' + ( '' if ( buildSettings.ReleaseSpecified() )  else 'debug')
            print('cmd: ' + cmd)
            
            result = systemManager.execute(cmd)
            if (result != 0) :
                sys.exit(-1)
                
            systemManager.removeDirectory(installDir)
            systemManager.distributeFiles(srcInstallDir, installDir, \
                  '*',                                                \
                  True, False, True) 

            # firebird install docs recommend changing the fbclient_ms.lib to gds32_ms.lib (the borland name?), so we'll make a copy
            systemManager.copyFile(os.path.join(installDir, 'lib//fbclient_ms.lib'), os.path.join(installDir, 'lib//gds32_ms.lib')) 

    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
