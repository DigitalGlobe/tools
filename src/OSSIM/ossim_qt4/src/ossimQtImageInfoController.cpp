//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
// 
// Description:
//
// Main interface between the image info dialog box and the image from the
// image chain.
//
//*************************************************************************
// $Id: ossimQtImageInfoController.cpp 17930 2010-08-19 19:33:31Z dburken $

#include <vector>

using namespace std;

#include <ossim/base/ossimConstants.h>
#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <QtGui/QWidget>
#include <QtGui/QDialog>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>
#include <Qt3Support/Q3FileDialog>
#include <ossimQt/ossimQtImageInfoController.h>
#include <ossimQt/ossimQtImageInfoDialog.h>
#include <ossimQt/ossimQtQuadProjectionDialog.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossimQt/ossimQtDisplayUtility.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/projection/ossimMiscProjectionFactory.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/base/ossimScalarTypeLut.h>

static const ossimString LAT_FORMAT = "dd@ mm' ss.ssssssss\" C";
static const ossimString LON_FORMAT = "ddd@ mm' ss.ssssssss\" C";


static ossimTrace traceDebug("ossimQtImageInfoController:degug");

ossimQtImageInfoController::ossimQtImageInfoController(ossimQtImageInfoDialog* dialog)
   :
      ossimConnectableObjectListener(),
      theDialog(dialog),
      theWidget(NULL),
      theCustomDialog(NULL),
      theCallBackDisabled(false)
{
}

ossimQtImageInfoController::~ossimQtImageInfoController()
{
   if (theCustomDialog)
   {
      theCustomDialog->close(true);
      theCustomDialog = NULL;
   }
}

void ossimQtImageInfoController::setImageWidget(ossimQtScrollingImageWidget* widget)
{
   theWidget = widget;
   if (theDialog)
   {
      initializeDialog();
   }
}

void ossimQtImageInfoController::initializeDialog()
{
   if (!theDialog || !theWidget) return;
   
   theCallBackDisabled = true;

   buildLayerMenu();
   buildImageGeometryPage(0);
   buildProjectionPage(0);
   buildValidVerticesPage(0);
   
   theCallBackDisabled = false;
}

void ossimQtImageInfoController::updateDialog()
{
   // cout << "updateDialog entered..." << endl;
   
   if (!theDialog || !theWidget) return;

   theCallBackDisabled = true;

   // Get the current layer.
   ossim_uint32 layer = theDialog->theLayerComboBox->currentItem();
   buildImageGeometryPage(layer);
   buildProjectionPage(layer);
   buildValidVerticesPage(layer);
   
   theCallBackDisabled = false;

   // cout << "updateDialog exited..." << endl;
}


void ossimQtImageInfoController::buildImageGeometryPage(ossim_uint32 layer) const
{
   const ossim_uint32 LAYERS = theWidget->getNumberOfInputs();
   
   if (layer >= LAYERS)
   {
      return;
   }
   
   ossimImageHandler* ih = getImageHandler(layer);
   if (!ih)
   {
      return;
   }

   // Used throughout.
   QString qs;
   ossim_uint32 i;
   
   // Get the image handler type.
   qs = ih->getShortName().c_str();
   theDialog->theImageTypeLineEdit->setText(qs);

   // Get the radiometry.
   ossimScalarType st = ih->getOutputScalarType();
   qs = ossimScalarTypeLut::instance()->getEntryString(st).c_str();
   theDialog->theRadiometryLineEdit->setText(qs);

   // Get the byte order.

   // Get the number of lines.
   i = ih->getNumberOfLines(0);
   qs = ossimString::toString(i).c_str();
   theDialog->theLinesLineEdit->setText(qs);

   // Get the number of samples.
   i = ih->getNumberOfSamples(0);
   qs = ossimString::toString(i).c_str();
   theDialog->theSamplesLineEdit->setText(qs);

   // Get the number of bands.
   i = ih->getNumberOfOutputBands();
   qs = ossimString::toString(i).c_str();
   theDialog->theBandsLineEdit->setText(qs);

   // Get the number of reduced resolution sets.
   i = ih->getNumberOfDecimationLevels();
   qs = ossimString::toString(i).c_str();
   theDialog->theRLevelLineEdit->setText(qs);

   // Initialize the band spin box.
   theDialog->theBandSpinBox->setMinValue(1);  // Always at least one.
   theDialog->theBandSpinBox->setMaxValue(static_cast<int>(i));
   theDialog->theBandSpinBox->setValue(1);

   // Set the min, max and null values.
   initializeMinMaxNulls();

   // Make all the line edits boxes read only for now.
   freezeLines(true);
   freezeSamples(true);
   freezeBands(true);
   freezeReducedResLevel(true);
   freezeMin(true);
   freezeMax(true);
   freezeNull(true);
}

void ossimQtImageInfoController::buildProjectionPage(ossim_uint32 layer)
{
   theCallBackDisabled = true;

   buildProjectionMenu();
   buildDatumMenu();
   buildUnitsMenu();
   buildPixelTypeMenu();
   buildZoneMenu();
   buildHemisphereMenu();
   
   ossimImageHandler* ih = getImageHandler(layer);
   if (!ih)
      return;

   updateProjectionPanel(ih->getImageGeometry().get());
}

void ossimQtImageInfoController::projectionComboBoxActivated()
{
   if (!theDialog || !theWidget)
   {
      return;
   }

   if (theCustomDialog)
   {
      QString caption("Question:");
      QString text = "You currently have a dialog ";
      text += theCustomDialog->name();
      text += "open.\n";
      text += "Kill it?";
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::Yes)
      {
         theCustomDialog->close(true);
         theCustomDialog = NULL;
      }
      else
      {
         return;
      }
   }

   enableDisableProjectionPanelFields();
   const ossimString PROJ = getProjectionString();
   if ( (PROJ == "Unknown") || (PROJ.contains("Sensor Model")) )
   {
      return;
   }

   if ( PROJ == "ossimBilinearProjection")
   {
      QString caption("Question:");
      QString text = "This projection has a custom dialog that requires\n";
      text += "you to drop geogrphic points.\n";
      text += "Would you like to drop points?";
      
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::Yes)
      {
         ossimQtQuadProjectionDialog* qpd =
            new ossimQtQuadProjectionDialog(theDialog);

         qpd->setImageWidget(theWidget, getLayerIndex());

         // Capture the pointer.
         theCustomDialog = qpd;

         //---
         // Connect the destroyed signal of the custom dialog up to the
         // theDialog's->customDialogHasClosed() slot so that we get a
         // notice to null the pointer that we just captured.
         //---
         theDialog->connect(theCustomDialog, SIGNAL(destroyed()),
                            theDialog, SLOT(customDialogHasClosed()));
         
         qpd->show();
      }
      else
      {
         caption = "Notice";
         text = "Please select another projection or hit reset.";
         QMessageBox::information( theDialog,
                                   caption,
                                   text,
                                   QMessageBox::Ok );
         theDialog->theProjectionComboBox->setCurrentItem(PROJ_TYPE_UNKNOWN);
      }
      return;
   }
}

