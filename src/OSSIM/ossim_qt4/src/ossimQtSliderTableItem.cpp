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
// $Id: ossimQtSliderTableItem.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <QtGui/QSlider>
#include <Qt3Support/Q3Table>
#include <QtGui/QStyle>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <ossimQt/ossimQtSliderTableItem.h>

#include <iostream>

QSlider* ossimQtSliderTableItem::theFakeSlider=0;

ossimQtSliderTableItem::ossimQtSliderTableItem(Q3Table * table, EditType et, const QString & text)
  :Q3TableItem(table, et, text)
{
  theMinValue = 0;
  theMaxValue = 99;
  theOrientation = Qt::Horizontal;
  theCurrentValue = 0;
  theTrackingFlag = true;
  setReplaceable( FALSE );
  if (!theFakeSlider ) 
    {
      QWidget* fakeWidget = new QWidget( 0, 0 );
      theFakeSlider       = new QSlider( fakeWidget, 0 );
      theFakeSlider->hide();
    }
  theSlider = NULL;
}

QWidget *ossimQtSliderTableItem::createEditor() const
{
    // create an editor - a combobox in our case
  ( (ossimQtSliderTableItem*)this )->theSlider = new QSlider(table()->viewport(), "qt_editor_slider" );
  theSlider->setMinValue(theMinValue);
  theSlider->setMaxValue(theMaxValue);
  theSlider->setOrientation(theOrientation);
  theSlider->setValue(theCurrentValue);
  theSlider->setTracking(theTrackingFlag);
  QObject::connect( theSlider, SIGNAL( valueChanged( int ) ), table(), SLOT( doValueChanged() ) );
  return theSlider;
}

void ossimQtSliderTableItem::paint( QPainter *p, const QColorGroup &cg,
			     const QRect &cr, bool selected )
{
    theFakeSlider->resize( cr.width(), cr.height() );
    theFakeSlider->setMinValue(theMinValue);
    theFakeSlider->setMaxValue(theMaxValue);
    theFakeSlider->setValue(theCurrentValue);
    theFakeSlider->setOrientation(theOrientation);
    theFakeSlider->setTracking(theTrackingFlag);
    p->eraseRect(theFakeSlider->rect());
    QColorGroup c( cg );
    if ( selected ) 
      {
	c.setBrush( QColorGroup::Base, cg.brush( QColorGroup::Highlight ) );
	c.setColor( QColorGroup::Text, cg.highlightedText() );
      }
    QStyle::State flags = QStyle::State_None;
    if(isEnabled() && table()->isEnabled())
    {
       flags |= QStyle::State_Enabled;
    }
    // Temp drb
    std::cout << "ossimQtSliderTableItem::paint need to fix drawComplexControl!" << std::endl;
    // table()->style()->drawComplexControl( QStyle::CC_Slider, p, theFakeSlider, theFakeSlider->rect(), c, flags );
}

QSize ossimQtSliderTableItem::sizeHint() const
{
    QSize sh = theFakeSlider->sizeHint();
    return sh.expandedTo( QApplication::globalStrut() );
}

void ossimQtSliderTableItem::setOrientation(Qt::Orientation orientation)
{
  theOrientation = orientation;
}

void ossimQtSliderTableItem::setMinValue(int minValue)
{
  theMinValue = minValue;
}

void ossimQtSliderTableItem::setMaxValue(int maxValue)
{
  theMaxValue = maxValue;
}

void ossimQtSliderTableItem::setContentFromEditor( QWidget *w )
{
  if ( w->inherits( "QSlider" ) ) 
    {
      QSlider *slider = (QSlider*)w;
      theCurrentValue = slider->value();
      theMinValue     = slider->minValue();
      theMaxValue     = slider->maxValue();
      theOrientation  = slider->orientation();
      // theTrackingFlag = slider->tracking();
      theTrackingFlag = slider->hasTracking();      
    }
}

QSlider* ossimQtSliderTableItem::slider()
{
  return theSlider;
}

void ossimQtSliderTableItem::setTrackingFlag(bool flag)
{
  theTrackingFlag = flag;
}

void ossimQtSliderTableItem::setCurrentValue(int value)
{
  theCurrentValue = value;
  table()->updateCell( row(), col() );
}
