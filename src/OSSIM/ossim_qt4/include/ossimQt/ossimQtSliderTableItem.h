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
// $Id: ossimQtSliderTableItem.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtSliderTableItem_HEADER
#define ossimQtSliderTableItem_HEADER
#include <QtCore/QString>

class QSlider;
class QWidget;
class ossimQtSliderTableItem : public Q3TableItem
{
public:
  ossimQtSliderTableItem(Q3Table * table, EditType et, const QString & text="");
  virtual void paint ( QPainter * p, const QColorGroup & cg, const QRect & cr, bool selected );
  virtual QWidget *createEditor() const;
  virtual QSize sizeHint() const;
  void setOrientation(Qt::Orientation orientation);
  int getCurrentValue()const
  {
    return theCurrentValue;
  }
  int getMinValue()const
  {
    return theMinValue;
  }
  int getMaxValue()const
  {
    return theMaxValue;
  }
  Qt::Orientation getOrientation()const
  {
    return theOrientation;
  }
  bool getTrackingFlag()const
  {
    return theTrackingFlag;
  }
  void setMinValue(int min);
  void setMaxValue(int max);
  void setTrackingFlag(bool flag);
  void setCurrentValue(int value);
  virtual void setContentFromEditor( QWidget *w );
  
  QSlider* slider();
protected:
  static QSlider* theFakeSlider;
  QSlider* theSlider;
  int  theMinValue;
  int  theMaxValue;
  Qt::Orientation theOrientation;
  bool           theTrackingFlag;
  int            theCurrentValue;

};

#endif
