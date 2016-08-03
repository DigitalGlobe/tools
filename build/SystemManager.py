#------------------------------------------------------------------------------
#
# SystemManager.py
#
#------------------------------------------------------------------------------

import glob
import os
import shutil
import subprocess

#------------------------------------------------------------------------------
# The SystemManager class represents managers that manage system environments
# and calls.
class SystemManager :

    #--------------------------------------------------------------------------
    # constants 
    
        #----------------------------------------------------------------------
        # the suffix for a debug file name
        _DEBUG_SUFFIX = "_d"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the include environment variable
        _ENVIRONMENT_VARIABLE_NAME_INCLUDE = "INCLUDE"
        #----------------------------------------------------------------------
        # the name of the library environment variable
        _ENVIRONMENT_VARIABLE_NAME_LIBRARY = "LIB"
        #----------------------------------------------------------------------
        # the name of the path environment variable
        _ENVIRONMENT_VARIABLE_NAME_PATH = "PATH"
        #----------------------------------------------------------------------
        # the name of the 32-bit program-files environment variable
        _ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86 = "ProgramFiles(x86)"
        #----------------------------------------------------------------------
        # the name of the 64-bit program-files environment variable
        _ENVIRONMENT_VARIABLE_PROGRAM_FILES_X64 = "ProgramFiles"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the default value of the 64-bit include environment value
        _VALUE_INCLUDE_X64 = "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\INCLUDE;%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\ATLMFC\\INCLUDE;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\ucrt;%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\include\\um;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\shared;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\um;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\winrt";
        #----------------------------------------------------------------------
        # the default value of the 32-bit include environment value
        _VALUE_INCLUDE_X86 = "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\INCLUDE;%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\ATLMFC\\INCLUDE;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\ucrt;%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\include\\um;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\shared;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\um;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\winrt";
        #----------------------------------------------------------------------
        # the default value for the 64-bit library environment variable
        _VALUE_LIBRARY_X64 = "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\LIB\\amd64;%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\ATLMFC\\LIB\\amd64;%ProgramFiles(x86)%\\Windows Kits\\10\\lib\\10.0.10586.0\\ucrt\\x64;%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\lib\\um\\x64;%ProgramFiles(x86)%\\Windows Kits\\10\\lib\\10.0.10586.0\\um\\x64"
        #----------------------------------------------------------------------
        # the default value for the 32-bit library environment value
        _VALUE_LIBRARY_X86 = "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\LIB;%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\ATLMFC\\LIB;%ProgramFiles(x86)%\\Windows Kits\\10\\lib\\10.0.10586.0\\ucrt\\x86;%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\lib\\um\\x86;%ProgramFiles(x86)%\\Windows Kits\\10\\lib\\10.0.10586.0\\um\\x86"
        #----------------------------------------------------------------------
        # the separator between values in an environment variable
        _VALUE_SEPARATOR = ";"
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # constructors

        #----------------------------------------------------------------------
        # Constructs this manager as a default manager.
        #
        # Parameters :
        #     self : this manager
        def __init__(self) :
        
            pass
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # public methods

        #----------------------------------------------------------------------
        # Appends a specified path to the include environment variable.
        #
        # Parameters :
        #     self     : this manager
        #     pathName : the name of the path to append
        def appendToIncludeEnvironmentVariable( self     , \
                                                pathName ) :
                                                
            self._appendToIncludeEnvironmentVariable( SystemManager._ENVIRONMENT_VARIABLE_NAME_INCLUDE , \
                                                      pathName                                         )
        #----------------------------------------------------------------------
        # Appends a specified path to the path environment variable.
        #
        # Parameters :
        #     self     : this manager
        #     pathName : the name of the path to append
        def appendToPathEnvironmentVariable( self     , \
                                             pathName ) :
                                                
            self._appendToPathEnvironmentVariable( SystemManager._ENVIRONMENT_VARIABLE_NAME_PATH , \
                                                   pathName                                      )
        #----------------------------------------------------------------------
        # Changes to a specified directory.
        #
        # Parameters :
        #     self     : this manager
        #     pathName : the path name of the directory to which to change
        def changeDirectory( self     , \
                             pathName ) :
                             
            os.chdir(pathName)
        #----------------------------------------------------------------------
        # Copies a specified directory.
        #
        # Parameters :
        #     self           : this manager
        #     sourcePathName : the path name of the directory to copy
        #     targetPathname : the path name of the target directory
        def copyDirectory( self           , \
                           sourcePathName , \
                           targetPathName ) :
                             
            print( "Copying directory \"%s\" to \"%s\"." % \
                   ( sourcePathName , \
                     targetPathName )                    )
            shutil.copytree( sourcePathName , \
                             targetPathName )
        #----------------------------------------------------------------------
        # Copies a specified file.
        #
        # Parameters :
        #     self           : this manager
        #     sourceFileName : the file name of the file to copy
        #     targetFilename : the file name of the target file
        def copyFile( self           , \
                      sourceFileName , \
                      targetFileName ) :
                             
            print( "Copying file \"%s\" to \"%s\"." % \
                   ( sourceFileName , \
                     targetFileName )               )
            shutil.copyfile( sourceFileName , \
                             targetFileName )
        #----------------------------------------------------------------------
        # Distributes specified files in a specified source path into a
        # specified target path.
        #
        # Parameters :
        #    self             : this manager
        #    sourcePathName   : the name of the source path to use
        #    targetPathName   : the name of the target path to use
        #    filePattern      : the pattern (e.g., <code>*.lib</code> of the
        #                       files to distribute
        #    releaseSpecified : if <code>True</code>, this method will use
        #                       the same source file names as the distributed
        #                       file name; if <code>False</code>, this method
        #                       will use a debug version of the source file
        #                       name as the distributed file name
        #    dConsidered      : if <code>True</code> and the
        #                       <code>releaseSpecified</code> parameter is
        #                       <code>True</code>, this method will handle
        #                       <code>d</code> suffixes in debug file names
        def distributeFiles( self             , \
                             sourcePathName   , \
                             targetPathName   , \
                             filePattern      , \
                             releaseSpecified , \
                             dConsidered      ) :
                             
            sourceFileNames = glob.glob( os.path.join( sourcePathName , \
                                                       filePattern    ) );
            for sourceFileName in sourceFileNames :
            
                targetFileName = os.path.join( targetPathName                   , \
                                               os.path.basename(sourceFileName) )
                if (not releaseSpecified) : \
                    
                    targetFileName = self.getDebugFileName( targetFileName , \
                                                            dConsidered    )
                    
                print( "Copying file \"%s\" to \"%s\"." % \
                       ( sourceFileName , \
                         targetFileName )               )
                shutil.copyfile( sourceFileName , \
                                 targetFileName )
        #----------------------------------------------------------------------
        # Executes a specified command line.
        #
        # Parameters :
        #     self        : this manager
        #     commandLine : the command line to execute
        # Returns :
        #     the exit code of the execution
        def execute( self        , \
                     commandLine ) :
            
            childProcess = subprocess.Popen( commandLine , \
                                             shell=True )
            childProcess.communicate()
            childProcess.wait()
            
            return childProcess.returncode
        #----------------------------------------------------------------------
        # Gets the name of the current path.
        #
        # Parameters :
        #     self : this manager
        # Returns :
        #     the name of the current path
        def getCurrentPathName(self) :
        
            return os.path.dirname( os.path.abspath(__file__) )
        #----------------------------------------------------------------------
        # Gets the name of a specified path that is relative to the current
        # path.
        #
        # Parameters :
        #     self     : this manager
        #     pathName : the name of the relative path
        # Returns :
        #     the name of the specified path
        def getCurrentRelativePathName( self     ,
                                        pathName ) :
        
            return ( os.path.normpath( os.path.join( self.getCurrentPathName() , \
                                                     pathName                  ) ) )
        #----------------------------------------------------------------------
        # Gets the debug version of a specified file name.  For example, if
        # the specified file name is <code>libjpeg.lib</code>, this method
        # will return <code>libjpeg_d.lib</code>.
        #
        # Parameters :
        #     self        : this manager
        #     fileName    : the file name to use
        #     dConsidered : if <code>True</code>, this method should consider
        #                   a trailing <code>d</code> that might already
        #                   exist (e.g., <code>libpngd.py); if
        #                   <code>False</code>, this method should not
        #                   consider a trailing <code>d</code>
        # Returns :
        #     the debug version of the specified file name
        def getDebugFileName( self                ,
                              fileName            ,
                              dConsidered = False ) :
                              
            fileNameWithoutExtension = os.path.splitext(fileName)[0]
            extension                = os.path.splitext(fileName)[1]
            
            if ( dConsidered and \
                 ( fileNameWithoutExtension.endswith("d") or \
                   fileNameWithoutExtension.endswith("D")  ) ) :
                   
                fileNameWithoutExtension = fileNameWithoutExtension[ 0:( len(fileNameWithoutExtension) - 1 ) ]
                              
            return ( fileNameWithoutExtension    + \
                     SystemManager._DEBUG_SUFFIX + \
                     extension                   )
        #----------------------------------------------------------------------
        # Gets the value of the include environment variable.
        #
        # Parameters :
        #     self : this manager
        # Return :
        #     the value of the include environment variable
        def getIncludeEnvironmentVariableValue(self) :
        
            return self._getEnvironmentVariableName(SystemManager._ENVIRONMENT_VARIABLE_NAME_INCLUDE)
        #----------------------------------------------------------------------
        # Gets the value of the path environment variable.
        #
        # Parameters :
        #     self : this manager
        # Return :
        #     the value of the path environment variable
        def getPathEnvironmentVariableValue(self) :
        
            return self._getEnvironmentVariableName(SystemManager._ENVIRONMENT_VARIABLE_NAME_PATH)
        #----------------------------------------------------------------------
        # Gets the path name of the program-files directory.
        #
        # Parameters :
        #     self         : this manager
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        def getProgramFilesPathName( self         , \
                                     x64Specified ) :
                                     
            return ( self._getEnvironmentVariableName(SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X64) \
                     if (x64Specified)                                                                       \
                     else self._getEnvironmentVariableName(SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86) )
        #----------------------------------------------------------------------
        # Initializes the include environment variable.
        #
        # Parameters :
        #     self         : this manager
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        def initializeIncludeEnvironmentVariable( self         , \
                                                  x64Specified ) :
                                                  
            value = ( SystemManager._VALUE_INCLUDE_X64 \
                      if (x64Specified)                \
                      else SystemManager._VALUE_INCLUDE_X86)
            value = value.replace( SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86             , \
                                   os.environ[SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86] )
            value = value.replace( "%" , \
                                   ""  )
            os.environ[SystemManager._ENVIRONMENT_VARIABLE_NAME_INCLUDE] = value
        #----------------------------------------------------------------------
        # Initializes the library environment variable.
        #
        # Parameters :
        #     self         : this manager
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        def initializeLibraryEnvironmentVariable( self         , \
                                                  x64Specified ) :
                                                  
            value = ( SystemManager._VALUE_LIBRARY_X64 \
                      if (x64Specified)                \
                      else SystemManager._VALUE_LIBRARY_X86)
            value = value.replace( SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86             , \
                                   os.environ[SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86] )
            value = value.replace( "%" , \
                                   ""  )
            os.environ[SystemManager._ENVIRONMENT_VARIABLE_NAME_LIBRARY] = value
        #----------------------------------------------------------------------
        # Makes a specified directory.  This method does nothing if the
        # specified directory already exists.
        #
        # Parameters :
        #     self     : this manager
        #     pathName : the path name of the directory to make
        def makeDirectory( self     , \
                           pathName ) :
                             
            if ( not os.path.exists(pathName) ) :
            
                os.makedirs(pathName)
        #----------------------------------------------------------------------
        # Removes a specified directory.  This method does nothing if the
        # specified directory does not exist.
        #
        # Parameters :
        #     self     : this manager
        #     pathName : the path name of the directory to delete
        def removeDirectory( self     , \
                             pathName ) :
                             
            if ( os.path.exists(pathName) ) :
            
                shutil.rmtree(pathName)
        #----------------------------------------------------------------------
    
    #--------------------------------------------------------------------------
    # private methods

        #----------------------------------------------------------------------
        # Appends a specified path to a specified path environment variable.
        #
        # Parameters :
        #     self                    : this manager
        #     environmentVariableName : the name of the environment variable
        #                               to which to append
        #     pathName                : the name of the path to append
        def _appendToPathEnvironmentVariable( self                    , \
                                              environmentVariableName , \
                                              pathName                ) :
                                                
            if (environmentVariableName in os.environ) :
            
                if ( not os.environ[environmentVariableName].endswith(SystemManager._VALUE_SEPARATOR) ) :
                
                    os.environ[environmentVariableName] += SystemManager._VALUE_SEPARATOR
                    
                os.environ[environmentVariableName] += pathName
            
            else :
            
                os.environ[environmentVariableName] = pathName
        #----------------------------------------------------------------------
        # Gets the value of a specified environment variable.
        #
        # Parameters :
        #     self                    : this manager
        #     environmentVariableName : the name of the environment variable
        #                               whose value to get
        # Return :
        #     the value of the specified environment variable, or an empty
        #     string if 
        def _getEnvironmentVariableName( self                    , \
                                         environmentVariableName ) :
                                        
            if (environmentVariableName in os.environ) :
            
                value = os.environ[environmentVariableName]
            
            else :
            
                value = ""
            
            return value
        #----------------------------------------------------------------------
    
#------------------------------------------------------------------------------