void ossimQtImageInfoController::enableDisableProjectionPanelFields() const
{
   const ossimString PROJ = getProjectionString();
   if ( (PROJ == "Unknown") ||
        (PROJ.contains("Sensor Model")) ||
        (PROJ == "ossimBilinearProjection") )
   {
      enableAllProjectionPanel(false);
      return;
   }

   //---
   // Start with all line edits unfrozen.
   // These can be frozen by things that have fixed values.
   //---
   enableAllProjectionPanel(true);
   freezeFalseEastingNorthing(false);
   freezeOrigin(false);
   freezeScaleFactor(false);
   freezeParallels(false);
   
   if ( (PROJ == "ossimAlbersProjection") ||
        (PROJ == "ossimLambertConformalConicProjection") )
   {
      theDialog->theUnitsComboBox->setCurrentItem(PROJ_UNITS_METERS);
      enableZone(false);
      enableHemisphere(false);
      enableFalseEastingNorthing(true);
      enableScaleFactor(false);
      enableParallels(true);
   }
   else if (PROJ == "ossimUtmProjection")  
   {
      setup_UTM_Menus();
   }
   else if ( PROJ.contains("NAD") ) // All state planes start with NAD.
   {
      setupStatePlaneMenus(PROJ);
   }
   else if ( PROJ == "ossimTransMercatorProjection")
   {
      theDialog->theUnitsComboBox->setCurrentItem(PROJ_UNITS_METERS);
      enableZone(false);
      enableHemisphere(false);
      enableFalseEastingNorthing(true);
      enableScaleFactor(true);
      enableParallels(false);
   }
   else if ( (PROJ == "ossimEquDistCylProjection") ||
             (PROJ == "ossimLlxyProjection") )
   {
      theDialog->theUnitsComboBox->setCurrentItem(PROJ_UNITS_DEGREES);
      enableZone(false);
      enableHemisphere(false);
      enableScaleFactor(false);
      enableParallels(false);
      enableFalseEastingNorthing(false);
   }
   else
   {
      // Default...
      theDialog->theUnitsComboBox->setCurrentItem(PROJ_UNITS_METERS);
      enableZone(false);
      enableHemisphere(false);
      enableFalseEastingNorthing(true);
      enableScaleFactor(true);
      enableParallels(true);
   }
}

void ossimQtImageInfoController::zoneComboBoxActivated() const
{
   if (!theDialog)
   {
      return;
   }
   
   const ossimString PROJ = getProjectionString();
   if (PROJ == "ossimUtmProjection")
   {
      setup_UTM_Menus();
   }
}

void ossimQtImageInfoController::setupStatePlaneMenus(const ossimString& name) const
{
   // State planes are either Transverse Mercator or Lambert.

   //---
   // Get the projection info from the name string.
   // Note:  That "info" is owned by the static instance of the
   //        ossimStatePlaneProjectionFactory so don't delete or bad things
   //        will happen to you!
   //---
   ossimRefPtr<ossimProjection> proj = 
      ossimEpsgProjectionFactory::instance()->createProjection(name);
   ossimMapProjection* map_proj = PTR_CAST(ossimMapProjection, proj.get());
   if (!map_proj)
      return;
   
   // Set the datum.
   setDatumMenu(map_proj->getDatum()->code());

   // Set the units to meters.
   theDialog->theUnitsComboBox->setCurrentItem(PROJ_UNITS_METERS);

   // Disable the zone.
   enableZone(false);
   
   // Disable the hemisphere.
   enableHemisphere(false);

   // Used throughout...
   ossimString os;
   QString     qs;

   // Enable false easting/northing.
   enableFalseEastingNorthing(true);

   //---
   // Set false easting. NOTE:  Some state planes imply feet.
   // Staying in meters for now.
   //---
   setFalseEasting(map_proj->getFalseEasting());

   // Set the false northing.
   setFalseNorthing(map_proj->getFalseNorthing());

   // Set the origin.
   ossimGpt origin (map_proj->getOrigin());
   setOriginLatitude(origin.lat);

   // Set the central meridian.
   setCentralMeridian(origin.lon);
   
   ossimTransMercatorProjection* tm_proj = PTR_CAST(ossimTransMercatorProjection, map_proj);
   ossimLambertConformalConicProjection* lcc_proj =
      PTR_CAST(ossimLambertConformalConicProjection, map_proj);
   if (tm_proj)
   {
      // Enable scale factor.
      enableScaleFactor(true);
      
      // Set the scale factor.
      setScaleFactor(tm_proj->getScaleFactor());
      
      // Disable parallels.
      enableParallels(false);
   }
   else if (lcc_proj)
   {
      // Disable scale factor.
      enableScaleFactor(false);
      
      // Enable parallels.
      enableParallels(true);
      
      // Set the parallels.
      setParallelOne(lcc_proj->getStandardParallel1());
      setParallelTwo(lcc_proj->getStandardParallel2());
   }
   
   // Freeze all the fixed things.
   enableDatum(false);
   freezeFalseEastingNorthing(true);
   freezeOrigin(true);
   freezeScaleFactor(true);
   freezeParallels(true);
}

void ossimQtImageInfoController::setup_UTM_Menus() const
{
   // Set the units to meters.
   theDialog->theUnitsComboBox->setCurrentItem(PROJ_UNITS_METERS);
   
   // Enable the zone.
   enableZone(true);

   // Enable the hemisphere.
   enableHemisphere(true);
   
   // Disable false easting, northing.
   enableFalseEastingNorthing(false);
   
   // Disable scale factor.
   enableScaleFactor(false);
   
   // Disable parallels.
   enableParallels(false);
   
   ossimUtmProjection utm(getZone());
   setOriginLatitude(&utm);
   setCentralMeridian(&utm);

   // Make the origin and meridian uneditable.
   freezeOrigin(true);
}

void ossimQtImageInfoController::tabChanged(const QString& qs)
{
   if (!theDialog)
   {
      return;
   }

   if (qs == "Image Projection")
   {
      //---
      // Call the projectionComboBoxActivated() method.  This will handle the
      // making of custom dialogs if needed.
      //---
      projectionComboBoxActivated();
   }
}

void ossimQtImageInfoController::updateProjectionPanel(const ossimImageGeometry* image_geom)
{
   if (!image_geom)
      return;

   ossimKeywordlist kwl;
   image_geom->saveState(kwl);

   // Lookup and set the projection
   setProjectionMenu(kwl);
   
   // Set datum menu.
   setDatumMenu(kwl);

   //---
   // Set the scale factor if there is one.  No interface in ossimMapProjection
   // so we'll do it with a keyword list.
   //---
   setScaleFactor(kwl);

   //---
   // Temp commented out...  Since the tie point displayed is always shifted
   // to point by the projection code.
   // Set the pixel type menu.
   // setPixelTypeMenu(kwl);
   //---
   
   //---
   // Since a keyword list for a projection could be as simple as a pcs code,
   // a tie point and scale, we will attempt to set all the fields from the
   // real projection
   //---
   
   // Grab a pointer to the current projection.
   const ossimMapProjection* proj = PTR_CAST(ossimMapProjection, image_geom->getProjection());
   if (proj)
   {
      // Set the units menu.
      setUnitsMenu(proj);

      // Set the zone menu.
      setZoneMenu(proj);

      // Set the hemisphere.
      setHemisphereMenu(proj);

      // Set the origin latitude.
      setOriginLatitude(proj);
      
      // Set the origin latitude.
      setCentralMeridian(proj);

      // Set the false easting.
      setFalseEasting(proj);

      // Set the false easting.
      setFalseNorthing(proj);

      // Set the X tie point.
      setTiePointX(proj);

      // Set the Y tie point.
      setTiePointY(proj);

      // Set the X scale.
      setScaleX(proj);

      // Set the Y scale.
      setScaleY(proj);

      // Set the first standard parallel.
      setParallelOne(proj);

      // Set the second standard parallel.
      setParallelTwo(proj);
   }
   else if (kwl.getSize())
   {
      //---
      // We didn't get a projection but still may have projection info.
      //---
      
      // Set the zone menu.
      setZoneMenu(kwl);

      // Set the hemisphere.
      setHemisphereMenu(kwl);

      // Set the origin latitude.
      setOriginLatitude(kwl);
      
      // Set the origin latitude.
      setCentralMeridian(kwl);

      // Set the false easting.
      setFalseEasting(kwl);

      // Set the false easting.
      setFalseNorthing(kwl);

      // Set the X tie point.
      setTiePointX(kwl);

      // Set the Y tie point.
      setTiePointY(kwl);

      // Set the X scale.
      setScaleX(kwl);

      // Set the Y scale.
      setScaleY(kwl);

      // Set the first standard parallel.
      setParallelOne(kwl);

      // Set the second standard parallel.
      setParallelTwo(kwl);      
      
   }
   
   // This enable disable appropriate menus.
   enableDisableProjectionPanelFields();
}

