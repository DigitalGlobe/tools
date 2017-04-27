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
// $Id: ossimQtPropertyColorItem.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <Qt3Support/Q3HBox>
#include <QtCore/QVariant>
#include <QtGui/QPainter>
#include <QtGui/QColorDialog>
//Added by qt3to4:
#include <Qt3Support/Q3Frame>
#include <ossimQt/ossimQtPropertyListView.h>
#include <ossimQt/ossimQtPropertyColorItem.h>
#include <ossimQt/ossimQtPropertyFactory.h>
#include <ossim/base/ossimColorProperty.h>
#include <ossim/base/ossimNumericProperty.h>

ossimQtPropertyColorItem::ossimQtPropertyColorItem(ossimQtPropertyListView *propList,
                                                   ossimQtPropertyItem *after,
                                                   ossimQtPropertyItem *parent,
                                                   ossimRefPtr<ossimProperty> oProp)
   :ossimQtPropertyItem(propList, after, parent, oProp),
    theBox(NULL),
    theColorPreview(NULL),
    theButton(NULL)
{
   theBox = new Q3HBox( theListView->viewport() );
   theBox->hide();
   theColorPreview = new Q3Frame( theBox );
   theButton = new QPushButton( "...", theBox );
   theButton->setFixedWidth( 20 );
   theBox->setFrameStyle( Q3Frame::StyledPanel | Q3Frame::Sunken );
   theBox->setLineWidth( 2 );
   theColorPreview->setFrameStyle( Q3Frame::Plain | Q3Frame::Box );
   theColorPreview->setLineWidth( 2 );
   
   QPalette pal = theColorPreview->palette();
   QColorGroup cg = pal.active();
   cg.setColor( QColorGroup::Foreground, cg.color( QColorGroup::Base ) );
   pal.setActive( cg );
   pal.setInactive( cg );
   pal.setDisabled( cg );
   theColorPreview->setPalette( pal );

   connect( theButton, SIGNAL( clicked() ),
            this, SLOT( getColor() ) );
}

ossimQtPropertyColorItem::~ossimQtPropertyColorItem()
{
   if(theBox)
   {
      delete theBox;
      theBox = NULL;
   }
}

void ossimQtPropertyColorItem::createChildren()
{

   if(childCount() > 0 ) return;
   
   ossimQtPropertyItem *i = this;

   ossimColorProperty* colorProperty = PTR_CAST(ossimColorProperty,
                                                getOssimProperty().get());

   if(colorProperty)
   {
      ossimNumericProperty* redProperty   = new ossimNumericProperty("Red");
      ossimNumericProperty* greenProperty = new ossimNumericProperty("Green");
      ossimNumericProperty* blueProperty  = new ossimNumericProperty("Blue");
      
      redProperty->setConstraints(0.0, 255.0);
      greenProperty->setConstraints(0.0, 255.0);
      blueProperty->setConstraints(0.0, 255.0);
      redProperty->setValue(ossimString::toString(colorProperty->getRed()));
      greenProperty->setValue(ossimString::toString(colorProperty->getGreen()));
      blueProperty->setValue(ossimString::toString(colorProperty->getBlue()));
      
      
      i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                           i,
                                                           this,
                                                           redProperty);
      addChild(i);
      if(i)
      {
         i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                              i,
                                                              this,
                                                              greenProperty);
         
         addChild(i);
         i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                              i,
                                                              this,
                                                              blueProperty);
         addChild(i);
      }
   }
}

void ossimQtPropertyColorItem::initChildren()
{
   ossimQtPropertyItem *item = 0;
   ossimColorProperty* colorProperty = PTR_CAST(ossimColorProperty,
                                                getOssimProperty().get());

   if(!colorProperty) return;
   
   for ( int i = 0; i < childCount(); ++i )
   {
      item = ossimQtPropertyItem::child( i );
      if(item->getOssimProperty().valid())
      {
         if ( item->getOssimProperty()->getName() == "Red")
         {
            item->setValue( QString(ossimString::toString(colorProperty->getRed()).c_str()));
         }
         else if ( item->getOssimProperty()->getName() == "Green")
         {
            item->setValue( QString(ossimString::toString(colorProperty->getGreen()).c_str()));
         }
         else if ( item->getOssimProperty()->getName() == "Blue")
         {
            item->setValue( QString(ossimString::toString(colorProperty->getBlue()).c_str()));
         }
      }
   }
}

