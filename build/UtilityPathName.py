#------------------------------------------------------------------------------
#
# UtilityPathName.py
#
#------------------------------------------------------------------------------

import os

#------------------------------------------------------------------------------
# The UtilityPathName static class provides methods for handling path names.
class UtilityPathName :

    #--------------------------------------------------------------------------
    # public methods
    
        #----------------------------------------------------------------------
        # Gets the file name of a specified path name.  For example, if the
        # specified file name is:
        #     "C:\Directory\File.txt"
        # this method will return:
        #     "File.txt".
        #
        # Parameters :
        #     pathName : the path name whose file name to get
        # Returns:
        #     the file name of the specified path name
        def getFileName(pathName) :
        
            return os.path.basename(pathName)
        
        #----------------------------------------------------------------------
        # Gets the parent of a specified path name.  For example, if the
        # specified path name is:
        #     "C:\Directory\File.txt"
        # or
        #     "C:\Directory\Subdirectory"
        # this method will return:
        #     "C:\Directory".
        #
        # Parameters :
        #     pathName : the path name whose parent to get
        # Returns:
        #     the parent of the specified path name
        def getParent(pathName) :
        
            return os.path.dirname(pathName)
        
        #----------------------------------------------------------------------
        # Gets the specified path name with any extension removed.  For
        # example, if the specified path name is:
        #     "C:\Directory\File.txt"
        # or:
        #     "C:\Directory\File"
        # this method will return:
        #     "C:\Directory\File".
        #
        # Parameters :
        #     pathName  the name of the path whose extension to remove
        # Returns :
        #     a copy of the specified path name, with the extension, if there
        #     is one, removed
        @staticmethod
        def getPathNameWithoutExtension(pathName) :
        
            pathNameWithoutExtension = os.path.splitext(pathName)[0]
            extension                = os.path.splitext(pathName)[1]
            
            return pathNameWithoutExtension
        #----------------------------------------------------------------------
        
    #--------------------------------------------------------------------------
    
#------------------------------------------------------------------------------
