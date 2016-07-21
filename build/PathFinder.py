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
        # the name of the Nmake executable file
        _FILE_NAME_NMAKE = "nmake.exe"
        #----------------------------------------------------------------------
        # the relative Visual Studio bin path
        _PATH_NAME_BIN = "VC\\bin\\"
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
        # Gets the name of the Nmake executable file.
        #
        # Parameters :
        #     self : this finder
        # Returns :
        #     the name of the Nmake executable file
        def getNmakeFileName(self) :
        
            fileName = os.path.join( self.getVisualStudioBinPathName() , \
                                     PathFinder._FILE_NAME_NMAKE       )
                                     
            if ( not os.path.exists(fileName) ) :
            
                raise Exception(_ERROR_FILE_NMAKE)
                
            return fileName
        #----------------------------------------------------------------------
        # Gets the name of the Visual Studio bin path.
        #
        # Parameters :
        #     self: this finder
        # Returns :
        #     the name of the Visual Studio bin path
        def getVisualStudioBinPathName(self) :
        
            pathName = os.path.join( self.getVisualStudioPathName() ,
                                     PathFinder._PATH_NAME_BIN      )
            if ( not os.path.exists(pathName) ) :
            
                raise Exception(PathFinder._ERROR_PATH_BIN)
                                     
            return pathName
        #----------------------------------------------------------------------
        # Gets the name of the Visual Studio include path.
        #
        # Parameters :
        #     self: this finder
        # Returns :
        #     the name of the Visual Studio include path
        def getVisualStudioIncludePathName(self) :
        
            pathName = os.path.join( self.getVisualStudioPathName() ,
                                     PathFinder._PATH_NAME_INCLUDE  )
            if ( not os.path.exists(pathName) ) :
            
                raise Exception(PathFinder._ERROR_PATH_INCLUDE)
                                     
            return pathName
        #----------------------------------------------------------------------
        # Gets the name of the Visual Studio path.
        #
        # Parameters :
        #     self: this finder
        # Returns :
        #     the name of the Visual Studio path
        # Throws :
        #     Exception : if this method failed to find Visual Studio
        def getVisualStudioPathName(self) :
        
            pathName = os.path.join( os.environ.get(PathFinder._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86) , \
                                     PathFinder._PATH_NAME_VISUAL_STUDIO                                )
            if ( not os.path.exists(pathName) ) :
            
                raise Exception(PathFinder._ERROR_PATH_VISUAL_STUDIO)
                                     
            return pathName
        #----------------------------------------------------------------------
        # Gets the name of the Windows SDK path.
        #
        # Parameters :
        #     self: this finder
        # Returns :
        #     the name of the Windows SDK path
        def getWindowsSdkPathName(self) :
        
            pathName = os.path.join( os.environ.get(PathFinder._ENVIRONMENT_VARIABLE_PROGRAM_FILES_X86) , \
                                     PathFinder._PATH_NAME_WINDOWS_SDK                                  )
            if ( not os.path.exists(pathName) ) :
            
                raise Exception(PathFinder._ERROR_PATH_WINDOWS_SDK)
                                     
            return pathName
        #----------------------------------------------------------------------
    
#------------------------------------------------------------------------------
