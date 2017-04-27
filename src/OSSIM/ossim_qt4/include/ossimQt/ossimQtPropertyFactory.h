//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimQtPropertyFactory.h 9505 2006-09-02 20:36:55Z dburken $
#ifndef ossimQtPropertyFactory_HEADER
#define ossimQtPropertyFactory_HEADER

class ossimQtPropertyListView;
class ossimQtPropertyItem;
class ossimProperty;

class ossimQtPropertyFactory
{
public:
   static ossimQtPropertyFactory* instance();
   /*!
    * It will create a QtProperty and take ownership
    * of the passed in property.  Returns NULL
    * if no property could hanle the passed in property
    */
   ossimQtPropertyItem* createProperty(ossimQtPropertyListView *propList,
                                       ossimQtPropertyItem *after,
                                       ossimQtPropertyItem *parent,
                                       ossimRefPtr<ossimProperty> oProp)const;
   
protected:
   ossimQtPropertyFactory();
   
   static ossimQtPropertyFactory* theInstance;

private:
   ossimQtPropertyFactory(const ossimQtPropertyFactory& rhs);
   const ossimQtPropertyFactory& operator = (const ossimQtPropertyFactory& rhs);

};

#endif