void ossimQtImageInfoController::buildProjectionMenu() const
{
   if (!theDialog)
   {
      return;
   }
   
   QStringList projList;
   projList.append("Unknown");
   projList.append("Sensor Model");
   projList.append("ossimBilinearProjection");
   
   std::vector<ossimString> tempOssimList;

   // Add all the map projections to the list.
   ossimMapProjectionFactory::instance()->getTypeNameList(tempOssimList);

   // Add all the State Plane Projection Coordinate Systems(PCS) to the list.
   ossimEpsgProjectionFactory::instance()->getTypeNameList(tempOssimList);

   // Put them in the qt list.
   std::vector<ossimString>::iterator listIter = tempOssimList.begin();
   while(listIter != tempOssimList.end())
   {
      projList.append((*listIter).c_str());
      ++listIter;
   }

   theDialog->theProjectionComboBox->clear();
   theDialog->theProjectionComboBox->addItems(projList);
}

void ossimQtImageInfoController::buildDatumMenu() const
{
   if (!theDialog)
   {
      return;
   }
   
   QStringList datumList;
   std::vector<ossimString> tempOssimDatumList =
      ossimDatumFactory::instance()->getList();
   std::vector<ossimString>::iterator listIter = tempOssimDatumList.begin();
   
   while(listIter != tempOssimDatumList.end())
   {
      const ossimDatum* datum = ossimDatumFactory::instance()->
         create(*listIter);
      if(datum)
      {
         datumList.append((datum->code() + ": " + datum->name()).c_str());
      }
      ++listIter;
   }
   theDialog->theDatumComboBox->clear();
   theDialog->theDatumComboBox->addItems(datumList);   
}

void ossimQtImageInfoController::buildUnitsMenu() const
{
   if (!theDialog)
   {
      return;
   }
   
   theDialog->theUnitsComboBox->clear();
   theDialog->theUnitsComboBox->insertItem("Degrees");
   theDialog->theUnitsComboBox->insertItem("Meters");   
}

void ossimQtImageInfoController::buildPixelTypeMenu() const
{
   if (!theDialog)
   {
      return;
   }
   
   theDialog->thePixelTypeComboBox->clear();
   theDialog->thePixelTypeComboBox->insertItem("Center of pixel (point)");
   theDialog->thePixelTypeComboBox->
      insertItem("Upper left corner of pixel (area)");  
}

void ossimQtImageInfoController::buildZoneMenu() const
{
   if (!theDialog)
   {
      return;
   }
   
   theDialog->theZoneComboBox->clear();
   
   for (int i = 1; i < 61; ++i)
   {
      theDialog->theZoneComboBox->
         insertItem(ossimString::toString(i).c_str());
   }
}

void ossimQtImageInfoController::buildHemisphereMenu() const
{
   if (!theDialog)
   {
      return;
   }

   theDialog->theHemisphereComboBox->clear();
   theDialog->theHemisphereComboBox->insertItem("Northern");
   theDialog->theHemisphereComboBox->insertItem("Southern");     
}

ossimString ossimQtImageInfoController::getProjectionString() const
{
   ossimString s;
   if (theDialog)
   {
      s = theDialog->theProjectionComboBox->currentText().ascii();
   }
   return s;
}


ossimRefPtr<ossimMapProjection> 
ossimQtImageInfoController::getNewMapProjection(const ossimKeywordlist& kwl) const
{
   ossimString projection_name = getProjectionString();
   ossimRefPtr<ossimMapProjection> map_proj = getNewMapProjection(projection_name);
   if (map_proj.valid())
      map_proj->loadState(kwl);

   return map_proj;
}

ossimRefPtr<ossimMapProjection> 
ossimQtImageInfoController::getNewMapProjection(const ossimString& name) const
{
   ossimRefPtr<ossimMapProjection> map_proj;
   ossimRefPtr<ossimProjection>  proj = 
      ossimMapProjectionFactory::instance()->createProjection(name);
  
   if (!proj.valid())
   {
      // Test for state plane. ??? WHY ISN'T THIS IN THE MAIN FACTORY?
      proj = ossimEpsgProjectionFactory::instance()->createProjection(name);
   }

   if (proj.valid())
      map_proj = PTR_CAST(ossimMapProjection, proj.get());

   return map_proj;  
}

ossimString ossimQtImageInfoController::getDatumString() const
{
   ossimString s = theDialog->theDatumComboBox->currentText().ascii();
   return s;
}

ossimString ossimQtImageInfoController::getUnitsString() const
{
   ossimString s = theDialog->theUnitsComboBox->currentText().ascii();
   return s;
}

ossimString ossimQtImageInfoController::getPixelTypeString() const
{
   ossimString s = theDialog->thePixelTypeComboBox->currentText().ascii();
   return s;
}

ossimString ossimQtImageInfoController::getZoneString() const
{
   ossimString s;
   if (theDialog->theZoneComboBox->isEnabled())
   {
      s = theDialog->theZoneComboBox->currentText().ascii();
   }
   return s;
}

ossimString ossimQtImageInfoController::getHemisphereString() const
{
   ossimString s;
   if (theDialog->theHemisphereComboBox->isEnabled())
   {
      s = theDialog->theHemisphereComboBox->currentText().ascii();
   }
   return s;
}

ossimString ossimQtImageInfoController::getOriginLatitudeString()  const
{
   ossimString s = theDialog->theOriginLatitudeLineEdit->text().ascii();
   return s;
}

ossimString ossimQtImageInfoController::getCentralMeridianString() const
{
   ossimString s = theDialog->theCentralMeridianLineEdit->text().ascii();
   return s;
}

ossimString ossimQtImageInfoController::getFalseEastingString() const
{
   ossimString s;
   if (theDialog->theFalseEastingLineEdit->isEnabled())
   {
      s = theDialog->theFalseEastingLineEdit->text().ascii();
   }
   return s; 
}