void ossimQtPropertyColorItem::showEditor()
{
   ossimQtPropertyItem::showEditor();
   placeEditor( theBox );
   
   if ( !theBox->isVisible() )
   {
      theBox->show();
      theListView->viewport()->setFocus();
      ossimColorProperty* colorProperty = PTR_CAST(ossimColorProperty,
                                                   getOssimProperty().get());
      if(!colorProperty) return;
      theColorPreview->setBackgroundColor( QColor(colorProperty->getRed(),
                                                  colorProperty->getGreen(),
                                                  colorProperty->getBlue()));
   }
}

void ossimQtPropertyColorItem::hideEditor()
{
    ossimQtPropertyItem::hideEditor();
    theBox->hide();
}

bool ossimQtPropertyColorItem::hasSubItems() const
{
   return true;
}

void ossimQtPropertyColorItem::childValueChanged( ossimQtPropertyItem *child )
{
   ossimString value = child->getOssimProperty()->valueToString();
   ossimColorProperty* colorProperty = PTR_CAST(ossimColorProperty,
                                                getOssimProperty().get());
   if(!colorProperty) return;
   
   if ( child->getOssimProperty()->getName() == "Red")
   {
      colorProperty->setRed(value.toInt32());
   }
   else if ( child->getOssimProperty()->getName() == "Green" )
   {
      colorProperty->setGreen((ossim_uint8)value.toInt32());
   }
   else if ( child->getOssimProperty()->getName() == "Blue")
   {
      colorProperty->setBlue((ossim_uint8)value.toInt32());
   }
   
   theColorPreview->setBackgroundColor( QColor(colorProperty->getRed(),
                                               colorProperty->getGreen(),
                                               colorProperty->getBlue()));
   
   notifyValueChange();
}

bool ossimQtPropertyColorItem::hasCustomContents() const
{
    return TRUE;
}

void ossimQtPropertyColorItem::drawCustomContents( QPainter *p, const QRect &r )
{
   ossimColorProperty* colorProperty = PTR_CAST(ossimColorProperty,
                                                getOssimProperty().get());
   if(!colorProperty) return;
   
    p->save();
    p->setPen( QPen( Qt::black, 1 ) );
    p->setBrush( QColor(colorProperty->getRed(),
                        colorProperty->getGreen(),
                        colorProperty->getBlue()));
    p->drawRect( r.x() + 2, r.y() + 2, r.width() - 5, r.height() - 5 );
    p->restore();
}

void ossimQtPropertyColorItem::getColor()
{
   ossimColorProperty* colorProperty = PTR_CAST(ossimColorProperty,
                                                getOssimProperty().get());
   if(!colorProperty) return;

   
   QColor c = QColorDialog::getColor( QColor((int)colorProperty->getRed(),
                                             (int)colorProperty->getGreen(),
                                             (int)colorProperty->getBlue()),
                                             theListView );
   if ( c.isValid() )
   {
      colorProperty->setRed((ossim_uint8)c.red());
      colorProperty->setGreen((ossim_uint8)c.green());
      colorProperty->setBlue((ossim_uint8)c.blue());
      theColorPreview->setBackgroundColor(c);

      if(isOpen())
      {
         initChildren();
      }
      
      setText( 1, "" );
      
      notifyValueChange();
   }
}

void ossimQtPropertyColorItem::resetProperty(bool notify)
{
   ossimQtPropertyItem::resetProperty(notify);
   
   ossimColorProperty* colorProperty = PTR_CAST(ossimColorProperty,
                                                getOssimProperty().get());
   if(!colorProperty) return;
   
   theColorPreview->setBackgroundColor( QColor(colorProperty->getRed(),
                                               colorProperty->getGreen(),
                                               colorProperty->getBlue()));
   
   
   if(isOpen())
   {
      for ( int i = 0; i < childCount(); ++i )
      {
         child(i)->resetProperty(false);
      }
      
      initChildren();
   }
   
}

