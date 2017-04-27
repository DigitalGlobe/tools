//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc.  All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
// 
// Description:
// Main interface cutting and outputting images.
//
//*************************************************************************
// $Id: ossimQtIgenController.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtIgenController_HEADER
#define ossimQtIgenController_HEADER

#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimROIEventListener.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <QtCore/QObject>
#include "ossimQtRoiRectAnnotator.h"

class ossimQtIgenDialog;
class ossimObjectDestructingEvent;
class ossimROIEvent;
class ossimQtScrollingImageWidget;
class QString;
class ossimProjection;
class ossimMapProjection;
class ossimImageFileWriter;
class ossimConnectableObject;

class ossimQtIgenController : public QObject,
                              public ossimConnectableObjectListener,
                              public ossimROIEventListener
{
   Q_OBJECT
public:
   
   ossimQtIgenController(ossimQtIgenDialog* dialog);
   virtual ~ossimQtIgenController();

   void setImageWidget(ossimQtScrollingImageWidget* widget);
   void setView(const ossimObject* view);
   void setOutputFile(const QString& file);
   void setOutputType(const QString& type);   
   void saveSpecFile(const QString& file);
   void runIgen();
   void gsdLineEditReturnPressed();
   void linesLineEditReturnPressed();
   void samplesLineEditReturnPressed();
   void outputFileLineEditReturnPressed();
   void setSceneBoundingRect();
   void editWriter();
   
   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);
   virtual void handleRectangleROIEvent( ossimROIEvent& event );

   static ossimFilename getLatestOutputFilePath();

private:

   /**
    * Removes theOutputFile the file putting up a QMessageBox.
    */
   void removeFile() const;
   
   /**
    * Performs a system call to igen.
    */
   void runBatchIgen();

   /**
    * @retun A duplicate of obj.
    *
    * @note User is responsible for deleting returned object pointer.
    */
   ossimConnectableObject* duplicate(const ossimConnectableObject* obj) const;

   /**
    * Finds all ossimViewInterfaces in the container and sets their view
    * to "theView".
    */
   void setContainerView(ossimConnectableObject* container);

   /**
    * Sets the widget roi and ensures the data members "theLines" and
    * "theSamples" are updated.
    */
   void setWidgetRect(const ossimIrect& rect);

   /**
    * Updates theOutputGeoPolygon from theWidget rect and the view.
    */
   void updateOutputGrect();

   /**
    * Updates widget roi from theOutputGeoPolygon after a view change.
    */
   void updateRoiRect();

   /**
    * Recalculates rect from existing widget rect center, gsd, lines, and
    * samples.  Then calls updateOutputGrect.
    */
   void recalculateRect();

   /**
    * Checks the input chain fed to the writer for outputFile.
    * This is used to ensure user has not selected an image that is used to
    * feed the input for the output file; hence, destroying the input chains
    * file.
    *
    * @param outputFile User selected output file name.
    *
    * @return true if outputFile is feeding the chain fed to the writer, false
    * if not.
    */
   bool isInChain(const ossimFilename& outputFile) const;

   void         updateDialog();
   void         refresh()         const;
   ossim_uint32 getLines()        const;
   ossim_uint32 getSamples()      const;
   QString      getWriterString() const;
   QString      getOutputFile()   const;

   /**
    * This will set theOutputImageField to that of the writer output filename
    * provided it is not empty and not the same as the source file.
    */
   void updateOutputFilenameFromWriter();

   // Disallow copy  constructor, assignment operator.
   ossimQtIgenController( const ossimQtIgenController&);
   ossimQtIgenController& operator=( const ossimQtIgenController&);
   
   ossimQtIgenDialog*           theDialog;
   ossimQtScrollingImageWidget* theWidget;
   ossimRefPtr<ossimProjection> theWindowView;
   ossimRefPtr<ossimMapProjection> theOutputView;
   ossimRefPtr<ossimImageFileWriter> theWriter;
   ossimQtRoiRectAnnotator      theAnnotator;
   ossimDpt                     theGsd;
   ossim_uint32                 theLines;
   ossim_uint32                 theSamples;
   ossimFilename                theOutputFile;
   ossimGeoPolygon              theOutputGeoPolygon;
   bool                         theCallBackDisabled;

   // Static data member to capture the last directory.
   static ossimFilename theLatestOutputFilePath;


public slots:
   virtual void viewChanged(const ossimObject* viewObj);

   /**
    * Slot connected to the progress dialogs cancel clicked signal.
    */
   virtual void abortClicked();
   
};

#endif /* #ifndef ossimQtIgenRemapperController_HEADER */

