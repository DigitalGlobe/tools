//*******************************************************************
//
// License:   See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtImageWindowController.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtImageWindowController_HEADER
#define ossimQtImageWindowController_HEADER

#include <QtCore/QObject>

#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/projection/ossimImageProjectionModel.h>


class QCloseEvent;
class QEvent;

class ossimConnectableObject;
class ossimQtMouseEvent;
class ossimObject;
class ossimQtImageWindow;
class ossimQtSwipeScrollingImageWidget;
class ossimQtImageControllerZoomEventFilter;
class ossimQtImageControllerOssimEventCapture;
class ossimQtImageControllerCustomEventFilter;
class ossimQtImageControllerTrackMouseEventFilter;
class ossimQtImageControllerPanEventFilter;
class ossimQtIgenDialog;
class ossimQtInformationDialog;
class ossimQtPositionQualityDialog;
class ossimHistogramRemapper;
class ossimImageHandler;
class ossimImageRenderer;
class ossimIpt;
class ossimKeywordlist;
class ossimRLevelFilter;

class ossimQtImageWindowController : public QObject
{
    Q_OBJECT
public:
    ossimQtImageWindowController(ossimQtImageWindow* window);
   virtual ~ossimQtImageWindowController();

   virtual void enterEvent(QEvent* evt);
   virtual void leaveEvent(QEvent* evt);
   virtual void forceExit();
   virtual void fileExit();
   virtual void fileSaveAs();
   virtual void fileSaveTiePoints();
   virtual void filePrint();

   virtual void viewTool();
   virtual void showPositionInformation();
   virtual void showPositionQuality();   
   virtual void imageGeometry();
   virtual void vectorSource();
   
   virtual void bandSelector();
   virtual void brightnessContrast();
   virtual void histogramOperation();
   virtual void hsiAdjustments();
   virtual void fusion();

  // correction filters
  //
  virtual void topographicCorrection();


  virtual void geometryAdjustment();

  virtual void editImageChain();
  virtual void editImageInfo();  
  virtual void editLayers();
   
   virtual void igenExport();
   
   virtual void zoomToolButtonToggled(bool value);
   virtual void panToToolButtonToggled(bool value);
   
   virtual void setZoomModeOut();
   virtual void setZoomModeIn();
   
   virtual void zoomTo(const ossimIpt& ulPt, const ossimIpt& lrPt);
   virtual void zoomTo(const ossimIpt& center, const ossimDpt& scale);
   virtual void zoomToCenter(const ossimDpt& scale);
   virtual void zoomToMetersPerPixel(const ossimDpt& metersPerPixel);

   virtual void panTo(const ossimIpt& pt);
   
   virtual void setResampler(const QString& typeName);
   virtual void setResampler(const QString& minifyName,
                             const QString& magnifyName);
   ossimDpt viewCoordinateToImageCoordinate(const ossimDpt& viewPt);
   ossimGpt imageCoordinateToGround(const ossimDpt& imgPt);
   virtual bool addInput(ossimConnectableObject* obj, bool refresh=true);
   virtual bool removeInput(ossimConnectableObject* obj, bool refresh = true);
   virtual void disconnectAllInputs();
   virtual void closeEvent(QCloseEvent* evt);
   virtual ossimQtSwipeScrollingImageWidget* getImageWidget();
   virtual ossimQtImageWindow* getImageWindow();
   virtual ossimDpt groundToImage(const ossimGpt& gpt)const;
   virtual ossimGpt imageToGround(const ossimIpt& ipt)const;
   virtual ossimGpt imageToGround(const ossimDpt& dpt)const;
   virtual ossimDpt imageToEastingNorthing(const ossimDpt& dpt)const;
   virtual ossimDpt imageToEastingNorthing(const ossimIpt& ipt)const;
   virtual void initializePointers();
   virtual void initializeMenus();
   virtual bool getAutoDeleteIfNoInputFlag()const;
   virtual void setAutoDeleteIfNoInputFlag(bool flag);
   virtual bool event(QEvent* event);

public slots:
   virtual void applyView(const ossimObject* view);
   virtual void propagateView();
   virtual void propagateResampler();
   virtual void fitToWindow();
   virtual void refreshWindow();
   virtual void fullRes();
   virtual void trackScrollingImageWidget(ossimQtMouseEvent* evt);
   virtual void swipeAddLayer();
   virtual void swipeAddLayerApply(ossimConnectableObject::ConnectableObjectList& layerObjList);
   virtual void swipeRemoveLayer();
   
   virtual void swipeVertical();
   virtual void swipeHorizontal();
   virtual void swipeBox();
   virtual void swipeCircle();
   virtual void swipeNone();

