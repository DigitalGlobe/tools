#------------------------------------------------------------------------------
#
# SystemManager.py
#
#------------------------------------------------------------------------------

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
        # the name of the path environment variable
        _ENVIRONMENT_VARIABLE_NAME_PATH = "PATH"
        #----------------------------------------------------------------------

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
                                                
            print(self._getEnvironmentVariableName(SystemManager._ENVIRONMENT_VARIABLE_NAME_INCLUDE))
            self._appendToPathEnvironmentVariable( SystemManager._ENVIRONMENT_VARIABLE_NAME_INCLUDE , \
                                                   pathName                                         )
            print(self._getEnvironmentVariableName(SystemManager._ENVIRONMENT_VARIABLE_NAME_INCLUDE))
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
                                             shell=True  )
            childProcess.communicate()
            
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
        #     self     : this manager
        #     fileName : the file name to use
        # Returns :
        #     the debug version of the specified file name
        def getDebugFileName( self     ,
                              fileName ) :
                              
            return ( os.path.splitext(fileName)[0] +
                     SystemManager._DEBUG_SUFFIX   +
                     os.path.splitext(fileName)[1] )
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
            
                os.environ[pathName] = pathName
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
