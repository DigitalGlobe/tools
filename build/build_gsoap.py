#------------------------------------------------------------------------------
#
# build_gsoap.py
#
# Summary : Builds the GSoap library.
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
        DESCRIPTION = "Builds the GSoap library."
        #----------------------------------------------------------------------
        # the name of the build debug executable SOAP file
        _FILE_NAME_EXECUTABLE_BUILD_SOAP_DEBUG = "soapcpp2.exe"
        #----------------------------------------------------------------------
        # the name of the build release executable SOAP file
        _FILE_NAME_EXECUTABLE_BUILD_SOAP_RELEASE = "soapcpp2.exe"
        #----------------------------------------------------------------------
        # the name of the distribution debug executable SOAP file
        _FILE_NAME_EXECUTABLE_DISTRIBUTION_SOAP_DEBUG = "soapcpp2_d.exe"
        #----------------------------------------------------------------------
        # the name of the distribution release executable SOAP file
        _FILE_NAME_EXECUTABLE_DISTRIBUTION_SOAP_RELEASE = "soapcpp2.exe"
        #----------------------------------------------------------------------
        # the name of the solution file
        _FILE_NAME_SOLUTION = "gsoap\\VisualStudio2005\\soapcpp2\\soapcpp2.sln"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit binary files
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit binary files
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain intermediary build files
        _PATH_NAME_BUILD = "GSoap"
        #----------------------------------------------------------------------
        # the name of the path that will contain debug 32-bit build files
        _PATH_NAME_BUILD_DEBUG_X86 = "gsoap\\VisualStudio2005\\soapcpp2\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain release 32-bit build files
        _PATH_NAME_BUILD_RELEASE_X86 = "gsoap\\VisualStudio2005\\soapcpp2\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain debug 64-bit build files
        _PATH_NAME_BUILD_DEBUG_X64 = "gsoap\\VisualStudio2005\\soapcpp2\\Debug"
        #----------------------------------------------------------------------
        # the name of the path that will contain release 64-bit build files
        _PATH_NAME_BUILD_RELEASE_X64 = "gsoap\\VisualStudio2005\\soapcpp2\\Release"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 32-bit binary files
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\bin"
        #----------------------------------------------------------------------
        # the name of the path that will contain built 64-bit binary files
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\bin"
        #----------------------------------------------------------------------
        # the name of the path that contains the source code
        _PATH_NAME_SOURCE = "..\\src\\GSoap"
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
            binaryPathName = ( systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64)      \
                               if ( buildSettings.X64Specified() )                                          \
                               else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86) )
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD )
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
            
            # determine file names
            if ( buildSettings.ReleaseSpecified() and \
                 buildSettings.X64Specified()       ) :
                 
                 buildSoapExecutableFileName        = os.path.join( buildPathName                                                                 , \
                                                                    Program._PATH_NAME_BUILD_RELEASE_X64                                          ,
                                                                    Program._FILE_NAME_EXECUTABLE_BUILD_SOAP_RELEASE                              )
                 distributionSoapExecutableFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                                    Program._FILE_NAME_EXECUTABLE_DISTRIBUTION_SOAP_RELEASE                       )
                 
            elif ( buildSettings.ReleaseSpecified() ) :
            
                 buildSoapExecutableFileName        = os.path.join( buildPathName                                                                 , \
                                                                    Program._PATH_NAME_BUILD_RELEASE_X86                                          ,
                                                                    Program._FILE_NAME_EXECUTABLE_BUILD_SOAP_RELEASE                              )
                 distributionSoapExecutableFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                                    Program._FILE_NAME_EXECUTABLE_DISTRIBUTION_SOAP_RELEASE                       )
                 
            elif ( buildSettings.X64Specified() ) :
            
                 buildSoapExecutableFileName        = os.path.join( buildPathName                                                                 , \
                                                                    Program._PATH_NAME_BUILD_DEBUG_X64                                            ,
                                                                    Program._FILE_NAME_EXECUTABLE_BUILD_SOAP_DEBUG                                )
                 distributionSoapExecutableFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X64) , \
                                                                    Program._FILE_NAME_EXECUTABLE_DISTRIBUTION_SOAP_DEBUG                         )
                 
            else :
            
                 buildSoapExecutableFileName        = os.path.join( buildPathName                                                                 , \
                                                                    Program._PATH_NAME_BUILD_DEBUG_X86                                            ,
                                                                    Program._FILE_NAME_EXECUTABLE_BUILD_SOAP_DEBUG                                )
                 distributionSoapExecutableFileName = os.path.join( systemManager.getCurrentRelativePathName(Program._PATH_NAME_DISTRIBUTION_X86) , \
                                                                    Program._FILE_NAME_EXECUTABLE_DISTRIBUTION_SOAP_DEBUG                         )
                 
            # initialize directories
            systemManager.removeDirectory(buildPathName)
            systemManager.copyDirectory( sourcePathName ,
                                         buildPathName  )
            
            # build the solution
            solutionFileName   = os.path.join( buildPathName               ,
                                               Program._FILE_NAME_SOLUTION )
            msBuildCommandLine = ( ( "\"%s\" "              + \
                                     "/p:Configuration=%s " + \
                                     "\"%s\""               ) % \
                                   ( pathFinder.getMSBuildFileName( buildSettings.X64Specified() ) , \
                                   ( "Release"                               \
                                     if ( buildSettings.ReleaseSpecified() ) \
                                     else "Debug"                          )                       , \
                                   solutionFileName                                                ) )
            msbuildResult = systemManager.execute(msBuildCommandLine)
            if (msbuildResult != 0) :
            
                sys.exit(-1)
                
            # distribute the library file
            systemManager.copyFile( buildSoapExecutableFileName        , \
                                    distributionSoapExecutableFileName )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------

#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