ossimString ossimQtImageInfoController::getFalseNorthingString() const
{
   ossimString s;
   if (theDialog->theFalseNorthingLineEdit->isEnabled())
   {
      s = theDialog->theFalseNorthingLineEdit->text().ascii();
   }
   return s;  
}

ossimString ossimQtImageInfoController::getTiePointX_String() const
{
   ossimString s = theDialog->theTiePointX_LineEdit->text().ascii();
   return s;  
}

ossimString ossimQtImageInfoController::getTiePointY_String() const
{
   ossimString s = theDialog->theTiePointY_LineEdit->text().ascii();
   return s;   
}

ossimString ossimQtImageInfoController::getScaleX_String() const
{
   ossimString s = theDialog->theScaleX_LineEdit->text().ascii();
   return s;
}

ossimString ossimQtImageInfoController::getScaleY_String() const
{
   ossimString s = theDialog->theScaleY_LineEdit->text().ascii();
   return s; 
}

ossimString ossimQtImageInfoController::getScaleFactorString() const
{
   ossimString s;
   if (theDialog->theScaleFactorLineEdit->isEnabled())
   {
      s = theDialog->theScaleFactorLineEdit->text().ascii();
   }
   return s; 
}

ossimString ossimQtImageInfoController::getParallelOneString() const
{
   ossimString s;
   if (theDialog->theParallelOneLineEdit->isEnabled())
   {
      s = theDialog->theParallelOneLineEdit->text().ascii();
   }
   return s;    
}

ossimString ossimQtImageInfoController::getParallelTwoString() const
{
   ossimString s;
   if (theDialog->theParallelTwoLineEdit->isEnabled())
   {
      s = theDialog->theParallelTwoLineEdit->text().ascii();
   }
   return s;  
}

void ossimQtImageInfoController::setProjectionMenu(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::TYPE_KW);

   if (lookup)
   {
      QString s1 = lookup;
      const int COUNT = theDialog->theProjectionComboBox->count();
      for (int i = 0; i < COUNT; ++i)
      {
         QString s2 = theDialog->theProjectionComboBox->text(i);
         
         if (s1 == s2)
         {
            theDialog->theProjectionComboBox->setCurrentItem(i);
            return;
         }
      }

      // If we get here "type" must be a sensor model.
      QString qs = lookup;
      qs += " Sensor Model";
      theDialog->theProjectionComboBox->
         setCurrentItem(ossimQtImageInfoController::PROJ_TYPE_SENSOR);
      theDialog->theProjectionComboBox->setCurrentText(qs);
      return;
   }

   // If we get here "type" must not be set.
   theDialog->theProjectionComboBox->
      setCurrentItem(ossimQtImageInfoController::PROJ_TYPE_UNKNOWN);
   return;
}

void ossimQtImageInfoController::setDatumMenu(const ossimKeywordlist& kwl) const
{
   ossimString code = "WGE"; // Default datum...
   
   const char* lookup = kwl.find(ossimKeywordNames::DATUM_KW);
   if (lookup)
   {
      code = lookup;
   }

   setDatumMenu(code);
}

void ossimQtImageInfoController::setDatumMenu(const ossimString& code) const
{
   const int COUNT = theDialog->theDatumComboBox->count();
   for (int i = 0; i < COUNT; ++i)
   {
      ossimString name = theDialog->theDatumComboBox->text(i).ascii();
      
      if (name.contains(code))
      {
         theDialog->theDatumComboBox->setCurrentItem(i);
         return;
      }
   }
}

void ossimQtImageInfoController::setUnitsMenu(const ossimMapProjection* proj) const
{
   if (proj)
   {
      if (proj->isGeographic())
      {
         theDialog->theUnitsComboBox->setCurrentItem(PROJ_UNITS_DEGREES);
      }
   }

   theDialog->theUnitsComboBox->setCurrentItem(PROJ_UNITS_METERS);
}

void ossimQtImageInfoController::setPixelTypeMenu(const ossimKeywordlist& kwl) const
{
   ossimString pixelType = kwl.find(ossimKeywordNames::PIXEL_TYPE_KW);
   pixelType=pixelType.trim();
   pixelType.downcase();

   if( pixelType.contains("area") )
   {
      theDialog->thePixelTypeComboBox->setCurrentItem(PIXEL_TYPE_AREA);
   }
   else
   {
      // Point is the default.
      theDialog->thePixelTypeComboBox->setCurrentItem(PIXEL_TYPE_POINT);
   }
}

void ossimQtImageInfoController::setZoneMenu(const ossimMapProjection* proj) const
{
   ossimUtmProjection* utm_proj = PTR_CAST(ossimUtmProjection, proj);
   if (utm_proj)
   {
      int zone = utm_proj->getZone() - 1;
      theDialog->theZoneTextLabel->setEnabled(true);
      theDialog->theZoneComboBox->setEnabled(true);
      theDialog->theZoneComboBox->setCurrentItem(zone);
   }
   else
   {
      // Disable
      theDialog->theZoneTextLabel->setEnabled(false);
      theDialog->theZoneComboBox->setEnabled(false);
   }
}

void ossimQtImageInfoController::setZoneMenu(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::ZONE_KW);
   if (lookup)
   {
      ossimString os = lookup;
      int zone = os.toInt();
      if (zone > 0 && zone < 61)
      {
         theDialog->theZoneComboBox->setCurrentItem(zone);
      }
   }
}

void ossimQtImageInfoController::setHemisphereMenu(const ossimMapProjection* proj) const
{
   ossimUtmProjection* utm_proj = PTR_CAST(ossimUtmProjection, proj);
   if (utm_proj)
   {
      theDialog->theHemisphereTextLabel->setEnabled(true);
      theDialog->theHemisphereComboBox->setEnabled(true);
      
      char h = utm_proj->getHemisphere();
      if (h == 'N')
      {
         theDialog->theHemisphereComboBox->
            setCurrentItem(NORTHERN_HEMISPHERE);
      }
      else
      {
         theDialog->theHemisphereComboBox->
            setCurrentItem(SOUTHERN_HEMISPHERE);
      }
   }
   else
   {
      // Disable
      theDialog->theHemisphereTextLabel->setEnabled(false);
      theDialog->theHemisphereComboBox->setEnabled(false);
   }
}

void ossimQtImageInfoController::setHemisphereMenu(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::HEMISPHERE_KW);
   if (lookup)
   {
      ossimString os = lookup;
      os = os.trim();
      os = os.upcase();
      if (os[static_cast<std::string::size_type>(0)] == 'N')
      {
         theDialog->theHemisphereComboBox->setCurrentItem(NORTHERN_HEMISPHERE);
      }
      else if (os[static_cast<std::string::size_type>(0)] == 'S')
      {
         theDialog->theHemisphereComboBox->setCurrentItem(SOUTHERN_HEMISPHERE);
      }
   }
}

void ossimQtImageInfoController::setOriginLatitude(const ossimMapProjection* proj) const
{
   setOriginLatitude(proj->origin().latd());
}

void ossimQtImageInfoController::setOriginLatitude(const ossimKeywordlist& kwl) const
{
   const char* lookup;
   if (lookup = kwl.find(ossimKeywordNames::ORIGIN_LATITUDE_KW))
   {
      ossimString os = lookup;
      setOriginLatitude(os.toDouble());
   }

   // Possible alternate KW's for this value:
   else if (lookup = kwl.find(ossimKeywordNames::REF_GPT_LAT_KW))
   {
       ossimString os = lookup;
       setOriginLatitude(os.toDouble());
   }
}

