/*

*/

%module pyossim

%{
#include <ossim/base/ossimString.h>
%}
%rename(__set__) ossimKeyword::operator=;
%rename(const_operator_char_ptr) ossimKeyword::operator const char*;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimKeyword.h"



