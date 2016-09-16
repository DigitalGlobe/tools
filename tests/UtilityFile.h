//-----------------------------------------------------------------------------
/*
 * UtilityFile.h
 */
//-----------------------------------------------------------------------------

#pragma once

#incldue <fstream>
#include <string>

//-----------------------------------------------------------------------------
namespace DigitalGlobeTesting
{
    //-------------------------------------------------------------------------
    /**
     * The <code>CFileDirectory</code> static class provides methods for
     * handling files.
     */
    class CUtilityFile
    {
        public:
        
            //-----------------------------------------------------------------
            // public methods
            
                //-------------------------------------------------------------
                /**
                 * Reads the content of a specified file.
                 *
                 * @param   szFielName  the name of the file to read
                 * @return  the content of the specified file
                 */
                static const std::string ReadFile(const char* szFileName)
                {
                    std::ifstream inputStream(szFileName);
                    
                    return string( ( std::istreambuf_iterator<char>(inputStream) ) ,
                                   ( std::istreambuf_iterator<char>(           ) ) );
                }
                //-------------------------------------------------------------
            
        private:
            
            //-----------------------------------------------------------------
            // private constructors
            
                //-------------------------------------------------------------
                /**
                 * Constructs this object as a default object.
                 */
                CUtilityFile() = delete;
                //-------------------------------------------------------------
            
    };
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