void ossimQtImageInfoController::setOriginLatitude(const double& origin) const
{
   ossimDms dms(origin, true); // true = lat flag...
   ossimString os = dms.toString(LAT_FORMAT);
   theDialog->theOriginLatitudeLineEdit->setText(os.c_str());
}

void ossimQtImageInfoController::setCentralMeridian(const ossimMapProjection* proj) const
{
   setCentralMeridian(proj->origin().lond());
}

void ossimQtImageInfoController::setCentralMeridian(const ossimKeywordlist& kwl) const
{
   const char* lookup;
   if (lookup = kwl.find(ossimKeywordNames::CENTRAL_MERIDIAN_KW))
   {
      ossimString os = lookup;
      setCentralMeridian(os.toDouble());
   }
   
   // Possible alternate KW's for this value:
   else if (lookup = kwl.find(ossimKeywordNames::REF_GPT_LON_KW))
   {
       ossimString os = lookup;
       setCentralMeridian(os.toDouble());
   }
}

void ossimQtImageInfoController::setCentralMeridian(const double& meridian) const
{
   ossimDms dms(meridian, false); // false = longitude flag...
   ossimString os = dms.toString(LON_FORMAT);
   theDialog->theCentralMeridianLineEdit->setText(os.c_str());
}

void ossimQtImageInfoController::setFalseEasting(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::FALSE_EASTING_KW);
   if (lookup)
   {
      theDialog->theFalseEastingLineEdit->setText(lookup);
   }
}

void ossimQtImageInfoController::setFalseEasting(const ossimMapProjection* proj) const
{
   setFalseEasting(proj->getFalseEasting());
}

void ossimQtImageInfoController::setFalseEasting(const double& false_easting) const
{
   ossimString os = ossimString::toString(false_easting);
   theDialog->theFalseEastingLineEdit->setText(os.c_str());
}

void ossimQtImageInfoController::setFalseNorthing(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::FALSE_NORTHING_KW);
   if (lookup)
   {
      theDialog->theFalseNorthingLineEdit->setText(lookup);
   }
}

void ossimQtImageInfoController::setFalseNorthing(const ossimMapProjection* proj) const
{
   setFalseNorthing(proj->getFalseNorthing());
}


void ossimQtImageInfoController::setFalseNorthing(const double& false_northing) const
{
   ossimString os = ossimString::toString(false_northing);
   theDialog->theFalseNorthingLineEdit->setText(os.c_str());
}

void ossimQtImageInfoController::setTiePointX(const ossimMapProjection* proj) const
{
   QString qs = "";
   if (proj->isGeographic())
   {
      ossimGpt gpt = proj->getUlGpt();
      ossimDms dms(gpt.lond(),
                   false); // lat flag...
      
      qs = dms.toString(LON_FORMAT).c_str();
   }
   else
   {
      ossimDpt ulpt = proj->getUlEastingNorthing();
      qs = ossimString::toString(ulpt.x).c_str();
   }
   theDialog->theTiePointX_LineEdit->setText(qs);
}

void ossimQtImageInfoController::setTiePointX(const ossimKeywordlist& kwl) const
{
   const char* lookup;
   if(lookup = kwl.find(ossimKeywordNames::TIE_POINT_EASTING_KW))
   {
      theDialog->theTiePointX_LineEdit->setText(lookup);
   }
   else if (lookup = kwl.find(ossimKeywordNames::TIE_POINT_LON_KW))
   {
       ossimString os = lookup;
       ossimDms dms(os.toDouble(), false);
       os = dms.toString(LON_FORMAT);
       theDialog->theTiePointX_LineEdit->setText(os.c_str());
   }
}

void ossimQtImageInfoController::setTiePointX(const double& tie) const
{
   ossimString os;
   
   if (isGeographic())
   {
      ossimDms dms(tie, false);
      os = dms.toString(LON_FORMAT);
   }
   else
   {
      os = ossimString::toString(tie);
   }
   theDialog->theTiePointX_LineEdit->setText(os.c_str());
}

void ossimQtImageInfoController::setTiePointY(const ossimMapProjection* proj) const
{
   QString qs = "";
   if (proj->isGeographic())
   {
      ossimGpt gpt = proj->getUlGpt();
      ossimDms dms(gpt.latd(),
                   true); // lat flag...
         
      qs = dms.toString(LAT_FORMAT).c_str();
   }
   else
   {
      ossimDpt ulpt = proj->getUlEastingNorthing();
      qs = ossimString::toString(ulpt.y).c_str();
   }
   theDialog->theTiePointY_LineEdit->setText(qs);
}

void ossimQtImageInfoController::setTiePointY(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::TIE_POINT_NORTHING_KW);
   if(lookup)
   {
      theDialog->theTiePointY_LineEdit->setText(lookup);
   }
   else
   {
      lookup = kwl.find(ossimKeywordNames::TIE_POINT_LAT_KW);
      if (lookup)
      {
         ossimString os = lookup;
         ossimDms dms(os.toDouble(), true);
         os = dms.toString(LAT_FORMAT);
         theDialog->theTiePointY_LineEdit->setText(os.c_str());
      }
   }
}

void ossimQtImageInfoController::setTiePointY(const double& tie) const
{
   ossimString os;
   
   if (isGeographic())
   {
      ossimDms dms(tie, true);
      os = dms.toString(LON_FORMAT);
   }
   else
   {
      os = ossimString::toString(tie);
   }
   QString qs = os.c_str();
   theDialog->theTiePointY_LineEdit->setText(qs);
}

void ossimQtImageInfoController::setScaleX(const ossimMapProjection* proj) const
{
   QString qs = "";
   ossimDpt pt;
   if (proj->isGeographic())
   {
      pt = proj->getDecimalDegreesPerPixel();
   }
   else
   {
      pt = proj->getMetersPerPixel();
   }
   qs = ossimString::toString(pt.x).c_str();
   theDialog->theScaleX_LineEdit->setText(qs);
}

void ossimQtImageInfoController::setScaleX(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::METERS_PER_PIXEL_X_KW);
   if(!lookup)
   {
      lookup = kwl.find(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON);
   }

   if (lookup)
   {
      theDialog->theScaleX_LineEdit->setText(lookup);
   }
}

void ossimQtImageInfoController::setScaleY(const ossimMapProjection* proj) const
{
   QString qs = "";
   ossimDpt pt;
   
   if (proj->isGeographic())
   {
      pt = proj->getDecimalDegreesPerPixel();
   }
   else
   {
      pt = proj->getMetersPerPixel();
   }
   qs = ossimString::toString(pt.y).c_str();
   theDialog->theScaleY_LineEdit->setText(qs);
}

void ossimQtImageInfoController::setScaleY(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::METERS_PER_PIXEL_Y_KW);
   if(!lookup)
   {
      lookup = kwl.find(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT);
   }

   if (lookup)
   {
      theDialog->theScaleY_LineEdit->setText(lookup);
   }
}

void ossimQtImageInfoController::setScaleFactor(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::SCALE_FACTOR_KW);
   if (lookup)
   {
      theDialog->theScaleFactorLineEdit->setText(lookup);
   }
}