   virtual void igenDialogDestroyed();
   virtual void informationDialogDestroyed();
   virtual void positionQualityDialogDestroyed();   
   virtual void setIdentityMode(bool flag);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const ossimString& prefix)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const ossimString& prefix);

signals:
   void closeSwipe();
   void viewChanged(const ossimObject* viewObj);

protected:

   /**
    * @brief Method to prompt the user to build overviews.  Builds if "yes" is
    * clicked and refreshes the image handler with an openOverview() call.
    *
    * @return true on success, false on error.
    */
   bool buildOverviews() const;   

   /**
    * @brief Method to prompt the user to build a histogram.  Builds if "yes" is
    * clicked and refreshes the image handler with an openHistogram() call.
    *
    * @return true on success, false on error.
    */
   bool buildHistogram() const;

   /**
    * @brief Determines if underlying input chain is a single image or a
    * composite image.
    *
    * @return true if single image false if not.
    */
   bool isSingleImageDisplay() const;
  
   virtual void setViewToInputs();
   virtual ossimGpt getCenterGround()const;
   virtual ossimGpt getBoundingCenterGround()const;
   virtual void applyViewWithCenter(const ossimObject* view,
                                    const ossimGpt& gpt);
   virtual void setFullResInputValue();
   virtual ossim_uint32 getNumberOfRLevels();

   virtual void getInputResampler(ossimString& minify,
                                  ossimString& magnify);
   virtual void setStatusText();
   virtual void buildStatusText(QString& statusText);

   /**
    * @brief Convenience method to return image handler.
    * @return A pointer to the ossimImageRenderer if there is one; else, 0.
    * @note This method returns 0 if isSingleImageDisplay() is false.
    */
   ossimImageHandler* getImageHandler() const;

   /**
    * @brief Convenience method to return the histogram remapper.
    * @return A pointer to the ossimImageRenderer if there is one; else, 0.
    * @note This method returns 0 if isSingleImageDisplay() is false.
    */
   ossimHistogramRemapper* getHistogramRemapper() const;
   
   /**
    * @brief Convenience method to returns resampler.
    * @return A pointer to the ossimImageRenderer if there is one; else, 0.
    * @note This method returns 0 if isSingleImageDisplay() is false.
    */
   ossimImageRenderer* getImageRenderer() const;

   /**
    * @brief Convenience method to return rlevel filter.
    * @return A pointer to the ossimRLevelFilter if there is one; else, 0.
    * @note This method returns 0 if isSingleImageDisplay() is false.
    */
   ossimRLevelFilter* getRLevelFilter() const;

   /**
    * @brief Adds an ossimRLevelFilter to the image chain if found.
    */
   void addRLevelFilter() const;

   /** only valid in identitiy mode. */
   void rnToR0(const ossimDpt& rnPt, ossimDpt& r0Pt) const;

   /** only valid in identitiy mode. */
   void r0ToRn(const ossimDpt& r0Pt, ossimDpt& rnPt) const;

   /**
    * Initializes zone and hemisphere.
    * Will use output projection (theView) if UTM; else, it
    * will use ossimUtmProjection::computeZone using theCurrentCenter
    * longitude for the zone and latitude for the hemisphere.
    *
    * @param zone The zone to initialize.
    * @param hemisphere The hemisphere to initialize.
    */
   void getZoneHemisphere(ossim_int32& zone, char& hemisphere) const;

   void setHistogramStretch();

   ossimQtImageWindow* theImageWindow;
   bool theZoomInFlag;
   ossimQtImageControllerZoomEventFilter *theZoomFilter;
   ossimQtImageControllerPanEventFilter  *thePanFilter;
   ossimQtImageControllerOssimEventCapture* theEventCapture;
   ossimQtImageControllerTrackMouseEventFilter* theTrackingCursorFilter;
   ossimRefPtr<ossimObject>   theView;
   ossimQtIgenDialog* theIgenDialog;
   ossimQtInformationDialog* thePositionInformationDialog;
   ossimQtPositionQualityDialog* thePositionQualityDialog;
   QObject*       theMainWindowController;
   mutable bool   thePropagateFlag;
   ossimDpt       theFullRes;
   ossimGpt       theCurrentCenter;
   QString        theStatusTextString;
   ossimRgbVector theRgbVector;
   std::vector<ossim_float64> theRawPixelValues;
   bool          theAutoDeleteIfNoInputFlag;

   /**
    * theIdentityModeFlag only valid for single image windows.
    */
   bool theIdentityModeFlag;
   ossimRefPtr<ossimImageProjectionModel> theImageModel;
   
};

#endif
