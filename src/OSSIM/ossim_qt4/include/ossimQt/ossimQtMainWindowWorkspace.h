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
// $Id: ossimQtMainWindowWorkspace.h 9114 2006-06-14 17:42:07Z gpotts $
#ifndef ossimQtMainWindowWorkspace_HEADER
#define ossimQtMainWindowWorkspace_HEADER
#include <QtGui/QWorkspace>
#include <ossim/base/ossimKeywordlist.h>

class ossimQtMainWindowWorkspace : public QWorkspace
{
  Q_OBJECT
public:
    ossimQtMainWindowWorkspace(QWidget* widget=0, const char* name=0)
	    :QWorkspace(widget, name)
	    {}

   virtual void sendEventToAll(QEvent* evt);
   
public slots:
 void minimizeAll();
  void restoreAll();
  void closeAll();
  void refreshAll();
};

#endif