void ossimQtImageInfoController::setScaleFactor(const double& scale) const
{
   ossimString os = ossimString::toString(scale);
   theDialog->theScaleFactorLineEdit->setText(os.c_str());  
}

void ossimQtImageInfoController::setParallelOne(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::STD_PARALLEL_1_KW);
   if (lookup)
   {
      ossimString s = lookup;
      double d = s.toDouble();
      setParallelOne(d);
   }
}

void ossimQtImageInfoController::setParallelOne(const ossimMapProjection* proj) const
{
   setParallelOne(proj->getStandardParallel1());
}

void ossimQtImageInfoController::setParallelOne(const double& degrees) const
{
   ossimDms dms(degrees, true); // true = lat flag...
   ossimString os = dms.toString(LAT_FORMAT);
   theDialog->theParallelOneLineEdit->setText(os.c_str());
}

void ossimQtImageInfoController::setParallelTwo(const ossimKeywordlist& kwl) const
{
   const char* lookup = kwl.find(ossimKeywordNames::STD_PARALLEL_2_KW);
   if (lookup)
   {
      ossimString s = lookup;
      double d = s.toDouble();
      setParallelTwo(d);
   }
}

void ossimQtImageInfoController::setParallelTwo(const ossimMapProjection* proj) const
{
   setParallelTwo(proj->getStandardParallel2());
}

void ossimQtImageInfoController::setParallelTwo(const double& degrees) const
{
   ossimDms dms(degrees, true); // true = lat flag...
   ossimString os = dms.toString(LAT_FORMAT);
   theDialog->theParallelTwoLineEdit->setText(os.c_str());
}

void ossimQtImageInfoController::initializeMinMaxNulls() const
{
   ossimImageHandler* ih = getImageHandler();
   if (!ih)
   {
      return;
   }
   const ossim_uint32 BAND = getBandIndex() - 1;
   if (BAND >= ih->getNumberOfOutputBands())
   {
      return;
   }

   const int PRECISION = 0;
   QString qs;
   ossim_float64 d;
   
   // Get the min value.
   d = ih->getMinPixelValue(BAND);
   qs = ossimString::toString(d, PRECISION, true).c_str();
   theDialog->theMinLineEdit->setText(qs);

   // Get the max value.
   d = ih->getMaxPixelValue(BAND);
   qs = ossimString::toString(d, PRECISION, true).c_str();
   theDialog->theMaxLineEdit->setText(qs);

   // Get the null value.
   d = ih->getNullPixelValue(BAND);
   qs = ossimString::toString(d, PRECISION, true).c_str();
   theDialog->theNullLineEdit->setText(qs);
}

void ossimQtImageInfoController::buildValidVerticesPage(ossim_uint32 layer) const
{
   ossimImageHandler* ih = getImageHandler(layer);
   if (ih)
   {
   }
}

void ossimQtImageInfoController::objectDestructingEvent(ossimObjectDestructingEvent& /* event */)
{
}

void ossimQtImageInfoController::scanForMinMax()
{
   QString caption("Sorry:");
   QString text = "Scan for min max not implemetted yet!";
   QMessageBox::information( theDialog,
                             caption,
                             text,
                             QMessageBox::Ok );
}

void ossimQtImageInfoController::scanForValidVertices()
{
   QString caption("Sorry:");
   QString text = "Scan for valid vertices not implemetted yet!";
   QMessageBox::information( theDialog,
                             caption,
                             text,
                             QMessageBox::Ok );
}

void ossimQtImageInfoController::layerComboBoxActivated()
{
   if (theCallBackDisabled) return;

   if (theCustomDialog)
   {
      QString caption("Question:");
      QString text = "You currently have a dialog ";
      text += theCustomDialog->name();
      text += "open from another layer.\n";
      text += "Kill it?";
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::Yes)
      {
         theCustomDialog->close(true);
         theCustomDialog = NULL;
      }
   }

   updateDialog();
}

ossim_uint32 ossimQtImageInfoController::getLayerIndex() const
{
   return static_cast<ossim_uint32>(theDialog->
                                    theLayerComboBox->currentItem());
}

ossim_uint32 ossimQtImageInfoController::getBandIndex() const
{
   return static_cast<ossim_uint32>(theDialog->theBandSpinBox->value());
}

ossimImageHandler* ossimQtImageInfoController::getImageHandler() const
{
   return getImageHandler(getLayerIndex());
}

ossimImageHandler* ossimQtImageInfoController::getImageHandler(ossim_uint32 layer) const
{
   ossimImageHandler* handler = NULL;
   ossimImageChain* ic = getChain(layer);
   if (ic)
   {
      // Image handler usually at the end of chain so start there...
      const ossim_uint32 NUMBER_OF_LINKS = ic->getNumberOfObjects(false);
      for (ossim_uint32 idx = NUMBER_OF_LINKS; idx > 0; --idx) 
      { 
         ossimConnectableObject* filter = (*ic)[(idx-1)];
         if (filter)
         {
            if ( PTR_CAST(ossimImageHandler, filter) )
            {
               handler = PTR_CAST(ossimImageHandler, filter);
               break;
            }
         }
      }
   }
   return handler;
}

ossimImageChain* ossimQtImageInfoController::getChain() const
{
   return getChain(getLayerIndex());
}

ossimImageChain* ossimQtImageInfoController::getChain(ossim_uint32 layer) const
{
   ossimImageChain* ic = NULL;
   ossimConnectableObject* obj = theWidget->getInput(layer);
   if (obj)
   {
      ic = PTR_CAST(ossimImageChain, obj);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimQtImageChainEditorController::getChain: getInput failed!" << std::endl;
   }
   return ic;
}

ossimImageViewTransform* ossimQtImageInfoController::getImageViewTransform() const
{
   ossimImageViewTransform* trans = NULL;

   ossimImageChain* ic = getChain();
   if (ic)
   {
      ossimImageRenderer* obj = (ossimImageRenderer*)ic->findObjectOfType("ossimImageRenderer", ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT);
      if(obj)
      {
         trans = obj->getImageViewTransform();
      }
   }
   return trans;
}

ossimProjection* ossimQtImageInfoController::getImageProjection() const
{
   ossimImageViewTransform* trans = getImageViewTransform();
   if (!trans)
      return 0;

   ossimImageViewProjectionTransform* ivpt = PTR_CAST(ossimImageViewProjectionTransform, trans);
   if(ivpt)
      return 0;

   ossimImageGeometry* geom = ivpt->getImageGeometry();
   if (!geom)
      return 0;

   return geom->getProjection();
}


void ossimQtImageInfoController::buildLayerMenu() const
{
   //---
   // Setup the layer menu.
   //---
   const ossim_uint32 LAYERS = theWidget->getNumberOfInputs();
   theDialog->theLayerComboBox->clear();
   for (ossim_uint32 layer = 0; layer < LAYERS; ++layer)
   {
      ossimImageHandler* ih = getImageHandler(layer);
      if (ih)
      {
         ossimString s = "layer ";
         s += ossimString::toString(layer + 1);
         s += ":image chain:";
         s += ih->getFilename();
         QString item = s.c_str();
         theDialog->theLayerComboBox->insertItem(item);
      }
   }
}

ossimString ossimQtImageInfoController::getDatumCode() const
{
   const ossimString SEPARATOR = ":";
   return getDatumString().before(SEPARATOR);
}

