#------------------------------------------------------------------------------
#
# FileDistributor.py
#
#------------------------------------------------------------------------------

import glob
import os
import shutil

from UtilityPathName import *

#------------------------------------------------------------------------------
# The FileDistributor class represents distributors that copy built files into
# to distribution folders.
class FileDistributor :

    #--------------------------------------------------------------------------
    # constants 
    
        #----------------------------------------------------------------------
        # the suffix for a debug file name
        _DEBUG_SUFFIX = "_d"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the pattern for binary files
        _FILE_PATTERN_BINARY = "*.exe"
        #----------------------------------------------------------------------
        # the pattern for debug files
        _FILE_PATTERN_DEBUG = "*.pdb"
        #----------------------------------------------------------------------
        # the pattern for dynamic files
        _FILE_PATTERN_DYNAMIC = "*.dll"
        #----------------------------------------------------------------------
        # the pattern for library files
        _FILE_PATTERN_LIBRARY = "*.lib"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the name of the default path to which to distribute 64-bit binary
        # files
        _PATH_NAME_DISTRIBUTION_BINARY_X64 = "..\\sdk\\x64\\bin"
        #----------------------------------------------------------------------
        # the name of the default path to which to distribute 32-bit binary
        # files
        _PATH_NAME_DISTRIBUTION_BINARY_X86 = "..\\sdk\\x86\\bin"
        #----------------------------------------------------------------------
        # the name of the default path to which to distribute 64-bit debug
        # files
        _PATH_NAME_DISTRIBUTION_DEBUG_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the default path to which to distribute 32-bit debug
        # files
        _PATH_NAME_DISTRIBUTION_DEBUG_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the default path to which to distribute 64-bit dynamic
        # files
        _PATH_NAME_DISTRIBUTION_DYNAMIC_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the default path to which to distribute 32-bit dynamic
        # files
        _PATH_NAME_DISTRIBUTION_DYNAMIC_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        # the name of the default path to which to distribute 64-bit library
        # files
        _PATH_NAME_DISTRIBUTION_LIBRARY_X64 = "..\\sdk\\x64\\lib"
        #----------------------------------------------------------------------
        # the name of the default path to which to distribute 32-bit library
        # files
        _PATH_NAME_DISTRIBUTION_LIBRARY_X86 = "..\\sdk\\x86\\lib"
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # fields
    
        #----------------------------------------------------------------------
        # the name of the path to which to distribute x64 binary files
        _distributionBinaryPathNameX64 = ""
        #----------------------------------------------------------------------
        # the name of the path to which to distribute x86 binary files
        _distributionBinaryPathNameX86 = ""
        #----------------------------------------------------------------------
        # the name of the path to which to distribute x64 debug files
        _distributionDebugPathNameX64 = ""
        #----------------------------------------------------------------------
        # the name of the path to which to distribute x86 debug files
        _distributionDebugPathNameX86 = ""
        #----------------------------------------------------------------------
        # the name of the path to which to distribute x64 dynamic files
        _distributionDynamicPathNameX64 = ""
        #----------------------------------------------------------------------
        # the name of the path to which to distribute x86 dynamic files
        _distributionDynamicPathNameX86 = ""
        #----------------------------------------------------------------------
        # the name of the path to which to distribute x64 library files
        _distributionLibraryPathNameX64 = ""
        #----------------------------------------------------------------------
        # the name of the path to which to distribute x86 library files
        _distributionLibraryPathNameX86 = ""
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # properties
    
        #----------------------------------------------------------------------
        # Gets the name of the path to which to distribute x64 binary files.
        #
        # Parameters :
        #     self : this distributor
        # Returns :
        #     the name of the path to which to distribute x64 binary files
        def getDistributionBinaryPathNameX64(self) :
        
            return self._distributionBinaryPathNameX64
        
        #----------------------------------------------------------------------
        # Sets x64 binary files distribution path name.
        #
        # Parameters :
        #     self  : this distributor
        #     value : x64 binary files distribution path name to use
        def setDistributionBinaryPathNameX64( self  , \
                                              value ) :
        
            self._distributionBinaryPathNameX64 = value
        
        #----------------------------------------------------------------------
        # Gets the name of the path to which to distribute x86 binary files.
        #
        # Parameters :
        #     self : this distributor
        # Returns :
        #     the name of the path to which to distribute x86 binary files
        def getDistributionBinaryPathNameX86(self) :
        
            return self._distributionBinaryPathNameX86
        
        #----------------------------------------------------------------------
        # Sets x86 binary files distribution path name.
        #
        # Parameters :
        #     self  : this distributor
        #     value : x86 binary files distribution path name to use
        def setDistributionBinaryPathNameX86( self  , \
                                              value ) :
        
            self._distributionBinaryPathNameX86 = value
        
        #----------------------------------------------------------------------
        # Gets the name of the path to which to distribute x64 debug files.
        #
        # Parameters :
        #     self : this distributor
        # Returns :
        #     the name of the path to which to distribute x64 debug files
        def getDistributionDebugPathNameX64(self) :
        
            return self._distributionDebugPathNameX64
        
        #----------------------------------------------------------------------
        # Sets x64 debug files distribution path name.
        #
        # Parameters :
        #     self  : this distributor
        #     value : x64 debug files distribution path name to use
        def setDistributionDebugPathNameX64( self  , \
                                             value ) :
        
            self._distributionDebugPathNameX64 = value
        
        #----------------------------------------------------------------------
        # Gets the name of the path to which to distribute x86 debug files.
        #
        # Parameters :
        #     self : this distributor
        # Returns :
        #     the name of the path to which to distribute x86 debug files
        def getDistributionDebugPathNameX86(self) :
        
            return self._distributionDebugPathNameX86
        
        #----------------------------------------------------------------------
        # Sets x86 debug files distribution path name.
        #
        # Parameters :
        #     self  : this distributor
        #     value : x86 debug files distribution path name to use
        def setDistributionDebugPathNameX86( self  , \
                                             value ) :
        
            self._distributionDebugPathNameX86 = value
        
        #----------------------------------------------------------------------
        # Gets the name of the path to which to distribute x64 dynamic files.
        #
        # Parameters :
        #     self : this distributor
        # Returns :
        #     the name of the path to which to distribute x64 dynamic files
        def getDistributionDynamicPathNameX64(self) :
        
            return self._distributionDynamicPathNameX64
        
        #----------------------------------------------------------------------
        # Sets x64 dynamic files distribution path name.
        #
        # Parameters :
        #     self  : this distributor
        #     value : x64 dynamic files distribution path name to use
        def setDistributionDynamicPathNameX64( self  , \
                                               value ) :
        
            self._distributionDynamicPathNameX64 = value
        
        #----------------------------------------------------------------------
        # Gets the name of the path to which to distribute x86 dynamic files.
        #
        # Parameters :
        #     self : this distributor
        # Returns :
        #     the name of the path to which to distribute x86 dynamic files
        def getDistributionDynamicPathNameX86(self) :
        
            return self._distributionDynamicPathNameX86
        
        #----------------------------------------------------------------------
        # Sets x86 dynamic files distribution path name.
        #
        # Parameters :
        #     self  : this distributor
        #     value : x86 dynamic files distribution path name to use
        def setDistributionDynamicPathNameX86( self  , \
                                               value ) :
        
            self._distributionDynamicPathNameX86 = value
        
        #----------------------------------------------------------------------
        # Gets the name of the path to which to distribute x64 library files.
        #
        # Parameters :
        #     self : this distributor
        # Returns :
        #     the name of the path to which to distribute x64 library files
        def getDistributionLibraryPathNameX64(self) :
        
            return self._distributionLibraryPathNameX64
        
        #----------------------------------------------------------------------
        # Sets x64 library files distribution path name.
        #
        # Parameters :
        #     self  : this distributor
        #     value : x64 library files distribution path name to use
        def setDistributionLibraryPathNameX64( self  , \
                                               value ) :
        
            self._distributionLibraryPathNameX64 = value
        
        #----------------------------------------------------------------------
        # Gets the name of the path to which to distribute x86 library files.
        #
        # Parameters :
        #     self : this distributor
        # Returns :
        #     the name of the path to which to distribute x86 library files
        def getDistributionLibraryPathNameX86(self) :
        
            return self._distributionLibraryPathNameX86
        
        #----------------------------------------------------------------------
        # Sets x86 library files distribution path name.
        #
        # Parameters :
        #     self  : this distributor
        #     value : x86 library files distribution path name to use
        def setDistributionLibraryPathNameX86( self  , \
                                               value ) :
        
            self._distributionLibraryPathNameX86 = value
        
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # constructors

        #----------------------------------------------------------------------
        # Constructs this distributor as a default distributor.
        #
        # Parameters :
        #     self          : this manager
        #     buildPathName : the name of the path that contains the build
        #                     files or an empty string if this constructor
        #                     is to assume that the current path is the build
        #                     path
        def __init__( self               , \
                      buildPathName = "" ) :
                      
            finalBuildPathName = ""
        
            # determine the build path to use
            finalBuildPathName = ( buildPathName      \
                                   if (buildPathName) \
                                   else os.path.dirname( os.path.abspath(__file__) ) )
                                   
            # initialize fields
            self._distributionBinaryPathNameX64  = os.path.normpath( os.path.join( finalBuildPathName                                  , \
                                                                                   FileDistributor._PATH_NAME_DISTRIBUTION_BINARY_X64  ) )
            self._distributionBinaryPathNameX86  = os.path.normpath( os.path.join( finalBuildPathName                                  , \
                                                                                   FileDistributor._PATH_NAME_DISTRIBUTION_BINARY_X86  ) )
            self._distributionDebugPathNameX64   = os.path.normpath( os.path.join( finalBuildPathName                                  , \
                                                                                   FileDistributor._PATH_NAME_DISTRIBUTION_DEBUG_X64   ) )
            self._distributionDebugPathNameX86   = os.path.normpath( os.path.join( finalBuildPathName                                  , \
                                                                                   FileDistributor._PATH_NAME_DISTRIBUTION_DEBUG_X86   ) )
            self._distributionDynamicPathNameX64 = os.path.normpath( os.path.join( finalBuildPathName                                  , \
                                                                                   FileDistributor._PATH_NAME_DISTRIBUTION_DYNAMIC_X64 ) )
            self._distributionDynamicPathNameX86 = os.path.normpath( os.path.join( finalBuildPathName                                  , \
                                                                                   FileDistributor._PATH_NAME_DISTRIBUTION_DYNAMIC_X86 ) )
            self._distributionLibraryPathNameX64 = os.path.normpath( os.path.join( finalBuildPathName                                  , \
                                                                                   FileDistributor._PATH_NAME_DISTRIBUTION_LIBRARY_X64 ) )
            self._distributionLibraryPathNameX86 = os.path.normpath( os.path.join( finalBuildPathName                                  , \
                                                                                   FileDistributor._PATH_NAME_DISTRIBUTION_LIBRARY_X86 ) )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # public methods

        #----------------------------------------------------------------------
        # Determines which of the specified path names to use, based on
        # whether x64 is specified and whether release is specified.
        #
        # Parameters :
        #     x64Specified       : if <code>True</code>, x64 is specified; if
        #                          <code>False</code>, x86 is specified
        #     releaseSpecified   : if <code>True</code>, release is specified;
        #                          if <code>False</code>, debug is specified
        #     x86DebugPathName   : the path name to use for x86 debug
        #     x86ReleasePathName : the path name to use for x86 release
        #     x64DebugPathName   : the path name to use for x64 debug
        #     x64ReleasePathName : the path name to use for x64 release
        def determinePathName( self               , \
                               x64Specified       , \
                               releaseSpecified   , \
                               x86DebugPathName   , \
                               x86ReleasePathName , \
                               x64DebugPathName   , \
                               x64ReleasePathName ) :
                               
            pathName = ""

            if ( releaseSpecified and \
                 x64Specified       ) :
                 
                 pathName = x64ReleasePathName
                 
            elif (releaseSpecified) :
            
                 pathName = x86ReleasePathName
                 
            elif (x64Specified) :
            
                 pathName = x64DebugPathName
                 
            else :
            
                 pathName = x86DebugPathName
                 
            return pathName
        #----------------------------------------------------------------------
        # Distributes all files.
        #
        # Parameters :
        #    self                  : this manager
        #    binarySourcePathName  : the name of the binary source path to use
        #    debugSourcePathName   : the name of the debug source path to use
        #    dynamicSourcePathName : the name of the dynamic source path to
        #                            use
        #    librarySourcePathName : the name of the library source path to
        #                            use
        #    x64Specified          : if <code>True</code>, this method will
        #                            distribute to the x64 library distribution
        #                            path; if <code>False</code>, this method will
        #                            distribute to the x86 library distribution path
        #    releaseSpecified      : if <code>True</code>, this method will use
        #                            the same source file names as the distributed
        #                            file name; if <code>False</code>, this method
        #                            will use a debug version of the source file
        #                            name as the distributed file name
        #    dConsidered           : if <code>True</code> and the
        #                            <code>releaseSpecified</code> parameter is
        #                            <code>True</code>, this method will handle
        #                            <code>d</code> suffixes in debug file names
        #    renameDictionary      : the dictionary to use for renaming files,
        #                            with source files names as keys and target
        #                            file names as values
        def distributeAllFiles( self                     , \
                                binarySourcePathName     , \
                                debugSourcePathName      , \
                                dynamicSourcePathName    , \
                                librarySourcePathName    , \
                                x64Specified             , \
                                releaseSpecified         , \
                                dConsidered      = False , \
                                renameDictionary = {}    ) :
                                      
            self.distributeBinaryFiles( binarySourcePathName , \
                                        x64Specified         , \
                                        releaseSpecified     , \
                                        dConsidered          , \
                                        renameDictionary     )
            self.distributeDebugFiles( debugSourcePathName , \
                                       x64Specified        , \
                                       releaseSpecified    , \
                                       dConsidered         , \
                                       renameDictionary    )
            self.distributeDynamicFiles( dynamicSourcePathName , \
                                         x64Specified          , \
                                         releaseSpecified      , \
                                         dConsidered           , \
                                         renameDictionary      )
            self.distributeLibraryFiles( librarySourcePathName , \
                                         x64Specified          , \
                                         releaseSpecified      , \
                                         dConsidered           , \
                                         renameDictionary      )
        #----------------------------------------------------------------------
        # Distributes the binary files in a specified source path.
        #
        # Parameters :
        #    self             : this manager
        #    sourcePathName   : the name of the source path to use
        #    x64Specified     : if <code>True</code>, this method will
        #                       distribute to the x64 binary distribution
        #                       path; if <code>False</code>, this method will
        #                       distribute to the x86 binary distribution path
        #    releaseSpecified : if <code>True</code>, this method will use
        #                       the same source file names as the distributed
        #                       file name; if <code>False</code>, this method
        #                       will use a debug version of the source file
        #                       name as the distributed file name
        #    dConsidered      : if <code>True</code> and the
        #                       <code>releaseSpecified</code> parameter is
        #                       <code>True</code>, this method will handle
        #                       <code>d</code> suffixes in debug file names
        #    renameDictionary : the dictionary to use for renaming files,
        #                       with source files names as keys and target
        #                       file names as values
        def distributeBinaryFiles( self                     , \
                                   sourcePathName           , \
                                   x64Specified             , \
                                   releaseSpecified         , \
                                   dConsidered      = False , \
                                   renameDictionary = {}    ) :
                                      
            self._distributeFiles( sourcePathName                               , \
                                   ( self._distributionBinaryPathNameX64        \
                                     if (x64Specified)                          \
                                     else self._distributionBinaryPathNameX86 ) , \
                                   FileDistributor._FILE_PATTERN_BINARY         , \
                                   releaseSpecified                             , \
                                   dConsidered                                  , \
                                   renameDictionary                             )
        #----------------------------------------------------------------------
        # Distributes the debug files in a specified source path.
        #
        # Parameters :
        #    self             : this manager
        #    sourcePathName   : the name of the source path to use
        #    x64Specified     : if <code>True</code>, this method will
        #                       distribute to the x64 debug distribution
        #                       path; if <code>False</code>, this method will
        #                       distribute to the x86 debug distribution path
        #    releaseSpecified : if <code>True</code>, this method will use
        #                       the same source file names as the distributed
        #                       file name; if <code>False</code>, this method
        #                       will use a debug version of the source file
        #                       name as the distributed file name
        #    dConsidered      : if <code>True</code> and the
        #                       <code>releaseSpecified</code> parameter is
        #                       <code>True</code>, this method will handle
        #                       <code>d</code> suffixes in debug file names
        #    renameDictionary : the dictionary to use for renaming files,
        #                       with source files names as keys and target
        #                       file names as values
        def distributeDebugFiles( self                     , \
                                  sourcePathName           , \
                                  x64Specified             , \
                                  releaseSpecified         , \
                                  dConsidered      = False , \
                                  renameDictionary = {}    ) :
                                      
            self._distributeFiles( sourcePathName                              , \
                                   ( self._distributionDebugPathNameX64        \
                                     if (x64Specified)                         \
                                     else self._distributionDebugPathNameX86 ) , \
                                   FileDistributor._FILE_PATTERN_DEBUG         , \
                                   releaseSpecified                            , \
                                   dConsidered                                 , \
                                   renameDictionary                            )
        #----------------------------------------------------------------------
        # Distributes the dynamic files in a specified source path.
        #
        # Parameters :
        #    self             : this manager
        #    sourcePathName   : the name of the source path to use
        #    x64Specified     : if <code>True</code>, this method will
        #                       distribute to the x64 dynamic distribution
        #                       path; if <code>False</code>, this method will
        #                       distribute to the x86 dynamic distribution path
        #    releaseSpecified : if <code>True</code>, this method will use
        #                       the same source file names as the distributed
        #                       file name; if <code>False</code>, this method
        #                       will use a debug version of the source file
        #                       name as the distributed file name
        #    dConsidered      : if <code>True</code> and the
        #                       <code>releaseSpecified</code> parameter is
        #                       <code>True</code>, this method will handle
        #                       <code>d</code> suffixes in debug file names
        #    renameDictionary : the dictionary to use for renaming files,
        #                       with source files names as keys and target
        #                       file names as values
        def distributeDynamicFiles( self                     , \
                                    sourcePathName           , \
                                    x64Specified             , \
                                    releaseSpecified         , \
                                    dConsidered      = False , \
                                    renameDictionary = {}    ) :
                                      
            self._distributeFiles( sourcePathName                                , \
                                   ( self._distributionDynamicPathNameX64        \
                                     if (x64Specified)                           \
                                     else self._distributionDynamicPathNameX86 ) , \
                                   FileDistributor._FILE_PATTERN_DYNAMIC         , \
                                   releaseSpecified                              , \
                                   dConsidered                                   , \
                                   renameDictionary                              )
        #----------------------------------------------------------------------
        # Distributes the library files in a specified source path.
        #
        # Parameters :
        #    self             : this manager
        #    sourcePathName   : the name of the source path to use
        #    x64Specified     : if <code>True</code>, this method will
        #                       distribute to the x64 library distribution
        #                       path; if <code>False</code>, this method will
        #                       distribute to the x86 library distribution path
        #    releaseSpecified : if <code>True</code>, this method will use
        #                       the same source file names as the distributed
        #                       file name; if <code>False</code>, this method
        #                       will use a debug version of the source file
        #                       name as the distributed file name
        #    dConsidered      : if <code>True</code> and the
        #                       <code>releaseSpecified</code> parameter is
        #                       <code>True</code>, this method will handle
        #                       <code>d</code> suffixes in debug file names
        #    renameDictionary : the dictionary to use for renaming files,
        #                       with source files names as keys and target
        #                       file names as values
        def distributeLibraryFiles( self                     , \
                                    sourcePathName           , \
                                    x64Specified             , \
                                    releaseSpecified         , \
                                    dConsidered      = False , \
                                    renameDictionary = {}    ) :
                                      
            self._distributeFiles( sourcePathName                                , \
                                   ( self._distributionLibraryPathNameX64        \
                                     if (x64Specified)                           \
                                     else self._distributionLibraryPathNameX86 ) , \
                                   FileDistributor._FILE_PATTERN_LIBRARY         , \
                                   releaseSpecified                              , \
                                   dConsidered                                   , \
                                   renameDictionary                              )
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # public methods

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
        #    renameDictionary : the dictionary to use for renaming files,
        #                       with source files names as keys and target
        #                       file names as values
        def _distributeFiles( self                     , \
                              sourcePathName           , \
                              targetPathName           , \
                              filePattern              , \
                              releaseSpecified         , \
                              dConsidered      = False , \
                              renameDictionary = {}    ) :
                             
            sourceFileNames = glob.glob( os.path.join( sourcePathName , \
                                                       filePattern    ) );
            for sourceFileName in sourceFileNames :
            
                baseFileName = os.path.basename(sourceFileName)
                if (baseFileName in renameDictionary) :
                
                    baseFileName = renameDictionary[baseFileName]
                    
                targetFileName = os.path.join( targetPathName , \
                                               baseFileName   )
                if (not releaseSpecified) : \
                    
                    targetFileName = self._getDebugFileName( targetFileName , \
                                                             dConsidered    )
                                                            
                shutil.copyfile( sourceFileName , \
                                 targetFileName )
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
        def _getDebugFileName( self                ,
                               fileName            ,
                               dConsidered = False ) :
                              
            fileNameWithoutExtension = os.path.splitext(fileName)[0]
            extension                = os.path.splitext(fileName)[1]
            
            if ( dConsidered and \
                 ( fileNameWithoutExtension.endswith("_d") or \
                   fileNameWithoutExtension.endswith("_D")  ) ) :
                   
                fileNameWithoutExtension = fileNameWithoutExtension[ 0:( len(fileNameWithoutExtension) - 2 ) ]
                              
            elif ( dConsidered and \
                   ( fileNameWithoutExtension.endswith("d") or \
                     fileNameWithoutExtension.endswith("D")  ) ) :
                   
                fileNameWithoutExtension = fileNameWithoutExtension[ 0:( len(fileNameWithoutExtension) - 1 ) ]
                              
            return ( fileNameWithoutExtension      + \
                     FileDistributor._DEBUG_SUFFIX + \
                     extension                     )
        #----------------------------------------------------------------------
    
#------------------------------------------------------------------------------
