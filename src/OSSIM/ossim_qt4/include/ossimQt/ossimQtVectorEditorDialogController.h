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
// $Id: ossimQtVectorEditorDialogController.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtVectorEditorDialogController_HEADER
#define ossimQtVectorEditorDialogController_HEADER
#include <QtCore/QObject>
#include <vector>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimFontInformation.h>

class ossimQtVectorEditorDialog;
class ossimVpfTileSource;
class ossimConnectableObject;
class ossimVpfAnnotationFeatureInfo;

enum ossimQtVectorEditorFeatureInfoType
  {
    ossimQtVectorEditorFeatureInfoType_UNKNOWN = 0,
    ossimQtVectorEditorFeatureInfoType_POINT   = 1,
    ossimQtVectorEditorFeatureInfoType_LINE    = 2,
    ossimQtVectorEditorFeatureInfoType_POLYGON = 3,
    ossimQtVectorEditorFeatureInfoType_TEXT    = 4,
  };

class ossimQtVectorEditorFeatureInfo
{
public:

  ossimQtVectorEditorFeatureInfo()
  {
  }
  ossimString                        theFeatureId;
  ossimQtVectorEditorFeatureInfoType theFeatureType;
  ossimRgbVector                     theColor;
  ossimDpt                           thePointRadius;
  ossim_uint32                       theThickness;
  bool                              theFillFlag;
  bool                              theEnabledFlag;

  ossimFontInformation               theFontInformation;
};

class ossimQtVectorEditorDialogController: public QObject
{
  Q_OBJECT

public:

  ossimQtVectorEditorDialogController();
  ossimQtVectorEditorDialogController(ossimQtVectorEditorDialog* dialog);
  virtual ~ossimQtVectorEditorDialogController();

  bool setObject(ossimConnectableObject* obj);

  void changeToFeature(ossim_int32 idx);
  
protected:
  ossimQtVectorEditorDialog*                  theDialog;
  ossimConnectableObject*                     theVectorSource;
  ossim_int32                                 theCurrentFeatureIdx;
  
   std::vector<ossimVpfAnnotationFeatureInfo*> theVpfFeatureList;
   
   std::vector<ossimQtVectorEditorFeatureInfo> theFeatureInfoList;
   mutable bool                                theTransferingToDialogFlag;
  
  void initialize();

private:
   void initializeVpfTileSource();
   void applyVpfTileSource();
   
   void transferListToDialog();
   void transferCurrentToDialog();
   void populateStyleWithCurrent();
   void populateFamilyNames();

public slots:
  virtual void apply();
  virtual void pointColor();
  virtual void lineColor();
  virtual void polygonColor();
  virtual void fontColor();

  virtual void pointEnabledToggled();
  virtual void lineEnabledToggled();
  virtual void polygonEnabledToggled();
  virtual void fontEnabledToggled();

   virtual void lineThicknessChanged();
   virtual void polygonThicknessChanged();
   virtual void pointFilledToggled();
   virtual void polygonFilledToggled();
   virtual void fontFamilyName();
   virtual void fontStyle();
   virtual void fontPointSize();
   virtual void fontRotation();
   virtual void fontShear();
   virtual void fontScale();

   virtual void disableAll();
   virtual void enableAll();
   
   void transferFromDialogCurrent();
};

#endif

