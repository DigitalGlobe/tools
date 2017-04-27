//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt.
//
// Author:  David Burken
//
// Description: Single image window.
//
// $Id: ossimQtSingleImageWindow.h 2259 2011-03-11 17:24:01Z david.burken $
//----------------------------------------------------------------------------
#ifndef ossimQtSingleImageWindow_HEADER 
#define ossimQtSingleImageWindow_HEADER

#include <vector>
#include <QtGui/QMainWindow>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimConnectableDisplayListener.h>
// #include <ossim/imaging/ossimImageChain.h>
// #include <ossim/imaging/ossimRLevelFilter.h>
class ossimQtScrollingImageWidget;
class ossimImageChain;
class ossimImageHandler;
class ossimQtMouseEvent;
class QMenu;
class QMenuItem;
class ossimRLevelFilter;

#include "ossimQtExport.h" /** for dll export stuff */

/**
 * This window is designed to be used as a simple viewer, exporter.
 * The basic image chain has no renderer and works in image space only.
 */
class OSSIMQT_DLL ossimQtSingleImageWindow :
   public QMainWindow,
   public ossimConnectableObject,
   public ossimConnectableDisplayListener
{
   Q_OBJECT
public:

   ossimQtSingleImageWindow(QWidget*    parent=0,
                            const char* name=0,
                            Qt::WFlags  f=0);
   
   virtual ~ossimQtSingleImageWindow();

   /** Override virtual method from ossimDisplayEventListener. */
   virtual void displayListEvent(ossimDisplayListEvent&);

   /** Override virtual method from ossimDisplayEventListener. */
   virtual void displayRefreshEvent(ossimDisplayRefreshEvent&);

   /** Concrete implementation of pure virtual from ossimConnectableObject. */
   virtual bool canConnectMyInputTo(ossim_int32 /*myInputIndex*/,
                                    const ossimConnectableObject* object)const;

   /**
    * Method to listen to "theChain" for adds of objects so we can,
    * for instance, rebuild the reduced res menu if one is added.
    */
   virtual void addObjectEvent(ossimContainerEvent& event);

   /**
    * Method to listen to "theChain" for adds of objects so we can,
    * for instance, rebuild the reduced res menu if one is removed.
    */
   virtual void removeObjectEvent(ossimContainerEvent& event);
 
public slots:
   void trackImageWidget(ossimQtMouseEvent* evt);

protected:
   virtual void customEvent(QEvent* event);
   
private slots:
   
   /** Calls theImageWidget->refresh() */
   void refreshDisplay();

   /**
    * Changes resolution level if an rlevel filter is in the chain.
    * Does nothing if no rlevel filter is present.
    */
   void changeResolutionLevel( QAction* action );
   
   void openImage();
   void closeImage();
   void editImageChain();
   void exportImage();
   void closeWindow();
   void about();

private:
   // Hide from use.
   ossimQtSingleImageWindow(const ossimQtSingleImageWindow& obj);

   // Hide from use.
   const ossimQtSingleImageWindow& operator=(
      const ossimQtSingleImageWindow& rhs);

   void displayImage(const ossimFilename& file);
   void createImageChain(ossimImageHandler* ih);
   void getAllowedFilterList(std::vector<ossimString>&) const;
   void buildOverViews(ossimImageHandler* ih);

   /**
    * Builds the resolution level menu.
    */
   void buildResolutionLevelMenu();
   
   /**
    * Checks "theImageChain" for an ossimRLevelFilter and returns a pointer
    * to it.
    * 
    * @return A pointer to the ossimRLevelFilter if there is one; else, NULL.
    */
   ossimRefPtr<ossimRLevelFilter> getRLevelFilter();

   bool getImageFile(ossimFilename& file);
   
   ossimQtScrollingImageWidget* theImageWidget;
   ossimRefPtr<ossimImageChain> theImageChain;
   ossimFilename                theLastOpenedDirectory;
   QMenu*                       theResolutionLevelMenu;
   
TYPE_DATA   
};
#endif /* #ifndef ossimQtSingleImageWindow_HEADER */
