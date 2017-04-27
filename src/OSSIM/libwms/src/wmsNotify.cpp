/* -*-c++-*- libwms - Copyright (C) since 2004 Garrett Potts 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the libwms Public License (WMSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * libwms Public License for more details.
*/
#include <wms/wmsNotify.h>
#include <stdio.h>
#include <stdarg.h>
#include <stack>
#include <fstream>

static std::ostream* theWmsFatalStream  = &std::cerr;
static std::ostream* theWmsWarnStream   = &std::cerr;
static std::ostream* theWmsInfoStream   = &std::cout;
static std::ostream* theWmsNoticeStream = &std::cout;
static std::ostream* theWmsDebugStream  = &std::cout;
static std::ostream* theWmsAlwaysStream = &std::cout;

static wmsNotifyFlags theNotifyFlags  = wmsNotifyFlags_ALL;
static std::stack<wmsNotifyFlags> theNotifyFlagsStack;

template <class charT, class traits = std::char_traits<charT> >
class wmsNullBufferStream : public std::basic_streambuf<charT, traits>
{
public:
   wmsNullBufferStream(){};
   
   
protected:
   
   std::streamsize xsputn(const charT * /*pChar*/, std::streamsize /*n*/)
      {
         return 0;
      }
   
private:
   wmsNullBufferStream(const wmsNullBufferStream&);
   wmsNullBufferStream& operator=(const wmsNullBufferStream&);
};

template <class charT, class traits = std::char_traits<charT> >
class wmsLogFileBufferStream : public std::basic_streambuf<charT, traits>
{
public:
   wmsLogFileBufferStream(){};
   
   void setLogFilename(const std::string& file)
      {
         theLogFilename = file;
      }
   std::string getLogFilename()const
      {
         return theLogFilename;
      }
   
protected:
   std::string theLogFilename;
   virtual int overflow(int c)
      {
         if(!traits::eq_int_type(c, traits::eof()))
         {
            char tempStr[2];
            tempStr[0] = c;
            tempStr[1] = '\0';
            
            tempString = tempString + tempStr;
         }
         
         return c;
      }
   
   virtual std::streamsize xsputn(const charT * pChar, std::streamsize n)
      {
         tempString = tempString + std::string(pChar, pChar + n);

         return n;
      }

   virtual int sync()
      {
         if ( theLogFilename.size() && tempString.size() )
         {
            std::ofstream outFile( theLogFilename.c_str(),
                                   std::ios::app|std::ios::out );
            if (outFile.good())
            {
               outFile.write(tempString.c_str(), tempString.length());
               tempString = "";
            }
         }
         return 0;
      }
   
private:
   std::string tempString;
   
   wmsLogFileBufferStream(const wmsLogFileBufferStream&);
   wmsLogFileBufferStream& operator=(const wmsLogFileBufferStream&);
};


class wmsNullStream : public std::ostream
{
public: 
   wmsNullStream() : std::ios(0),std::ostream(&nullBufferStream){}
   virtual ~wmsNullStream() 
      {
         nullBufferStream.pubsync(); 
      }
   
private:
   wmsNullBufferStream<char> nullBufferStream; 
   // Copy & assignment are undefined in iostreams
   wmsNullStream(const wmsNullStream&);
   wmsNullStream & operator=(const wmsNullStream&);
};

class wmsLogFileStream : public std::ostream
{
public: 
   wmsLogFileStream() : std::ios(0),std::ostream(&theLogFileBufferStream){}
   virtual ~wmsLogFileStream() 
      {
         theLogFileBufferStream.pubsync(); 
      }
   void setLogFilename(const std::string& filename)
      {
         theLogFileBufferStream.setLogFilename(filename);
      }
   std::string getLogFilename()const
      {
         return theLogFileBufferStream.getLogFilename();
      }
   
private:
   wmsLogFileBufferStream<char> theLogFileBufferStream; 
   // Copy & assignment are undefined in iostreams
   wmsLogFileStream(const wmsLogFileStream&);
   wmsLogFileStream & operator=(const wmsLogFileStream&);
};

static wmsNullStream    theWmsNullStream;
static wmsLogFileStream theLogFileStream;

void wmsSetDefaultNotifyHandlers()
{
   theWmsFatalStream  = &std::cerr;
   theWmsWarnStream   = &std::cout;
   theWmsInfoStream   = &std::cout;
   theWmsNoticeStream = &std::cout;
   theWmsDebugStream  = &std::cout;
   theWmsAlwaysStream = &std::cout;
}

