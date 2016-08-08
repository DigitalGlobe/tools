//-----------------------------------------------------------------------------
/*
 * UtilityDirectory.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <direct.h>
#include <string>

//-----------------------------------------------------------------------------
namespace DigitalGlobeTesting
{
    //-------------------------------------------------------------------------
    /**
     * The <code>CUtilityDirectory</code> static class provides methods for
     * handling directories.
     */
    class CUtilityDirectory
    {
        public:
        
            //-----------------------------------------------------------------
            // public methods
            
                //-------------------------------------------------------------
                /**
                 * Gets the path name of the current directory.
                 *
                 * Returns :
                 *     the path name of the current directory
                 */
                static std::string GetCurrentDirectory()
                {
                    char* sCurrentPathName = nullptr;

                    sCurrentPathName = ::_getcwd( NULL ,
                                                  0    );

                    return sCurrentPathName;
                }
                //-------------------------------------------------------------
            
            //-----------------------------------------------------------------
            
        private:
            
            //-----------------------------------------------------------------
            // private constructors
            
                //-------------------------------------------------------------
                /**
                 * Constructs this object as a default object.
                 */
                CUtilityDirectory() = delete;
                //-------------------------------------------------------------
            
    };
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
