//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtPropertyItem.h 12122 2007-12-06 13:27:09Z gpotts $
#ifndef ossimQtPropertyItem_HEADER
#define ossimQtPropertyItem_HEADER
#include <QtCore/QObject>
#include <Qt3Support/Q3PtrList>
#include <Qt3Support/Q3ListView>
#include <QtGui/QColor>
#include <QtGui/QPushButton>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimRefPtr.h>

class ossimPropertyInterface;
class ossimQtPropertyListView;
class ossimQtPropertyItem : public QObject,
                            public Q3ListViewItem
{
   Q_OBJECT
public:
   ossimQtPropertyItem(ossimQtPropertyListView *propList,
                       ossimQtPropertyItem *after,
                       ossimQtPropertyItem *parent,
                       ossimRefPtr<ossimProperty> oProp);
   virtual ~ossimQtPropertyItem();
   virtual ossimRefPtr<ossimProperty> getOssimProperty();
   virtual const ossimRefPtr<ossimProperty> getOssimProperty()const;
   virtual void notifyValueChange();

   virtual int getIndentation()const;
   virtual ossimQtPropertyListView* getListView();
   bool isOpen() const;
   virtual void setOpen(bool b );
   virtual ossimQtPropertyItem *propertyParent() const;
   virtual bool hasSubItems() const;
   virtual void showEditor();
   virtual void hideEditor();
   virtual void placeEditor( QWidget *w );
   void setChanged(bool b);
   bool isChanged()const;
   void createResetButton();
   void updateResetButtonState();
   void addChild( ossimQtPropertyItem *i );
   int childCount() const;
   ossimQtPropertyItem *child( int i );
   ossimQtPropertyItem* getRootItem();
   
   virtual void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int align );
   virtual bool hasCustomContents() const;
   virtual void drawCustomContents( QPainter *p, const QRect &r );
   virtual void paintBranches( QPainter * p, const QColorGroup & cg,
                               int w, int y, int h );
   virtual void paintFocus( QPainter *p, const QColorGroup &cg, const QRect &r );
   void updateBackgroundColor();
   
   virtual void childValueChanged( ossimQtPropertyItem * );
   virtual void notifyResetStateChanged();
   virtual void createChildren();
   virtual void initChildren();
   virtual void setText(int col, const QString &t );
   void setup ();
   virtual void setValue(const QVariant& v);
   virtual QVariant value()const;
   virtual void toggle();

   /**
    * Sets "theMaker" which is the object that made "theProperty".
    */
   virtual void setMaker(ossimPropertyInterface* maker);

   /**
    * Derived classes should implement.  This is just a stub.  After all
    * derived classes are implemented this should probably be a pure
    * virtual.
    */
   virtual void updateGUI();

   static const int INDENTATION_AMOUNT;
   
protected:
   ossimQtPropertyListView*        theListView;
   ossimQtPropertyItem            *theParent;
   ossimRefPtr<ossimProperty>     theProperty;
   ossimRefPtr<ossimProperty>     theResetProperty;
   bool                            theOpenFlag;
   bool                            theChangedFlag;
   QColor                          theBackgroundColor;
   Q3PtrList<ossimQtPropertyItem>   theChildren;
   QPushButton                    *theResetButton;
   bool                            theOwnsPropertyFlag;

   /**
    * This is the object that made "theProperty".
    * 
    * This could be a ossimRefPtr maybe???
    */
   ossimPropertyInterface*         theMaker;
   
   virtual QColor backgroundColor();

public slots:
   virtual void resetProperty(bool notify = true);
};

#endif