void wmsSetNotifyStream(std::ostream* outputStream,
                          wmsNotifyFlags whichLevelsToRedirect)
{
   if(whichLevelsToRedirect&wmsNotifyFlags_FATAL)
   {
      theWmsFatalStream = outputStream;
   }
   if(whichLevelsToRedirect&wmsNotifyFlags_WARN)
   {
      theWmsWarnStream = outputStream;
   }
   if(whichLevelsToRedirect&wmsNotifyFlags_INFO)
   {
      theWmsInfoStream = outputStream;
   }
   if(whichLevelsToRedirect&wmsNotifyFlags_NOTICE)
   {
      theWmsNoticeStream = outputStream;
   }
   if(whichLevelsToRedirect&wmsNotifyFlags_DEBUG)
   {
      theWmsDebugStream = outputStream;
   }
}

std::ostream* wmsGetNotifyStream(wmsNotifyLevel whichLevel)
{
   std::ostream* notifyStream = &theWmsNullStream;

   switch(whichLevel)
   {
      case wmsNotifyLevel_ALWAYS:
      {
         notifyStream = theWmsAlwaysStream;
         break;
      }
      case wmsNotifyLevel_FATAL:
      {
         notifyStream = theWmsFatalStream;
         break;
      }
      case wmsNotifyLevel_WARN:
      {
         notifyStream = theWmsWarnStream;
         break;
      }
      case wmsNotifyLevel_INFO:
      {
         notifyStream = theWmsInfoStream;
         break;
      }
      case wmsNotifyLevel_NOTICE:
      {
         notifyStream = theWmsNoticeStream;
         break;
      }
      case wmsNotifyLevel_DEBUG:
      {
         notifyStream = theWmsDebugStream;
         break;
      }
   }
   return notifyStream;
}

std::ostream& wmsNotify(wmsNotifyLevel level)
{
   if(wmsIsNotifyEnabled())
   {
      if(theLogFileStream.getLogFilename() != "")
      {
         return theLogFileStream;
      }
      else
      {
         bool reportMessageFlag = false;
         switch(level)
         {
            case wmsNotifyLevel_ALWAYS:
            {
               reportMessageFlag = true;
               break;
            }
            case wmsNotifyLevel_FATAL:
            {
               if(theNotifyFlags&wmsNotifyFlags_FATAL)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case wmsNotifyLevel_WARN:
            {
               if(theNotifyFlags&wmsNotifyFlags_WARN)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case wmsNotifyLevel_INFO:
            {
               if(theNotifyFlags&wmsNotifyFlags_INFO)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case wmsNotifyLevel_NOTICE:
            {
               if(theNotifyFlags&wmsNotifyFlags_NOTICE)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case wmsNotifyLevel_DEBUG:
            {
               if(theNotifyFlags&wmsNotifyFlags_DEBUG)
               {
                  reportMessageFlag = true;
               }
               break;
            }
         }
         if(reportMessageFlag)
         {
            return *wmsGetNotifyStream(level);
         }
      }
   }

   return theWmsNullStream;
}

void wmsSetLogFilename(const std::string& filename)
{
   theLogFileStream.setLogFilename(filename);
}

const char* wmsErrorV(const char *fmt, va_list args )
{  
   static char temp[2024];
   if(fmt)
   {
      vsprintf(temp, fmt, args);
   }
   else
   {
      sprintf(temp,"%s", "");
   }

   return temp;
}

void wmsEnableNotify(wmsNotifyFlags flags)
{
   theNotifyFlags = (wmsNotifyFlags)(theNotifyFlags | flags);
}

void wmsDisableNotify(wmsNotifyFlags flags)
{
   theNotifyFlags = (wmsNotifyFlags)((wmsNotifyFlags_ALL^flags)&
                                       theNotifyFlags);
}

void wmsSetNotifyFlag(wmsNotifyFlags notifyFlags)
{
   theNotifyFlags = notifyFlags;
}

void wmsPushNotifyFlags()
{
   theNotifyFlagsStack.push(theNotifyFlags);
}

void wmsPopNotifyFlags()
{
   if(theNotifyFlagsStack.empty())
   {
      return;
   }
   theNotifyFlags = theNotifyFlagsStack.top();
   theNotifyFlagsStack.pop();
}

wmsNotifyFlags wmsGetNotifyFlags()
{
   return theNotifyFlags;
}



bool wmsIsNotifyEnabled()
{
   return  (theNotifyFlags != wmsNotifyFlags_NONE);
}


