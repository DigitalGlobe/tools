//-----------------------------------------------------------------------------
//
// TestException.h
//
//-----------------------------------------------------------------------------

#pragma once

#include <exception>
#include <string>

//-----------------------------------------------------------------------------
/**
 * The <code>CTestException</code> class represents exceptions thrown to
 * indicate test failures.
 */
class CTestException : public std::exception
{
    public:

        //---------------------------------------------------------------------
        // constructors

            CTestException();
            CTestException(const char* szMessage);
            CTestException(const CTestException& ex);

        //---------------------------------------------------------------------
        // destructor

            virtual ~CTestException();

        //---------------------------------------------------------------------
        // properties

            const char* GetMessage() const;

        //---------------------------------------------------------------------
        // overridden/implemented methods

            CTestException&     operator=(const CTestException& ex);
            virtual const char* what() const throw();

    private:

        //---------------------------------------------------------------------
        // fields

            //-----------------------------------------------------------------
            /** the message of this exception */
            std::string m_sMessage;
            //-----------------------------------------------------------------

};
//-----------------------------------------------------------------------------
