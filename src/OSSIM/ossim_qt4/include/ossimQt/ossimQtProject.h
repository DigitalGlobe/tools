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
// $Id: ossimQtProject.h 9114 2006-06-14 17:42:07Z gpotts $
#ifndef ossimQtProject_HEADER
#define ossimQtProject_HEADER
#include <QtCore/QObject>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>

class ossimQtMainWindowWorkspace;
class QWidget;
class ossimDataManager;

class ossimQtProject : public QObject
{
public:
   ossimQtProject(QWidget* parent);
   virtual ~ossimQtProject();
   
   ossimDataManager* getDataManager()
      {
         return theDataManager;
      }
   ossimQtMainWindowWorkspace* getWorkspace()
      {
         return theWorkspace;
      }
   virtual void show();
   virtual void hide();
   ossimFilename  getFilename()const;
   void setFilename(const ossimFilename& filename);
   virtual bool save();
   virtual bool close();
   virtual bool open(const ossimFilename& project);
   virtual void setDirtyFlag(bool flag=true);
   virtual bool isDirty()const;
  virtual ossimString getDescription()const
  {
    return theDescription;
  }
  virtual void setDescription(const ossimString& description)
  {
    theDescription = description;
  }

  static const ossimString EXTENSION;
protected:
   static int theUnamedFileCounter;
   mutable bool       theDirtyFlag;
   ossimDataManager*           theDataManager;
   ossimQtMainWindowWorkspace* theWorkspace;
   ossimFilename               theFilename;
   ossimString                 theDescription;
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const ossimString& prefix)const;
   virtual bool loadState(ossimKeywordlist& kwl,
                          const ossimString& prefix);
   
};

#endif
