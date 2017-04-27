//-----------------------------------------------------------------------------
// File:  ossimjni.i
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: SWIG Module to build ossim-jni java bindings.
//
//-----------------------------------------------------------------------------
// $Id: ossimjni.i 22506 2013-12-09 19:40:49Z dburken $

// Start ossimJniWrappers

%module ossimJniWrappers
%{

#include <ossimjni/Chipper.h>
#include <ossimjni/Constants.h>
#include <ossimjni/ElevMgr.h>
#include <ossimjni/ImageData.h>
#include <ossimjni/ImageUtil.h>
#include <ossimjni/Info.h>
#include <ossimjni/Init.h>
#include <ossimjni/Keywordlist.h>
#include <ossimjni/KeywordlistIterator.h>
#include <ossimjni/SingleImageChain.h>
#include <ossimjni/StringPair.h>

#include <exception>
#include <vector>

%}

//---
// Handle any std::exception including ossimException.
// Note: Must be placed before any methods or libraries to be wrapped:
//---
%include <exception.i>
%exception
{
   try
   {
      $action
   }
   catch (const std::exception& e)
   {
      SWIG_exception(SWIG_RuntimeError, e.what());
   }
}
   
//---
// This tells SWIG to treat char ** as a special case when used as a parameter
//in a function call.
//---
%typemap(in) char ** (jint size) 
{
   int i = 0;
   size = (*jenv)->GetArrayLength(jenv, $input);
   $1 = (char **) malloc((size+1)*sizeof(char *));
   /* make a copy of each string */
   for (i = 0; i<size; i++)
   {
      jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, $input, i);
      const char * c_string = (*jenv)->GetStringUTFChars(jenv, j_string, 0);
      $1[i] = malloc((strlen(c_string)+1)*sizeof(char));
      strcpy($1[i], c_string);
      (*jenv)->ReleaseStringUTFChars(jenv, j_string, c_string);
      (*jenv)->DeleteLocalRef(jenv, j_string);
   }
   $1[i] = 0;
}

// This cleans up the memory we malloc'd before the function call.
%typemap(freearg) char **
{
   int i;
   for (i=0; i<size$argnum-1; i++)
      free($1[i]);
   free($1);
}

// This allows a C function to return a char ** as a Java String array.
%typemap(out) char **
{
   int i;
   int len=0;
   jstring temp_string;
   const jclass clazz = (*jenv)->FindClass(jenv, "java/lang/String");
   
   while ($1[len]) len++;    
   jresult = (*jenv)->NewObjectArray(jenv, len, clazz, NULL);
   /* exception checking omitted */
   
   for (i=0; i<len; i++) {
      temp_string = (*jenv)->NewStringUTF(jenv, *result++);
      (*jenv)->SetObjectArrayElement(jenv, jresult, i, temp_string);
      (*jenv)->DeleteLocalRef(jenv, temp_string);
   }
}

// These 3 typemaps tell SWIG what JNI and Java types to use.
%typemap(jni) char ** "jobjectArray"
%typemap(jtype) char ** "String[]"
%typemap(jstype) char ** "String[]"

// These 2 typemaps handle the conversion of the jtype to jstype typemap type and vice versa.
%typemap(javain) char ** "$javainput"
%typemap(javaout) char **
{
   return $jnicall;
}

%include <std_vector.i>
%include <std_string.i>
%include <typemaps.i>
%include <arrays_java.i>
%include <various.i>

// Handle Init::initialize arg:
%apply char **STRING_ARRAY { char* argv[] };

// Basic primatives:
%include <ossimjni/Constants.h>

%apply short[] {ossimjni_sint16 *};
%apply unsigned short[] {ossimjni_uint16 *};
%apply int[] {ossimjni_sint32 *};
%apply int[] {ossimjni_int32 *};
%apply int[] {int *};
%apply unsigned int[] {ossimjni_uint32 *};
%apply float[] {ossimjni_float32 *};
%apply double[] {ossimjni_float64 *};
%apply unsigned char[]  { ossimjni_uint8 * }
%apply char* BYTE  { ossimjni_int8 * }

// Rename overloaded operators to squash swig warnings :
%rename(assign) operator=;

namespace std
{
   %template(StringVector) vector<std::string>;
   %template(UintVector) vector<ossimjni_uint32>;
}

// Used for java class generation.
%include <ossimjni/Chipper.h>
%include <ossimjni/ElevMgr.h>
%include <ossimjni/ImageData.h>
%include <ossimjni/ImageUtil.h>
%include <ossimjni/Info.h>
%include <ossimjni/Init.h>
%include <ossimjni/Keywordlist.h>
%include <ossimjni/KeywordlistIterator.h>
%include <ossimjni/SingleImageChain.h>
%include <ossimjni/StringPair.h>

%pragma(java) jniclasscode=
%{
    static
    {
       try
       {
          System.loadLibrary("ossimjni-swig");
       }
       catch (UnsatisfiedLinkError e)
       {
          System.err.println("Native code library failed to load. \n" + e);
          System.exit(1);
       }
    }
%}

