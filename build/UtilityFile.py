#------------------------------------------------------------------------------
#
# UtilityFile.py
#
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# The UtilityFile static class provides methods for handling files.
class UtilityFile :

    #--------------------------------------------------------------------------
    # constants
    
        #----------------------------------------------------------------------
        # read access
        _ACCESS_READ = "r"
        #----------------------------------------------------------------------
        # write access
        _ACCESS_WRITE = "w"
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    # public methods
    
        #----------------------------------------------------------------------
        # Reads the lines of a specified file.
        #
        # Parameters :
        #     fileName : the name of the file whose lines to read
        # Returns :
        #     the read lines
        # Throws :
        #     Exception : if this method failed to read the lines of the
        #                 specified file
        @staticmethod
        def readLines(fileName) :
        
            with open( fileName                 , \
                       UtilityFile._ACCESS_READ ) \
                       as file :
                       
                lines = file.read().splitlines()
                file.close()
        
            return lines
        #----------------------------------------------------------------------
        # Replaces lines in a specified file that begin with keys in a
        # specified replacement dictionary.
        #
        # Parameters :
        #     fileName              : the name of the file whose lines to
        #                             replace
        #     replacementDictionary : the dictionary of replacement, with
        #                             start-with keys and replacement-line
        #                             values
        # Throws :
        #     Exception : if this method failed to replace the lines of the
        #                 specified file
        def replaceStartWithLines( fileName              , \
                                   replacementDictionary ) :
                                   
            # read the lines of the specified file
            lines = UtilityFile.readLines(fileName)
            
            # replace the lines of the specified files
            replaced = False
            for lineIndex in range( 0          , \
                                    len(lines) ) :
                                    
                for startWithKey in replacementDictionary :
                
                    if ( lines[lineIndex].startswith(startWithKey) ) :
                    
                        lines[lineIndex] = replacementDictionary[startWithKey]
                        replaced         = True
                        
            # write the lines of the specified, if this method replaced any
            #     lines
            if (replaced) :
            
                UtilityFile.writeLines( fileName , \
                                        lines    )
        #----------------------------------------------------------------------
        # Writes specified lines to a specified file.
        #
        # Parameters :
        #     fileName : the name of the file to which to write
        #     lines    : the lines to write
        # Throws :
        #     Exception : if this method failed to write the specified lines
        #                 to the specified file
        @staticmethod
        def writeLines( fileName , \
                        lines    ) :
        
            with open( fileName                  , \
                       UtilityFile._ACCESS_WRITE ) \
                       as file :
                       
                file.write( '\n'.join(lines) )
                file.close()
        
            return lines
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    
#------------------------------------------------------------------------------
