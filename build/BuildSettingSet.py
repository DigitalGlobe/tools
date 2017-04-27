#------------------------------------------------------------------------------
#
# BuildSettingSet.py
#
#------------------------------------------------------------------------------

import argparse

#------------------------------------------------------------------------------
# The BuildSettingSet class represents sets of build settings.
class BuildSettingSet :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # a description of the bitness argument
        _ARGUMENT_DESCRIPTION_BITNESS = "the bitness of the build, either \"x86\" or \"x64\""
        #----------------------------------------------------------------------
        # the name of the bitness argument
        _ARGUMENT_NAME_BITNESS = "bitness"
        #----------------------------------------------------------------------
        # the value of the 32-bit bitness argument
        ARGUMENT_VALUE_BITNESS_X86 = "x86"
        #----------------------------------------------------------------------
        # the value of the 64-bit bitness argument
        ARGUMENT_VALUE_BITNESS_X64 = "x64"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # a description of the configuration argument
        _ARGUMENT_DESCRIPTION_CONFIGURATION = "the configuration of the build, either \"debug\" or \"release\""
        #----------------------------------------------------------------------
        # the name of the configuration argument
        _ARGUMENT_NAME_CONFIGURATION = "configuration"
        #----------------------------------------------------------------------
        # the value of the debug configuration argument
        ARGUMENT_VALUE_CONFIGURATION_DEBUG = "debug"
        #----------------------------------------------------------------------
        # the value of the release configuration argument
        ARGUMENT_VALUE_CONFIGURATION_RELEASE = "release"
        #----------------------------------------------------------------------
        
        #----------------------------------------------------------------------
        # the error for invalid bitness
        _ERROR_BITNESS = "Bitness must be either \"x86\" or \"x64\"."
        #----------------------------------------------------------------------
        # the error for invalid configuration
        _ERROR_CONFIGURATION = "Configuration must be either \"debug\" or \"release\"."
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # fields
    
        #----------------------------------------------------------------------
        # a description of the build to which this set applies
        _description = ""
        #----------------------------------------------------------------------
        # <code>True</code> if release is specified for this set,
        # <code>False</code> if debug is specified for this set
        _releaseSpecified = False
        #----------------------------------------------------------------------
        # <code>True</code> if 64-bit is specified for this set,
        # <code>False</code> if 32-bit is specified for this set
        _x64Specified = False
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # constructors
    
        #----------------------------------------------------------------------
        # Constructs this set using specified parameters.
        #
        # Parameters :
        #     self             : this BuildSettingSet
        #     description      : a description of the build to which this set
        #                        applies
        #     releaseSpecified : if <code>True</code>, release is specified
        #                        for this set; if <code>False</code>, debug
        #                        is specified for this set
        #     x64Specified     : if <code>True</code>, 64-bit is specified
        #                        for this set; if <code>False</code>, 32-bit
        #                        is specified for this set
        def __init__( self                     , \
                      description      = ""    , \
                      releaseSpecified = False , \
                      x64Specified     = False ) :
        
            self._description      = description
            self._releaseSpecified = releaseSpecified
            self._x64Specified     = x64Specified
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # properties
    
        #----------------------------------------------------------------------
        # Gets a description of the build to which this set applies.
        #
        # Returns :
        #     a description of the build to which this set applies
        @property
        def Description(self) :
        
            return self._description
        #----------------------------------------------------------------------
        # Gets whether release is specified.
        #
        # Returns :
        #     <code>True</code> if release is specified for this set,
        #     <code>False</code> if debug is specified for this set
        def ReleaseSpecified(self) :
        
            return self._releaseSpecified
        #----------------------------------------------------------------------
        # Gets whether 64-bit is specified.
        #
        # Returns :
        #     <code>True</code> if 64-bit is specified for this set,
        #     <code>False</code> if 32-bit is specified for this set
        def X64Specified(self) :
        
            return self._x64Specified
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # public methods
    
        #----------------------------------------------------------------------
        # Creates a set from command-line arguments.
        #
        # Parameters :
        #     description : a description of the build to which this set applies
        # Returns :
        #     the created set
        @staticmethod
        def fromCommandLine(description) :
        
            set = BuildSettingSet()
            set._parseArguments()
            set._description = description
            
            return set
        
        #----------------------------------------------------------------------
        # Returns a string representation of this set.
        #
        # Parameters :
        #     self : this set
        # Returns :
        #     a string representation of this set
        def toString(self) :
        
            representation = ""
            
            representation += ( "Description=\""  + \
                                self._description + \
                                "\""              )
            representation += ","
            representation += ( "ReleaseSpecified="         + \
                                str(self._releaseSpecified) )
            representation += ","
            representation += ( "X64Specified="         + \
                                str(self._x64Specified) )
            
            return representation
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # private methods
    
        #----------------------------------------------------------------------
        # Creates a parser to parse command-line arguments.
        #
        # Parameters :
        #     self : this BuildSettingSet
        # Returns :
        #     the created parser
        def _createArgumentParser(self):

            argumentParser = argparse.ArgumentParser(self._description)
            argumentParser.add_argument( BuildSettingSet._ARGUMENT_NAME_BITNESS                     , \
                                         help = BuildSettingSet._ARGUMENT_DESCRIPTION_BITNESS       )
            argumentParser.add_argument( BuildSettingSet._ARGUMENT_NAME_CONFIGURATION               , \
                                         help = BuildSettingSet._ARGUMENT_DESCRIPTION_CONFIGURATION )

            return argumentParser
        #----------------------------------------------------------------------
        # Parses command-line arguments.
        #
        # Parameters :
        #     self : this BuildSettingSet
        # Throws: 
        #     Exception : if the command-line arguments are invalid
        def _parseArguments(self) :
        
            # parse the command line
            argumentParser = self._createArgumentParser()
            arguments      = argumentParser.parse_args();
            
            # process the bitness argument
            if (arguments.bitness == BuildSettingSet.ARGUMENT_VALUE_BITNESS_X86) :
            
                self._x64Specified = False
            
            elif (arguments.bitness == BuildSettingSet.ARGUMENT_VALUE_BITNESS_X64) :
            
                self._x64Specified = True
            
            else :
            
                raise Exception(BuildSettingSet._ERROR_BITNESS)
                
            # process the configuration argument
            if (arguments.configuration == BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_DEBUG) :
            
                self._releaseSpecified = False
            
            elif (arguments.configuration == BuildSettingSet.ARGUMENT_VALUE_CONFIGURATION_RELEASE) :
            
                self._releaseSpecified = True
            
            else :
            
                raise Exception(BuildSettingSet._ERROR_CONFIGURATION)
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    
#------------------------------------------------------------------------------
