#------------------------------------------------------------------------------
#
# SystemManager.py
#
#------------------------------------------------------------------------------

import glob
import os
import shutil
import subprocess
import time


class CTError(Exception):
    def __init__(self, errors):
        self.errors = errors
try:
    O_BINARY = os.O_BINARY
except:
    O_BINARY = 0
    
READ_FLAGS = os.O_RDONLY | O_BINARY
WRITE_FLAGS = os.O_WRONLY | os.O_CREAT | os.O_TRUNC | O_BINARY
BUFFER_SIZE = 128*1024

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
        # the name of the msbuild path, local to the visual studio version being used
        _ENVIRONMENT_VARIABLE_MSBUILD = "MSBuild\\14.0\\Bin"
        
        #----------------------------------------------------------------------
        # the name of the 32-bit program-files environment variable
        _ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86 = "ProgramFiles(x86)"
        #----------------------------------------------------------------------
        # the name of the 64-bit program-files environment variable
        _ENVIRONMENT_VARIABLE_PROGRAM_FILES_X64 = "ProgramFiles"
        #----------------------------------------------------------------------
        # the name of the 32-bit sdk include base
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X86 = "ProgramFiles(x86)\\Windows Kits\\10\\include\\10.0.10586.0"
        # _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X86 = "ProgramFiles(x86)\\Windows Kits\\7.1\\include"
        #----------------------------------------------------------------------
        # the name of the 32-bit sdk library base
        _ENVIRONMENT_VARIABLE_SDK_LIB_X86 = "ProgramFiles(x86)\\Windows Kits\\10\\lib\\10.0.10586.0"
        # _ENVIRONMENT_VARIABLE_SDK_LIB_X86 = "ProgramFiles(x86)\\Windows Kits\\7.1\\lib"
        #----------------------------------------------------------------------
        # the name of the 64-bit sdk include base
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X64 = "ProgramFiles(x86)\\Windows Kits\\10\\include\\10.0.10586.0"
        # _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X64 = "ProgramFiles(x86)\\Windows Kits\\7.1\\include"
        #----------------------------------------------------------------------
        # the name of the 32-bit sdk library base
        _ENVIRONMENT_VARIABLE_SDK_LIB_X64 = "ProgramFiles(x86)\\Windows Kits\\10\\lib\\10.0.10586.0"
        # _ENVIRONMENT_VARIABLE_SDK_LIB_X64 = "ProgramFiles(x86)\\Windows Kits\\7.1\\lib\\x64"
        #----------------------------------------------------------------------
        # the name of the 64-bit sdk library base
        #  # the name of the 64-bit sdk library base
        _ENVIRONMENT_VARIABLE_VISUAL_STUDIO = "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC"
        # _ENVIRONMENT_VARIABLE_VISUAL_STUDIO = "%ProgramFiles(x86)%\\Microsoft Visual Studio 10.0\\VC"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the default value of the 64-bit include environment value
        # _VALUE_INCLUDE_X64 = "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\INCLUDE;%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\ATLMFC\\INCLUDE;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\ucrt;%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\include\\um;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\shared;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\um;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\winrt";
        _VALUE_INCLUDE_X64 = _ENVIRONMENT_VARIABLE_VISUAL_STUDIO + "\\INCLUDE;" + \
        _ENVIRONMENT_VARIABLE_VISUAL_STUDIO + "\\ATLMFC\\INCLUDE;" + \
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X64 + "\\ucrt;" + \
        "%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\include\\um;" + \
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X64 + "\\shared;" + \
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X64 + "\\um;" + \
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X64 + "\\winrt"
        #----------------------------------------------------------------------
        # the default value of the 32-bit include environment value
        # _VALUE_INCLUDE_X86 = "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\INCLUDE;%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\ATLMFC\\INCLUDE;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\ucrt;%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\include\\um;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\shared;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\um;%ProgramFiles(x86)%\\Windows Kits\\10\\include\\10.0.10586.0\\winrt";
        _VALUE_INCLUDE_X86 = _ENVIRONMENT_VARIABLE_VISUAL_STUDIO + "\\INCLUDE;" + \
        _ENVIRONMENT_VARIABLE_VISUAL_STUDIO + "\\ATLMFC\\INCLUDE;" + \
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X86 + "\\ucrt;" + \
        "%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\include\\um;" + \
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X86 + "\\shared;" + \
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X86 + "\\um;" + \
        _ENVIRONMENT_VARIABLE_SDK_INCLUDE_X86 + "\\winrt"
        #----------------------------------------------------------------------
        # the default value for the 64-bit library environment variable
        # _VALUE_LIBRARY_X64 = "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\LIB\\amd64;%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\ATLMFC\\LIB\\amd64;%ProgramFiles(x86)%\\Windows Kits\\10\\lib\\10.0.10586.0\\ucrt\\x64;%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\lib\\um\\x64;%ProgramFiles(x86)%\\Windows Kits\\10\\lib\\10.0.10586.0\\um\\x64"
        _VALUE_LIBRARY_X64 = _ENVIRONMENT_VARIABLE_VISUAL_STUDIO + "\\LIB\\amd64;" + \
        _ENVIRONMENT_VARIABLE_VISUAL_STUDIO + "\\ATLMFC\\LIB\\amd64;" + \
        _ENVIRONMENT_VARIABLE_SDK_LIB_X64 + "\\ucrt\\x64;" + \
        "%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\lib\\um\\amd64;" + \
        _ENVIRONMENT_VARIABLE_SDK_LIB_X64 + "\\um\\x64"
        
        #----------------------------------------------------------------------
        # the default value for the 32-bit library environment value
        # _VALUE_LIBRARY_X86 = "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\LIB;%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\ATLMFC\\LIB;%ProgramFiles(x86)%\\Windows Kits\\10\\lib\\10.0.10586.0\\ucrt\\x86;%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\lib\\um\\x86;%ProgramFiles(x86)%\\Windows Kits\\10\\lib\\10.0.10586.0\\um\\x86"
        _VALUE_LIBRARY_X86 = _ENVIRONMENT_VARIABLE_VISUAL_STUDIO + "\\LIB;" + \
        _ENVIRONMENT_VARIABLE_VISUAL_STUDIO + "\\ATLMFC\\LIB;" + \
        _ENVIRONMENT_VARIABLE_SDK_LIB_X86 + "\\ucrt\\x86;" + \
        "%ProgramFiles(x86)%\\Windows Kits\\NETFXSDK\\4.6.1\\lib\\um\\x86;" + \
        _ENVIRONMENT_VARIABLE_SDK_LIB_X86 + "\\um\\x86"

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
                                             pathName, prepend = False ) :
                                                
            self._appendToPathEnvironmentVariable( SystemManager._ENVIRONMENT_VARIABLE_NAME_PATH , \
                                                   pathName, prepend                                      )
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
            self._copytree( sourcePathName,  targetPathName )
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
            self._copyfile( sourceFileName , targetFileName )
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
                             dConsidered      , \
                             hierarchical = False, \
                             flattenhierarchy = False) :
                        
            sourceFileNames = glob.glob( os.path.join( sourcePathName , \
                                                       filePattern    ) );
            first = True
              
            for sourceFileName in sourceFileNames :
                if os.path.isfile(sourceFileName):
                    targetFileName = os.path.join( targetPathName                   , \
                                                   os.path.basename(sourceFileName) )
                    if (not releaseSpecified) : \
                        
                        targetFileName = self.getDebugFileName( targetFileName , \
                                                                dConsidered    )
                        
                    print( "Copying file \"%s\" to \"%s\"." % \
                           ( sourceFileName , \
                             targetFileName )               )
                    if first:
                        first = False
                        self.makeDirectory(targetPathName)

                    self._copyfile( sourceFileName , \
                                     targetFileName )
                                 
            if hierarchical:
                for f in os.listdir(sourcePathName):
                    if os.path.isdir(os.path.join(sourcePathName, f)):
                        tpn = os.path.join(targetPathName, ('' if flattenhierarchy else f))
                        spn = os.path.join(sourcePathName, f)
                        self.distributeFiles(spn, tpn, filePattern, releaseSpecified, dConsidered, hierarchical, flattenhierarchy)
                        
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
        # Gets the value the location of Visual Studio.
        #
        # Parameters :
        #     self : this manager
        # Return :
        #     the value of the visual studio variable
        def getVisualStudioPath(self) :
            value = SystemManager._ENVIRONMENT_VARIABLE_VISUAL_STUDIO
            value = value.replace( SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86             , \
                                   os.environ[SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86] )
            value = value.replace( "%" , \
                                   ""  )

            return value

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
        # Gets the value of the msbuild environment variable.
        #
        # Parameters :
        #     self : this manager
        # Return :
        #     the value of the path environment variable
        def getPathEnvironmentVariableValue(self) :
            return self._getEnvironmentVariableName(SystemManager._ENVIRONMENT_VARIABLE_MSBUILD)
            
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
        #     additionalPath: optional additional include path to add at the  start of the includes
        def initializeIncludeEnvironmentVariable( self         , \
                                                  x64Specified, \
                                                  additionalPath = None) :
                                                  
            value = ( SystemManager._VALUE_INCLUDE_X64 \
                      if (x64Specified)                \
                      else SystemManager._VALUE_INCLUDE_X86)
            if additionalPath:
                value = additionalPath + ';' + value
                
            value = value.replace( SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86             , \
                                   os.environ[SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86] )
            value = value.replace( "%" , \
                                   ""  )
                                   
            print("include path: " + value)
            os.environ[SystemManager._ENVIRONMENT_VARIABLE_NAME_INCLUDE] = value
        #----------------------------------------------------------------------
        # Initializes the library environment variable.
        #
        # Parameters :
        #     self         : this manager
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        #     additionalPath: optional additional include path to add at the  start of the includes
        def initializeLibraryEnvironmentVariable( self         , \
                                                  x64Specified, \
                                                  additionalPath = None) :
                                                  
            value = ( SystemManager._VALUE_LIBRARY_X64 \
                      if (x64Specified)                \
                      else SystemManager._VALUE_LIBRARY_X86)
                      
            if additionalPath:
                value = value + ";" + additionalPath

            value = value.replace( SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86             , \
                                   os.environ[SystemManager._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86] )
            value = value.replace( "%" , \
                                   ""  )
                                   
            print("library path: " + value)

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
        # Removes a specified file.  This method does nothing if the specified
        # file does not exist.
        #
        # Parameters :
        #     self     : this manager
        #     fileName : the file name of the file to delete
        def removeFile( self     , \
                        fileName ) :
                             
            if ( os.path.exists(fileName) ) :
            
                os.remove(fileName)
        #----------------------------------------------------------------------
        # Removes a specified directory.  This method does nothing if the
        # specified directory does not exist.
        #
        # Parameters :
        #     self     : this manager
        #     pathName : the path name of the directory to delete
        def removeDirectory( self     , \
                             pathName ) :
                             
            RETRY_DELAY = 1
                             
            while ( os.path.exists(pathName) ) :
            
                try :
            
                    shutil.rmtree(pathName)
                    
                except (IOError, WindowsError) as ex:
                
                    print(ex)
                    print("Retrying...")
                    time.sleep(RETRY_DELAY)
        #----------------------------------------------------------------------
    
    #--------------------------------------------------------------------------
    # private methods

        #----------------------------------------------------------------------
        # Appends a specified include to a specified include environment variable.
        #
        # Parameters :
        #     self                    : this manager
        #     environmentVariableName : the name of the environment variable
        #                               to which to append
        #     pathName                : the name of the include to append
        def _appendToIncludeEnvironmentVariable( self                    , \
                                                 environmentVariableName , \
                                                 pathName                ) :
                                                
            if (environmentVariableName in os.environ) :
            
                if ( not os.environ[environmentVariableName].endswith(SystemManager._VALUE_SEPARATOR) ) :
                
                    os.environ[environmentVariableName] += SystemManager._VALUE_SEPARATOR
                    
                os.environ[environmentVariableName] += pathName
            
            else :
            
                os.environ[environmentVariableName] = pathName
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
                                              pathName, prepend = False                ) :
                                                
            if (environmentVariableName in os.environ) :
                if prepend:
                    os.environ[environmentVariableName] = pathName + SystemManager._VALUE_SEPARATOR + os.environ[environmentVariableName]
                else:
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
    
    
        def _copyfile(self, src, dst):
            try:
                fin = os.open(src, READ_FLAGS)
                stat = os.fstat(fin)
                fout = os.open(dst, WRITE_FLAGS, stat.st_mode)
                for x in iter(lambda: os.read(fin, BUFFER_SIZE), ""):
                    os.write(fout, x)
            finally:
                try: os.close(fin)
                except: pass
                try: os.close(fout)
                except: pass

        def _copytree(self, src, dst, symlinks=False, ignore=[]):
            # print("copying " + src + " to " + dst)
            names = os.listdir(src)

            if not os.path.exists(dst):
                os.makedirs(dst)
            errors = []
            for name in names:
                if name in ignore:
                    continue
                srcname = os.path.join(src, name)
                dstname = os.path.join(dst, name)
                try:
                    if symlinks and os.path.islink(srcname):
                        linkto = os.readlink(srcname)
                        os.symlink(linkto, dstname)
                    elif os.path.isdir(srcname):
                        self._copytree(srcname, dstname, symlinks, ignore)
                    else:
                        self._copyfile(srcname, dstname)
                    # XXX What about devices, sockets etc.?
                except (IOError, os.error), why:
                    errors.append((srcname, dstname, str(why)))
                except CTError, err:
                    errors.extend(err.errors)
            if errors:
                raise CTError(errors)

#------------------------------------------------------------------------------
