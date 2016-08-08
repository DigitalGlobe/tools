#------------------------------------------------------------------------------
#
# PathFinder.py
#
#------------------------------------------------------------------------------

import os

#------------------------------------------------------------------------------
# The PathFinder class represents finders that locate the paths of build
# tools.
class PathFinder :

    #--------------------------------------------------------------------------
    # constants 
    
        #----------------------------------------------------------------------
        # the name of the environment variable that contains the path of the
        # 32-bit program files folder
        _ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86 = "ProgramFiles(x86)"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the CMake executable file
        FILE_NAME_CMAKE = "cmake.exe"
        #----------------------------------------------------------------------
        # the name of the 32-bit MSBuild executable file
        _FILE_NAME_MSBUILD_X86 = "MSBuild\\14.0\\Bin\\MSBuild.exe"
        #----------------------------------------------------------------------
        # the name of the 64-bit MSBuild executable file
        _FILE_NAME_MSBUILD_X64 = "MSBuild\\14.0\\Bin\\amd64\\MSBuild.exe"
        #----------------------------------------------------------------------
        # the name of the Nmake executable file
        _FILE_NAME_NMAKE = "nmake.exe"
        #----------------------------------------------------------------------
        # the relative 64-bit Visual Studio bin path
        _PATH_NAME_BIN_X64 = "VC\\bin\\amd64"
        #----------------------------------------------------------------------
        # the relative 32-bit Visual Studio bin path
        _PATH_NAME_BIN_X86 = "VC\\bin\\"
        #----------------------------------------------------------------------
        # the relative Visual Studio include path
        _PATH_NAME_INCLUDE = "VC\\include\\"
        #----------------------------------------------------------------------
        # the relative path name of Visual Studio
        _PATH_NAME_VISUAL_STUDIO = "Microsoft Visual Studio 14.0"
        #----------------------------------------------------------------------
        # the relative path name of the Windows SDK
        _PATH_NAME_WINDOWS_SDK = "Microsoft SDKs\\Windows\\v7.1A\\Include\\"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the error for MSBuild files
        _ERROR_FILE_MSBUILD = "Could not find MSBuild."
        #----------------------------------------------------------------------
        # the error for Nmake files
        _ERROR_FILE_NMAKE = "Could not find Nmake."
        #----------------------------------------------------------------------
        # the error for Visual Studio bin paths
        _ERROR_PATH_BIN = "Could not find Visual Studio \"bin\" path."
        #----------------------------------------------------------------------
        # the error for Visual Studio include paths
        _ERROR_PATH_INCLUDE = "Could not find Visual Studio \"include\" path."
        #----------------------------------------------------------------------
        # the error for Visual Studio paths
        _ERROR_PATH_VISUAL_STUDIO = "Could not find Visual Studio path."
        #----------------------------------------------------------------------
        # the error for Windows SDK paths
        _ERROR_PATH_WINDOWS_SDK = "Could not find Windows SDK path."
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # constructors

        #----------------------------------------------------------------------
        # Constructs this finder as a default finder.
        #
        # Parameters :
        #     self : this finder
        def __init__(self) :
        
            pass
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # public methods

        #----------------------------------------------------------------------
        # Gets the name of the MSBuild executable file.
        #
        # Parameters :
        #     self         : this finder
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        # Returns :
        #     the name of the MSBuild executable file
        def getMSBuildFileName( self         , \
                                x64Specified ) :
        
            fileName = os.path.join( os.environ.get(PathFinder._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86) , \
                                     ( PathFinder._FILE_NAME_MSBUILD_X64 \
                                       if (x64Specified)                 \
                                       else PathFinder._FILE_NAME_MSBUILD_X86 )                         )
                                     
            if ( not os.path.exists(fileName) ) :
            
                raise Exception(PathFinder._ERROR_FILE_MSBUILD)
                
            return fileName
        #----------------------------------------------------------------------
        # Gets the name of the Nmake executable file.
        #
        # Parameters :
        #     self         : this finder
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        # Returns :
        #     the name of the Nmake executable file
        def getNmakeFileName( self         , \
                              x64Specified ) :
        
            fileName = os.path.join( self.getVisualStudioBinPathName(x64Specified) , \
                                     PathFinder._FILE_NAME_NMAKE                   )
                                     
            if ( not os.path.exists(fileName) ) :
            
                raise Exception(PathFinder._ERROR_FILE_NMAKE)
                
            return fileName
        #----------------------------------------------------------------------
        # Gets the name of the Visual Studio bin path.
        #
        # Parameters :
        #     self         : this finder
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        # Returns :
        #     the name of the Visual Studio bin path
        def getVisualStudioBinPathName( self         , \
                                        x64Specified ) :
        
            pathName = os.path.join( self.getVisualStudioPathName(x64Specified) ,
                                     ( PathFinder._PATH_NAME_BIN_X64      \
                                       if (x64Specified)                  \
                                       else PathFinder._PATH_NAME_BIN_X86 )     )
            if ( not os.path.exists(pathName) ) :
            
                raise Exception(PathFinder._ERROR_PATH_BIN)
                                     
            return pathName
        #----------------------------------------------------------------------
        # Gets the name of the Visual Studio include path.
        #
        # Parameters :
        #     self         : this finder
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        # Returns :
        #     the name of the Visual Studio include path
        def getVisualStudioIncludePathName( self         , \
                                            x64Specified ) :
        
            pathName = os.path.join( self.getVisualStudioPathName(x64Specified) ,
                                     PathFinder._PATH_NAME_INCLUDE              )
            if ( not os.path.exists(pathName) ) :
            
                raise Exception(PathFinder._ERROR_PATH_INCLUDE)
                                     
            return pathName
        #----------------------------------------------------------------------
        # Gets the name of the Visual Studio path.
        #
        # Parameters :
        #     self         : this finder
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        # Returns :
        #     the name of the Visual Studio path
        # Throws :
        #     Exception : if this method failed to find Visual Studio
        def getVisualStudioPathName( self         , \
                                     x64Specified ) :
        
            pathName = os.path.join( os.environ.get(PathFinder._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86) , \
                                     PathFinder._PATH_NAME_VISUAL_STUDIO                                )
            if ( not os.path.exists(pathName) ) :
            
                raise Exception(PathFinder._ERROR_PATH_VISUAL_STUDIO)
                                     
            return pathName
        #----------------------------------------------------------------------
        # Gets the name of the Windows SDK path.
        #
        # Parameters :
        #     self         : this finder
        #     x64Specified : if <code>true</code>, 64-bit is specified; if
        #                    <code>false</code>, 32-bit is specified
        # Returns :
        #     the name of the Windows SDK path
        def getWindowsSdkPathName( self         , \
                                   x64Specified ) :
        
            pathName = os.path.join( os.environ.get(PathFinder._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86) , \
                                     PathFinder._PATH_NAME_WINDOWS_SDK                                  )
            if ( not os.path.exists(pathName) ) :
            
                raise Exception(PathFinder._ERROR_PATH_WINDOWS_SDK)
                                     
            return pathName
        #----------------------------------------------------------------------
    
#------------------------------------------------------------------------------
