//-----------------------------------------------------------------------------
//
// TestException.cpp
//
//-----------------------------------------------------------------------------

#include "TestException.h"

using namespace std;

//-----------------------------------------------------------------------------
/**
 * Constructs this exception as a default exception.
 */
CTestException::CTestException() : exception()
{
}
//-----------------------------------------------------------------------------
/**
 * Constructs this exception using specified parameters.
 *
 * @param   szMessage  the message to use
 */
CTestException::CTestException(const char* szMessage)
                               : exception() ,
                                 m_sMessage(szMessage)
{
}
//-----------------------------------------------------------------------------
/**
 * Constructs this exception using a specified exception.
 *
 * @param   ex  the exception from which to constructs this exception
 */
CTestException::CTestException(const CTestException& ex)
                               : exception() ,
                                 m_sMessage(ex.m_sMessage)
{
}
//-----------------------------------------------------------------------------
/**
 * Destructs this exception.
 */
CTestException::~CTestException()
{
}
//-----------------------------------------------------------------------------
/**
 * Gets the message of this exception.
 *
 * @return  the message of this exception
 */
const char* CTestException::GetMessage() const
{
    return m_sMessage.c_str();
}
//-----------------------------------------------------------------------------
/**
 * Sets the properties of this exception to equal the properties of a
 * specified exception.
 *
 * @param   ex  the exception to use
 * @return  this exception
 */
CTestException& CTestException::operator=(const CTestException& ex)
{
    if (this != &ex)
    {
        m_sMessage = ex.m_sMessage;
    }

    return *this;
}
//-----------------------------------------------------------------------------
/**
 * Gets the message of this exception.
 *
 * @return  the message of this exception
 */
const char* CTestException::what() const throw()
{
    return m_sMessage.c_str();
}
//-----------------------------------------------------------------------------
