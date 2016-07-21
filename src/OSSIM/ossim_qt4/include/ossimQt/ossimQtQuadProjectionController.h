//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
// 
// Description:
//
// Controller for quad projection dialog box.
//
//*************************************************************************
// $Id: ossimQtQuadProjectionController.h 12109 2007-12-04 18:09:45Z gpotts $
#ifndef ossimQtQuadProjectionController_HEADER
#define ossimQtQuadProjectionController_HEADER 

#include <QtCore/QObject>
#include <ossim/base/ossimConstants.h>

class ossimQtQuadProjectionDialog;
class ossimQtScrollingImageWidget;
class ossimQtImageInfoController;
class ossimDatum;
class ossimDpt;
class ossimGpt;
class ossimImageChain;
class ossimImageHandler;
class ossimQtMouseEvent;
class ossimKeywordlist;
class ossimString;
class ossimImageViewTransform;
class ossimProjection;


class ossimQtQuadProjectionController : public QObject
{
Q_OBJECT
      
public:

   enum Column
   {
      LINE_COLUMN      = 0,
      SAMPLE_COLUMN    = 1,
      LATITUDE_COLUMN  = 2,
      LONGITUDE_COLUMN = 3,
      HEIGHT_COLUMN    = 4
   };
   
   ossimQtQuadProjectionController(ossimQtQuadProjectionDialog* dialog);
   virtual ~ossimQtQuadProjectionController();

   void setImageWidget(ossimQtScrollingImageWidget* widget,
                       ossim_uint32 widget_layer);

   void reset();
   void save();
   void deleteRow() const;
   void addRow() const;
   void cellValueChanged(int row, int col) const;

   /**
    * If true mouse events will be ignored.
    */
   void enableRoaming(bool flag);
      
private:
   ossimQtQuadProjectionController( const ossimQtQuadProjectionController&);
   ossimQtQuadProjectionController& operator=( const ossimQtQuadProjectionController&);

   void initializeDialog() const;
   void buildTable() const;
   void buildDatumMenu() const;
   void clearDialog() const;
   bool updateDialogFromImageHandler() const;
   
   void setDatumMenu(const ossimKeywordlist& kwl) const;
   void setDatumMenu(const ossimString& code) const;
   void setTableRow(int row, const ossimGpt& gp, const ossimDpt& dp) const;
   void setTableRow(int row, const ossimGpt& gp) const;
   void setTableRow(int row, const ossimDpt& dp) const;

   void sendEmptyColumnMessage(int row,
                               ossimQtQuadProjectionController::Column col) const;
   
   const ossimDatum*        getDatum()              const;
   ossimImageHandler*       getImageHandler()       const;
   ossimImageChain*         getChain()              const;
   ossimImageViewTransform* getImageViewTransform() const;
   ossimProjection*         getImageProjection()    const;

   double getDegrees(const ossimString& s, bool lat_flag) const;
   
   ossimQtQuadProjectionDialog* theDialog;
   ossimQtScrollingImageWidget* theWidget;
   ossim_uint32                 theWidgetLayer;
   bool                         theRoamingFlag;

public slots:

   /**
    * Slot to hook up to the widget's scrollingImageWidgetMouseEvent.
    */
   virtual void trackScrollingImageWidget(ossimQtMouseEvent* evt);
};

#endif /* #ifndef ossimQtQuadProjectionController_HEADER */