double ossimQtImageInfoController::getDegrees(const ossimString& s,
                                              bool lat_flag) const
{
   ossimDms dms(0.0, lat_flag);
   dms.setDegrees(s.c_str());
   return dms.getDegrees();
}

bool ossimQtImageInfoController::isGeographic() const
{
   const ossimString PROJ = getProjectionString();
   if ( (PROJ == "ossimEquDistCylProjection") ||
        (PROJ == "ossimLlxyProjection") )
   {
      return true;
   }

   return false;
}

long  ossimQtImageInfoController::getZone() const
{
   return getZoneString().toLong();
}

void ossimQtImageInfoController::enableDatum(bool flag) const
{
   theDialog->theDatumTextLabel->setEnabled(flag);
   theDialog->theDatumComboBox->setEnabled(flag);
}

void ossimQtImageInfoController::enableUnits(bool flag) const
{
   theDialog->theUnitsTextLabel->setEnabled(flag);
   theDialog->theUnitsComboBox->setEnabled(flag);
}

void ossimQtImageInfoController::enablePixelType(bool flag) const
{
   theDialog->thePixelTypeTextLabel->setEnabled(flag);
   theDialog->thePixelTypeComboBox->setEnabled(flag);
}

void ossimQtImageInfoController::enableZone(bool flag) const
{
   theDialog->theZoneTextLabel->setEnabled(flag);
   theDialog->theZoneComboBox->setEnabled(flag);
}

void ossimQtImageInfoController::enableHemisphere(bool flag) const
{
   theDialog->theHemisphereTextLabel->setEnabled(flag);
   theDialog->theHemisphereComboBox->setEnabled(flag);
}

void ossimQtImageInfoController::enableOriginLatitude(bool flag) const
{
   theDialog->theOriginLatitudeTextLabel->setEnabled(flag);
   theDialog->theOriginLatitudeLineEdit->setEnabled(flag);
}

void ossimQtImageInfoController::enableCentralMeridian(bool flag) const
{
   theDialog->theCentralMeridianTextLabel->setEnabled(flag);
   theDialog->theCentralMeridianLineEdit->setEnabled(flag);
}

void ossimQtImageInfoController::enableFalseEastingNorthing(bool flag) const
{
   theDialog->theFalseEastingTextLabel->setEnabled(flag);
   theDialog->theFalseEastingLineEdit->setEnabled(flag);
   theDialog->theFalseNorthingTextLabel->setEnabled(flag);
   theDialog->theFalseNorthingLineEdit->setEnabled(flag);
}

void ossimQtImageInfoController::enableTiePoints(bool flag) const
{
   theDialog->theTiePointX_TextLabel->setEnabled(flag);
   theDialog->theTiePointX_LineEdit->setEnabled(flag);
   theDialog->theTiePointY_TextLabel->setEnabled(flag);
   theDialog->theTiePointY_LineEdit->setEnabled(flag);
}

void ossimQtImageInfoController::enableScale(bool flag) const
{
   theDialog->theScaleX_TextLabel->setEnabled(flag);
   theDialog->theScaleX_LineEdit->setEnabled(flag);
   theDialog->theScaleY_TextLabel->setEnabled(flag);
   theDialog->theScaleY_LineEdit->setEnabled(flag);
}

void ossimQtImageInfoController::enableScaleFactor(bool flag) const
{
   theDialog->theScaleFactorTextLabel->setEnabled(flag);
   theDialog->theScaleFactorLineEdit->setEnabled(flag);
}

void ossimQtImageInfoController::enableParallels(bool flag) const
{
   // Enable parallels.
   theDialog->theParallelOneTextLabel->setEnabled(flag);
   theDialog->theParallelOneLineEdit->setEnabled(flag);
   theDialog->theParallelTwoTextLabel->setEnabled(flag);
   theDialog->theParallelTwoLineEdit->setEnabled(flag);
}

void ossimQtImageInfoController::enableAllProjectionPanel(bool flag) const
{
   enableDatum(flag);
   enableUnits(flag);
   enablePixelType(flag);
   enableZone(flag);
   enableHemisphere(flag);
   enableOriginLatitude(flag);
   enableCentralMeridian(flag);
   enableFalseEastingNorthing(flag);
   enableTiePoints(flag);
   enableScale(flag);
   enableScaleFactor(flag);
   enableParallels(flag);
}

