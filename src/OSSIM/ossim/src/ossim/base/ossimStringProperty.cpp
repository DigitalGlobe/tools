//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: MIT see top level license.txt
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimStringProperty.cpp 23666 2015-12-14 20:01:22Z rashadkm $
#include <ossim/base/ossimStringProperty.h>
#include <algorithm>
RTTI_DEF1(ossimStringProperty, "ossimStringProperty", ossimProperty);

ossimStringProperty::ossimStringProperty(const ossimString& name,
                                         const ossimString& value,
                                         bool editableFlag,
                                         const std::vector<ossimString>& constraintList)
   :ossimProperty(name),
    theValue(value),
    theEditableFlag(editableFlag),
    theConstraints(constraintList)
{
}

ossimStringProperty::ossimStringProperty(const ossimStringProperty& rhs)
   :ossimProperty(rhs),
    theValue(rhs.theValue),
    theEditableFlag(rhs.theEditableFlag),
    theConstraints(rhs.theConstraints)
{
}

ossimObject* ossimStringProperty::dup()const
{
   return new ossimStringProperty(*this);
}

const ossimProperty& ossimStringProperty::assign(const ossimProperty& rhs)
{
   ossimProperty::assign(rhs);
   
   ossimStringProperty* rhsPtr = PTR_CAST(ossimStringProperty,
					  &rhs);
   
   theValue        = rhs.valueToString();
   
   if(rhsPtr)
     {
       theEditableFlag = rhsPtr->theEditableFlag;
       theConstraints  = rhsPtr->theConstraints;
     }
   
   return *this;
}


void ossimStringProperty::setEditableFlag(bool flag)
{
   theEditableFlag = flag;
}

bool ossimStringProperty::getEditableFlag()const
{
   return theEditableFlag;
}

bool ossimStringProperty::isEditable()const
{
   return (getEditableFlag() == true);
}


void ossimStringProperty::clearConstraints()
{
   theConstraints.clear();
}

void ossimStringProperty::setConstraints(const std::vector<ossimString>& constraintList)
{
   theConstraints = constraintList;
}

void ossimStringProperty::addConstraint(const ossimString& value)
{
   theConstraints.push_back(value);
}

const std::vector<ossimString>& ossimStringProperty::getConstraints()const
{
   return theConstraints;
}

bool ossimStringProperty::hasConstraints()const
{
   return (theConstraints.size() > 0);
}

bool ossimStringProperty::setValue(const ossimString& value)
{
   bool result = true;
   
   if(theConstraints.size() > 0)
   {
      if(std::find(theConstraints.begin(),
                   theConstraints.end(),
                   value)
                    != theConstraints.end())
      {
         theValue = value;
      }
      else
      {
         result = false;
      }
   }
   else
   {
      theValue = value;
   }

   return result;
}

void ossimStringProperty::valueToString(ossimString& valueResult)const
{
   valueResult = theValue;
}