void ossimQtImageInfoController::freezeLines(bool flag) const
{
   theDialog->theLinesLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::freezeSamples(bool flag) const
{
   theDialog->theSamplesLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::freezeBands(bool flag) const
{
   theDialog->theBandsLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::freezeReducedResLevel(bool flag) const
{
   theDialog->theRLevelLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::freezeMin(bool flag) const
{
   theDialog->theMinLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::freezeMax(bool flag) const
{
   theDialog->theMaxLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::freezeNull(bool flag) const
{
   theDialog->theNullLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::freezeOrigin(bool flag) const
{
   theDialog->theOriginLatitudeLineEdit->setReadOnly(flag);
   theDialog->theCentralMeridianLineEdit->setReadOnly(flag);   
}

void ossimQtImageInfoController::freezeFalseEastingNorthing(bool flag) const
{
   theDialog->theFalseEastingLineEdit->setReadOnly(flag);
   theDialog->theFalseNorthingLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::freezeScaleFactor(bool flag) const
{
   theDialog->theScaleFactorLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::freezeParallels(bool flag) const
{
   theDialog->theParallelOneLineEdit->setReadOnly(flag);
   theDialog->theParallelTwoLineEdit->setReadOnly(flag);
}

void ossimQtImageInfoController::saveProjection() const
{
   if (!theDialog)
   {
      return;
   }

   ossimString os; // Used throughout.
   double       d; // Used throughout.

   // Get the projection type.
   const ossimString PROJ = getProjectionString();
   
   //---
   // Check the type.  It it's not a map projection then we can't do
   // anything.
   //---
   if ( (PROJ == "Unknown") || (PROJ.contains("Sensor Model")) )
   {
      QString caption("Sorry:");
      QString text = "You cannot save a ";
      text += PROJ.c_str();
      QMessageBox::information( theDialog,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }
   
   // Build the keyword list up.
   ossimKeywordlist kwl;

   // Add the projection type.
   kwl.add(ossimKeywordNames::TYPE_KW, PROJ);

   // Add the datum.
   kwl.add(ossimKeywordNames::DATUM_KW, getDatumCode());

   // Add the pixel type.
   if (getPixelTypeString().contains("point"))
   {
      kwl.add(ossimKeywordNames::PIXEL_TYPE_KW, "point");
   }
   else
   {
      kwl.add(ossimKeywordNames::PIXEL_TYPE_KW, "area");
   }
  
   // Add the zone.
   os = getZoneString();
   if (os.size())
   {
      kwl.add(ossimKeywordNames::ZONE_KW, os);
   }

   // Add the hemisphere.
   os = getHemisphereString();
   if (os.size())
   {
      kwl.add(ossimKeywordNames::HEMISPHERE_KW, os);
   }

   // Add the origin latitude.
   d = getDegrees(getOriginLatitudeString(), true);
   kwl.add(ossimKeywordNames::ORIGIN_LATITUDE_KW, d);

   // Add the central meridian.
   d = getDegrees(getCentralMeridianString(), false);
   kwl.add(ossimKeywordNames::CENTRAL_MERIDIAN_KW, d);

   // Add the false easting.
   os = getFalseEastingString();
   if (os.size())
   {
      kwl.add(ossimKeywordNames::FALSE_EASTING_KW, os);
   }

   // Add the false northing.
   os = getFalseNorthingString();
   if (os.size())
   {
      kwl.add(ossimKeywordNames::FALSE_NORTHING_KW, os);
   }

   //---
   // Must have the tie points and scale for all projections.
   //---
   if (isGeographic())
   {
      // Longitude tie point.
      d = getDegrees(getTiePointX_String(), false);
      kwl.add(ossimKeywordNames::TIE_POINT_LON_KW, d);

      // Latitude tie point.
      d = getDegrees(getTiePointY_String(), true);
      kwl.add(ossimKeywordNames::TIE_POINT_LAT_KW, d);

      // Longitude scale.
      d = getDegrees(getScaleX_String(), false);
      kwl.add(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON, d);

      // Latitude scale.
      d = getDegrees(getScaleY_String(), true);
      kwl.add(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT, d);
   }
   else
   {
      // Easting tie point.
      kwl.add(ossimKeywordNames::TIE_POINT_EASTING_KW, getTiePointX_String());

      // Northing tie point.
      kwl.add(ossimKeywordNames::TIE_POINT_NORTHING_KW, getTiePointY_String());
      
      // Easting scale.
      kwl.add(ossimKeywordNames::METERS_PER_PIXEL_X_KW, getScaleX_String());

      // Northing scale.
      kwl.add(ossimKeywordNames::METERS_PER_PIXEL_Y_KW, getScaleY_String());
   }
 
   // Add the scale factor.
   os = getScaleFactorString();
   if (os.toDouble() > 0.0) // Avoid divide by zero.
   {
      kwl.add(ossimKeywordNames::SCALE_FACTOR_KW, os);
   }

   // Add the standard parallel one.
   os = getParallelOneString();
   if (os.size())
   {
      kwl.add(ossimKeywordNames::STD_PARALLEL_1_KW, os);
   }

   // Add the standard parallel two.
   os = getParallelTwoString();
   if (os.size())
   {
      kwl.add(ossimKeywordNames::STD_PARALLEL_2_KW, os);
   }

   //---
   // All through with keyword.  Let's verify we can get a projection out of
   // it.
   //---
   ossimRefPtr<ossimProjection> proj = 
      ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   if (!proj.valid())
   {
      QString caption("Sorry:");
      QString text = "Your keyword list failed to make a projection.";
      QMessageBox::information( theDialog,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }
   
   QString caption("Question:");
   QString text = "Are you sure you want to change the image's projection?";
   int answer = QMessageBox::question( theDialog,
                                       caption,
                                       text,
                                       QMessageBox::Yes,
                                       QMessageBox::No);
   if (answer == QMessageBox::Yes)
   {
      ossimImageHandler*  ih = getImageHandler();
      ossimImageChain*    ic = getChain();
      if (ih && ic)
      {
         // Save the geometry to a file.
         ossimFilename geomFile = ih->getFilename();
         geomFile.setExtension("geom");
         QString qs = geomFile.c_str();
         
         Q3FileDialog* fd = new Q3FileDialog( theDialog );
         fd->setMode(Q3FileDialog::AnyFile);
         fd->setSelection(qs);
         if (fd->exec() == QDialog::Accepted )
         {
            qs = fd->selectedFile();
            geomFile = qs.ascii();

            // Don't let the user select the image.
            if (geomFile != ih->getFilename())
            {
               if ( kwl.write(geomFile.c_str()) )
               {
                  ossimImageGeometry* geom = new ossimImageGeometry;
                  geom->loadState(kwl);
                  ih->setImageGeometry(geom); // provides mem management for geom instance
                  ic->initialize();
                  ossimQtDisplayUtility::flushAllOutputs(ic);
               }
               else
               {
                  QString caption("Write failed!");
                  QString text = "Check file permissions.";
                  QMessageBox::information( theDialog,
                                            caption,
                                            text,
                                            QMessageBox::Ok );
               }
            }
            else
            {
               QString caption("Sorry");
               QString text = "I won't let you overwrite the image file.";
               QMessageBox::information( theDialog,
                                         caption,
                                         text,
                                         QMessageBox::Ok );
            }
         }
         
         // Delete fd???
         delete fd;
         fd = NULL;
      }
   }
   else
   {
      QString caption("Save canceled:");
      QString text = "No action taken.";
      QMessageBox::information( theDialog,
                                caption,
                                text,
                                QMessageBox::Ok );
   }
}

void ossimQtImageInfoController::resetProjection()
{
   if (!theDialog)
   {
      return;
   }

   if (theCustomDialog)
   {
      QString caption("Question:");
      QString text = "You currently have a dialog ";
      text += theCustomDialog->name();
      text += "open.\n";
      text += "Kill it?";
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::Yes)
      {
         theCustomDialog->close(true);
         theCustomDialog = NULL;
      }
      else
      {
         QString caption("Reset canceled:");
         QString text = "No action taken.";
         QMessageBox::information( theDialog,
                                   caption,
                                   text,
                                   QMessageBox::Ok );
         return;
      }
   }

      // popup question???
      
   
   ossimImageHandler* ih = getImageHandler();
   if (!ih)
   {
      return;
   }

   ossimString s;
   updateProjectionPanel(ih->getImageGeometry().get());   
}

void ossimQtImageInfoController::originLineEditActivated() const
{
   ossimString s = getOriginLatitudeString();
   double d = getDegrees(s, true);
   setOriginLatitude(d);
}

void ossimQtImageInfoController::centralMeridianLineEditActivated() const
{
   ossimString s = getCentralMeridianString();
   double d = getDegrees(s, false);
   setCentralMeridian(d);
}

void ossimQtImageInfoController::tiePointX_activated() const
{
   if (isGeographic())
   {
      ossimString s = getTiePointX_String();
      double d = getDegrees(s, false);
      setTiePointX(d);
   }

   // Nothing to do if not geographic.
}

void ossimQtImageInfoController::tiePointY_activated() const
{
   if (isGeographic())
   {
      ossimString s = getTiePointY_String();
      double d = getDegrees(s, false);
      setTiePointY(d);
   }

   // Nothing to do if not geographic.   
}

void ossimQtImageInfoController::parallelOneLineEditActivated() const
{
   ossimString s = getParallelOneString();
   double d = getDegrees(s, true);
   setParallelOne(d);
}

void ossimQtImageInfoController::parallelTwoLineEditActivated() const
{
   ossimString s = getParallelOneString();
   double d = getDegrees(s, true);
   setParallelTwo(d);   
}

void ossimQtImageInfoController::customDialogHasClosed()
{
   if (theCustomDialog)
   {
      theCustomDialog = NULL;
   }
}

// Private to disallow copy constructor, assignment operator use...
ossimQtImageInfoController::ossimQtImageInfoController(const ossimQtImageInfoController&)
   :
      ossimConnectableObjectListener(),
      theDialog(NULL),
      theWidget(NULL),
      theCustomDialog(NULL),
      theCallBackDisabled(false)
{
}

ossimQtImageInfoController& ossimQtImageInfoController::operator=(const ossimQtImageInfoController&)
{
   return *this;
}
