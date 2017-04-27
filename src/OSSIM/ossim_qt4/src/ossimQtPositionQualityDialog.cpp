//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Tabbed dialog box for the position quality evaluator.  Contains two widgets
// or tabs, the "Quality Evaluator" and the "Report Generator".
//
// $Id$
//----------------------------------------------------------------------------

// Define for proprietary inserts. Uncomment only if you have them.
// #define HAS_PQE_INSERTS

#include <sstream>

#include <QtGui/QFileDialog>
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

#include <ossimQt/ossimQtPositionQualityDialog.h>
#include <ossimQt/ossimQtImageWindowController.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossimQt/ossimQtDisplayUtility.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlAttribute.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/imaging/ossimGeoAnnotationSource.h>
#include <ossim/imaging/ossimGeoAnnotationEllipseObject.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <ossim/projection/ossimRpcModel.h>
#include <ossim/projection/ossimSensorModel.h>
#include <ossim/projection/ossimSensorModelTuple.h>
#include <ossim/projection/ossimPositionQualityEvaluator.h>
#include <ossim/elevation/ossimElevationAccuracyInfo.h>
#include <time.h>
#ifdef HAS_PQE_INSERTS
#  include <insertion-code/pqeInsert3.h>
#endif

static ossimTrace traceDebug(
   ossimString("ossimQtPositionQualityDialog:debug"));

//---
// Must match ossimPositionQualityEvaluator enumeration as it will be used as
// index to this array. Last cell must be '0' for qt combo box.
//---
static const char* PROBABILITY_LEVEL[] =
{ "ONE SIGMA", "P50", "P90", "P95", 0 };

static const int DEFAULT_PROBABILITY_ITEM_INDEX = 3; // "P95"

static const ossimString LAT_FORMAT = "dd@ mm' ss.ssss\" C";
static const ossimString LON_FORMAT = "ddd@ mm' ss.ssss\" C";

//---
// This is the name used in the "Elevation Accuracy" pulldown if no
// "pqe_elev" keywords are found in the ossim preferences file.
//---
static const ossimString ELEV_SOURCE_IGNORE_NAME = "n/a";

static const QString ANNOTATED_CHIP_OPTION = "Annotated Chip";
static const QString ORP_OPTION            = "OSSIM Report (ORP)";

ossimPqeAccSrc::ossimPqeAccSrc()
   : theName(),
     theCe90(0.0),
     theLe90(0.0)
{}

std::ostream& operator<<(std::ostream& out, const ossimPqeAccSrc& obj)
{
   out << "ossimPqeAccSrc"
       << "\ntheName: " << obj.theName
       << "\ntheCe90: " << obj.theCe90
       << "\ntheLe90: " << obj.theLe90
       << std::endl;
   return out;
}

ossimQtPositionQualityDialog::ossimQtPositionQualityDialog(QWidget *parent)
   : QDialog(parent, Qt::WDestructiveClose), // Set to have destructive close.
     theTabWidget(0),
     theQualityEvaluatorTab(0),
     theReportGeneratorTab(0)
{
   theTabWidget = new QTabWidget;

   theQualityEvaluatorTab = new ossimQtQualityEvaluatorTab();
   theTabWidget->addTab(theQualityEvaluatorTab, tr("Quality Evaluator"));

   theReportGeneratorTab = new ossimQtReportGeneratorTab();
   theTabWidget->addTab(theReportGeneratorTab, tr("Report Generator"));

   // For communication with to/from tabs.
   theReportGeneratorTab->setQualityEvaluatorTab(theQualityEvaluatorTab);

   QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
   
   QVBoxLayout* mainLayout = new QVBoxLayout;
   mainLayout->addWidget(theTabWidget);
   mainLayout->addWidget(buttonBox);
   setLayout(mainLayout);
   
   setWindowTitle(tr("Position Quality Evaluator Dialog"));
}

ossimQtPositionQualityDialog::~ossimQtPositionQualityDialog()
{
}

void ossimQtPositionQualityDialog::setImageWidget(
   ossimQtScrollingImageWidget* widget)
{
   theQualityEvaluatorTab->setImageWidget(widget);
}

void ossimQtPositionQualityDialog::setController(
   ossimQtImageWindowController* controller)
{
   theReportGeneratorTab->setController(controller);
}

ossimQtQualityEvaluatorTab::ossimQtQualityEvaluatorTab(QWidget *parent)
   : QWidget(parent),
     theWidget(0),
     
     thePqeEnabledCheckBox(0),
     theGraphicsEnabledCheckBox(0),
     
     theSemiMajorAxisLineEdit(0),
     theSemiMinorAxisLineEdit(0),
     theAzimuthLineEdit(0),
     
     theCircularErrorLineEdit(0),
     theLinearErrorLineEdit(0),
     
     theLatitudeLineEdit(0),
     theLongitudeLineEdit(0),
     theHeightAboveEllipsoidLineEdit(0),
     theHeightAboveMslLineEdit(0),
     
     theElevSourceLineEdit(0),
     
     theProbabilityLevelComboBox(0),
     theBackdropAccuracyComboBox(0),
     theElevationAccuracyComboBox(0),

     theGroundPt(),
     theImagePt(),
     theCovMat(3, 3),
     theHeightAboveEllipsoid(0.0),
     theHeightAboveMsl(0.0),
     theSemiMinorAxis(0.0),
     theSemiMajorAxis(0.0),

     //---
     // Set the nan as an indication to the save routine that the user has NOT
     // dropped a point yet.
     //---  
     theAzimuth(ossim::nan()),

     theCe90(0.0),
     theLe90(0.0),

     thePqeEnabledFlag(true),
     theGraphicsEnabledFlag(true),
     
     theEllipse(0),
     theAnnotator(0),
     
     theBackdropSources(),
     theElevationSources(),

     theRpcPqeInputs()
{
   // row 1 start
   QGroupBox*   hBoxRow1 = new QGroupBox();
   QHBoxLayout* layout1   = new QHBoxLayout;
   thePqeEnabledCheckBox = new QCheckBox();
   thePqeEnabledCheckBox->setText(QString("PQE Mouse Entry Enabled"));
   thePqeEnabledCheckBox->setChecked(thePqeEnabledFlag);
   theGraphicsEnabledCheckBox = new QCheckBox();
   theGraphicsEnabledCheckBox->setText(QString("Graphics Overlay Enabled"));
   theGraphicsEnabledCheckBox->setChecked(theGraphicsEnabledFlag);
   layout1->addWidget(thePqeEnabledCheckBox);
   layout1->addWidget(theGraphicsEnabledCheckBox);
   hBoxRow1->setLayout(layout1);
   // row 1 end

   // row 2 start
   QGroupBox* hBoxRow2 = new QGroupBox();

   // row 2 will be a 2x2 grid
   QGridLayout* r2lo = new QGridLayout;

   // box for row 2 column 1 row 1(r2c1r1)
   QGroupBox* r2c1r1 = new QGroupBox(tr("Position"));
   QGridLayout* r2c1r1lo = new QGridLayout;

   // latitude
   QLabel* lab1 = new QLabel(tr("Lat:"));
   theLatitudeLineEdit = new QLineEdit();
   theLatitudeLineEdit->setReadOnly(true);
   
   // longitude
   QLabel* lab2 = new QLabel(tr("Lon:"));
   theLongitudeLineEdit = new QLineEdit();
   theLongitudeLineEdit->setReadOnly(true);
   
   // height above ellipsoid
   QLabel* lab3 = new QLabel(tr("Hgt:"));
   theHeightAboveEllipsoidLineEdit = new QLineEdit();
   theHeightAboveEllipsoidLineEdit->setReadOnly(true);
   
   // height above msl
   QLabel* lab4 = new QLabel(tr("MSL:"));
   theHeightAboveMslLineEdit = new QLineEdit();
   theHeightAboveMslLineEdit->setReadOnly(true);
   
   r2c1r1lo->addWidget(lab1, 0, 0);                            // row 0, col 0
   r2c1r1lo->addWidget(theLatitudeLineEdit, 0, 1);             // row 0, col 1
   r2c1r1lo->addWidget(lab2, 1, 0);                            // row 1, col 0
   r2c1r1lo->addWidget(theLongitudeLineEdit, 1, 1);            // row 1, col 1
   r2c1r1lo->addWidget(lab3, 2, 0);                            // row 2, col 0
   r2c1r1lo->addWidget(theHeightAboveEllipsoidLineEdit, 2, 1); // row 2, col 1
   r2c1r1lo->addWidget(lab4, 3, 0);                            // row 3, col 0
   r2c1r1lo->addWidget(theHeightAboveMslLineEdit, 3, 1);       // row 3, col 1
   r2c1r1->setLayout(r2c1r1lo);

   // box for row 2 column 2 row 1(r2c2r1)
   QGroupBox* r2c2r1 = new QGroupBox(tr("Error Ellipse"));
   QGridLayout* r2c2r1lo = new QGridLayout;

   // Semi-Major Axis:
   QLabel* lab6 = new QLabel(tr("Semi-Major Axis:"));
   theSemiMajorAxisLineEdit= new QLineEdit();
   theSemiMajorAxisLineEdit->setReadOnly(true);

   // Semi-Minor Axis:
   QLabel* lab7 = new QLabel(tr("Semi-Minor Axis:"));
   theSemiMinorAxisLineEdit= new QLineEdit();
   theSemiMinorAxisLineEdit->setReadOnly(true);
   
   // Azimuth:
   QLabel* lab8 = new QLabel(tr("Azimuth:"));
   theAzimuthLineEdit= new QLineEdit();
   theAzimuthLineEdit->setReadOnly(true);

   r2c2r1lo->addWidget(lab6, 0, 0);
   r2c2r1lo->addWidget(theSemiMajorAxisLineEdit, 0, 1);
   r2c2r1lo->addWidget(lab7, 1, 0);
   r2c2r1lo->addWidget(theSemiMinorAxisLineEdit, 1, 1);
   r2c2r1lo->addWidget(lab8, 2, 0);
   r2c2r1lo->addWidget(theAzimuthLineEdit, 2, 1);
   r2c2r1->setLayout(r2c2r1lo);

   // elev source box row 2 column 1 row 2 (r2c1r2)
   QGroupBox* r2c1r2 = new QGroupBox(tr("Model elevation source"));
   QVBoxLayout* r2c1r2lo = new QVBoxLayout;
   theElevSourceLineEdit = new QLineEdit();
   theElevSourceLineEdit->setReadOnly(true);
   r2c1r2lo->addWidget(theElevSourceLineEdit);
   r2c1r2->setLayout(r2c1r2lo);

   // accuracy box (r2c2r2)
   QGroupBox* r2c2r2 = new QGroupBox(tr("Accuracy"));
   QGridLayout* r2c2r2lo = new QGridLayout;

   // Circular Error:
   QLabel* lab9 = new QLabel(tr("Circular Error:"));
   theCircularErrorLineEdit= new QLineEdit();
   theCircularErrorLineEdit->setReadOnly(true);

   // Linear Error:
   QLabel* lab10 = new QLabel(tr("Linear Error:"));
   theLinearErrorLineEdit= new QLineEdit();
   theLinearErrorLineEdit->setReadOnly(true);

   r2c2r2lo->addWidget(lab9, 0, 0);
   r2c2r2lo->addWidget(theCircularErrorLineEdit, 0, 1);
   r2c2r2lo->addWidget(lab10, 1, 0);
   r2c2r2lo->addWidget(theLinearErrorLineEdit, 1, 1);
   r2c2r2->setLayout(r2c2r2lo);

   
   r2lo->addWidget(r2c1r1, 0, 0);
   r2lo->addWidget(r2c2r1, 0, 1);
   r2lo->addWidget(r2c1r2, 1, 0);
   r2lo->addWidget(r2c2r2, 1, 1);
   hBoxRow2->setLayout(r2lo);
   // row 2 end
   
   // Row 3.
   QGroupBox* hBoxRow3 = new QGroupBox();
   QHBoxLayout* hBoxRow3lo = new QHBoxLayout;
   
   QGroupBox* r3c1     = new QGroupBox(tr("Probability Level"));
   QHBoxLayout* r3c1lo = new QHBoxLayout;
   QGroupBox* r3c2     = new QGroupBox(tr("Backdrop Accuracy"));
   QHBoxLayout* r3c2lo = new QHBoxLayout;
   QGroupBox* r3c3     = new QGroupBox(tr("Elevation Accuracy"));
   QHBoxLayout* r3c3lo = new QHBoxLayout;

   theProbabilityLevelComboBox  = new QComboBox();
   theBackdropAccuracyComboBox  = new QComboBox();
   theElevationAccuracyComboBox = new QComboBox();
   QStringList list;
   list << PROBABILITY_LEVEL[0]
        << PROBABILITY_LEVEL[1]
        << PROBABILITY_LEVEL[2]
        << PROBABILITY_LEVEL[3];
   
   theProbabilityLevelComboBox->insertItems(0, list);

   // Set "P95" as the default.
   theProbabilityLevelComboBox->setCurrentItem(DEFAULT_PROBABILITY_ITEM_INDEX);

   buildBackdropPulldown();
   buildElevAccPulldown();

   r3c1lo->addWidget(theProbabilityLevelComboBox);
   r3c1->setLayout(r3c1lo);
   r3c2lo->addWidget(theBackdropAccuracyComboBox);
   r3c2->setLayout(r3c2lo);
   r3c3lo->addWidget(theElevationAccuracyComboBox);
   r3c3->setLayout(r3c3lo);
   

   hBoxRow3lo->addWidget(r3c1);
   hBoxRow3lo->addWidget(r3c2);
   hBoxRow3lo->addWidget(r3c3);
   hBoxRow3->setLayout(hBoxRow3lo);
   // end of row 3
   
   QVBoxLayout *mainLayout = new QVBoxLayout;
   mainLayout->addWidget(hBoxRow1);
   mainLayout->addWidget(hBoxRow2);
   mainLayout->addWidget(hBoxRow3);
   
   setLayout(mainLayout);

      // Connect signals with slots.
   connect(thePqeEnabledCheckBox, SIGNAL( toggled(bool) ),
           this, SLOT( pqeEnabledToggled(bool) ) );
   connect(theGraphicsEnabledCheckBox, SIGNAL( toggled(bool) ),
           this, SLOT( graphicsEnabledToggled(bool) ) );
   connect(theProbabilityLevelComboBox, SIGNAL( activated(int) ),
           this, SLOT( recalc(int) ) );
   connect(theBackdropAccuracyComboBox, SIGNAL( activated(int) ),
           this, SLOT( recalc(int) ) );
   connect(theElevationAccuracyComboBox, SIGNAL( activated(int) ),
           this, SLOT( recalc(int) ) );
}

ossimQtQualityEvaluatorTab::~ossimQtQualityEvaluatorTab()
{
   if (theAnnotator.get())
   {
      removeAnnotatorFromChain();
   }
}

void ossimQtQualityEvaluatorTab::setImageWidget(ossimQtScrollingImageWidget* widget)
{
   theWidget = widget;

   connect(theWidget,
           SIGNAL(scrollingImageWidgetMouseEvent(ossimQtMouseEvent*)),
           this,
           SLOT(trackScrollingImageWidget(ossimQtMouseEvent*)));
}

ossim_float64 ossimQtQualityEvaluatorTab::getAzimuth() const
{
   return theAzimuth;
}

void ossimQtQualityEvaluatorTab::trackScrollingImageWidget(
   ossimQtMouseEvent* event)
{
   if (thePqeEnabledFlag == false)
   {
      event->getEvent()->ignore();
      return;
   }
   
   // Activate on left press only...
   if ( ( event->getButton() == Qt::LeftButton) &&
        ( event->getQtEventType() == QEvent::MouseButtonPress ) )
   {
      // Get the mouse point.
      ossimDpt viewPt = event->getShiftPoint();

      //---
      // Convert the view point to image space for the
      // ossimSensorModelTuple::intersect method.
      //---
      
      ossimImageViewTransform* view = getImageViewTransform();
      if (view)
      {
         // Use the view to convert to image point.
         view->viewToImage(viewPt, theImagePt);
      }

      //---
      // Convert the view point to world space.  This will be the center
      // point of the ellipse.
      //---
      ossimImageGeometry* proj = getViewGeometry();
      if (proj)
      {
         proj->localToWorld(viewPt, theGroundPt);
         initializeDialog();
      }

   } // End of mouse event check.
}

void ossimQtQualityEvaluatorTab::recalc(int /* item */ )
{
   initAccuracyStrings();
}

void ossimQtQualityEvaluatorTab::pqeEnabledToggled(bool flag)
{
   thePqeEnabledFlag = flag;
   // do something here...

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << " pqeEnabledToggled DEBUG:"
         << "\nflag: " << (flag?"true":"false")
         << std::endl;
   }

}

void ossimQtQualityEvaluatorTab::graphicsEnabledToggled(bool flag)
{
   theGraphicsEnabledFlag = flag;
   if (theAnnotator.get())
   {
      theAnnotator->setEnableFlag(flag);
      refresh();
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "graphicsEnabledToggled  DEBUG:"
         << "\nflag: " << (flag?"true":"false")
         << std::endl;
   }
}

void ossimQtQualityEvaluatorTab::initializeDialog()
{
   initPositionStrings();
   
   // Check elevation:
   if (checkElevForPoint(theGroundPt))
   {
      initAccuracyStrings();
   }
}

void ossimQtQualityEvaluatorTab::initPositionStrings()
{
   setLatString();
   setLonString();
   setHgtAboveEllipsoidString();
   setHgtAboveMslString();
   setElevSourceString();
}

void ossimQtQualityEvaluatorTab::initAccuracyStrings()
{
   static const char MODULE[] =
      "ossimQtPositionQualityDialog::initAccuracyStrings";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..." << std::endl;
   }
   
   ossimSensorModel* model = getSensorModel();
   if (model)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Model name: " << model->getClassName() << std::endl;
      }
      
      // Load the models into the tuple
      ossimSensorModelTuple smTuple;
      smTuple.addImage(model);
      
      //---
      // Set the surface accuracy.  User choice of from source data or from
      // preference settings.
      //---
      ossim_float64 elevCE90 = 0.0;
      ossim_float64 elevLE90 = 0.0;
      getCeLe(elevCE90, elevLE90);
      
      if ( elevCE90 && elevLE90 )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE
               << "\nSetting intersection surface accuracy: "
               << "ce=" << elevCE90 << " le=" << elevLE90
               << std::endl;
         }
         smTuple.setIntersectionSurfaceAccuracy(elevCE90, elevLE90);
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE
               << " No surface accuracy used in solution!"
               << std::endl;
         }
      }

      ossimEcefPoint intECF;
      ossimSensorModelTuple::IntersectStatus status =
         smTuple.intersect(0, theImagePt, intECF, theCovMat);
      if (status != ossimSensorModelTuple::ERROR_PROP_FAIL)
      {
         if (isRpcModel())
         {
            // Capture the rpc inputs to pqe.
            smTuple.getRpcPqeInputs(theRpcPqeInputs);
         }
         
         // Instantiate a quality evaluator.
         ossimPositionQualityEvaluator qev(intECF, theCovMat);
         
         // Set theCovMat to ENU for output if necessary
         qev.getCovMatrix(theCovMat);

         // Index 0 is none.
         if ( theBackdropAccuracyComboBox->currentItem() != 0 )
         {
            // Add backdrop contributions.
            int bdSourceIdx = theBackdropAccuracyComboBox->currentItem();
            ossim_float64 bdCE90 = theBackdropSources[bdSourceIdx].theCe90;
            ossim_float64 bdLE90 = theBackdropSources[bdSourceIdx].theLe90;
            qev.addContributingCE_LE(bdCE90, bdLE90);

            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << theBackdropSources[bdSourceIdx]
                  << std::endl;
            }
         }

         //---
         // Accuracy results processing
         //---

         pqeProbLev_t pLev = static_cast<pqeProbLev_t>(
            theProbabilityLevelComboBox->currentItem());

         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG:"
               << "\nProbability level:  " << PROBABILITY_LEVEL[pLev]
               << std::endl;
         }
         
         theCe90 = 0.0;
         theLe90 = 0.0;
         
         // Get the accuracy estimate.
         bool epOK = qev.computeCE_LE(pLev, theCe90, theLe90);

         if (epOK)
         {
            // Get the error ellipse.
            pqeErrorEllipse ell;
            if (qev.extractErrorEllipse(pLev, ell))
            {
               theSemiMinorAxis = ell.theSemiMinorAxis;
               theSemiMajorAxis = ell.theSemiMajorAxis;               
               theAzimuth       = ell.theAzimAngle*DEG_PER_RAD;

               ossimString os;

               os = ossimString::toString(theSemiMajorAxis, 2);
               os += " m";
               theSemiMajorAxisLineEdit->setText(os.c_str());

               os = ossimString::toString(theSemiMinorAxis, 2);
               os += " m";
               theSemiMinorAxisLineEdit->setText(os.c_str());

               os = ossimString::toString(theAzimuth, 2);
               os += " deg";
               theAzimuthLineEdit->setText(os.c_str());

               os = ossimString::toString(theCe90, 2);
               os += " m";
               theCircularErrorLineEdit->setText(os.c_str());

               os = ossimString::toString(theLe90, 2);
               os += " m";
               theLinearErrorLineEdit->setText(os.c_str());

               // Update the ellipse graphic.
               updateEllipse();
            }
            else
            {
               QString caption = "ERROR:";
               QString text = "Error returned from ossimPositionQualityEvaluator::extractErrorEllipse method.";
               QMessageBox::information( this,
                                         caption,
                                         text,
                                         QMessageBox::Ok );
            }
            
         } // if (epOK)
         
      } // if (status != ossimSensorModelTuple::ERROR_PROP_FAIL)
   }
   else
   {
      QString caption = "Sensor model required:";
      QString text = "Cannot derive position quality from this data.";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exited..." << std::endl;
   }
}

void ossimQtQualityEvaluatorTab::setLatString()
{
   ossimDms dms(theGroundPt.latd(), true);
   theLatitudeLineEdit->setText( dms.toString(LAT_FORMAT).c_str() );
}

void ossimQtQualityEvaluatorTab::setLonString()
{
   ossimDms dms(theGroundPt.lond(), false);
   theLongitudeLineEdit->setText( dms.toString(LON_FORMAT).c_str() );
}

void ossimQtQualityEvaluatorTab::setHgtAboveEllipsoidString()
{
   theHeightAboveEllipsoid =
      ossimElevManager::instance()->getHeightAboveEllipsoid(theGroundPt);
   
   if ( ossim::isnan(theHeightAboveEllipsoid) )
   {
      theHeightAboveEllipsoidLineEdit->setText("nan");
   }
   else
   {
      ossimString os = ossimString::toString(theHeightAboveEllipsoid, 2);
      os += " m";
      theHeightAboveEllipsoidLineEdit->setText(os.c_str());
   }
}

void ossimQtQualityEvaluatorTab::setHgtAboveMslString()
{
   theHeightAboveMsl =
      ossimElevManager::instance()->getHeightAboveMSL(theGroundPt);

   if ( ossim::isnan(theHeightAboveMsl) )
   {
      theHeightAboveMslLineEdit->setText("nan");
   }
   else
   {
      ossimString os = ossimString::toString(theHeightAboveMsl, 2);
      os += " m";
      theHeightAboveMslLineEdit->setText(os.c_str());
   }
}

void ossimQtQualityEvaluatorTab::setElevSourceString()
{
#if 0
   ossimFilename f = ossimElevManager::instance()->
      getCellFilenameForPoint(theGroundPt);
   theElevSourceLineEdit->setText(f.c_str());
#endif
}

void ossimQtQualityEvaluatorTab::buildBackdropPulldown()
{
   if (!theBackdropAccuracyComboBox)
   {
      return;
   }

   //---
   // Keyword example "name,ce90,le90":
   // pqe_backdrop1: JOG-A,25,25
   //---

   // Start with clean list.
   theBackdropSources.clear();
   
   // Put "None" as the first item.
   ossimPqeAccSrc src;
   src.theName = "None";
   src.theCe90 = 1.0;
   src.theLe90 = 1.0;
   theBackdropSources.push_back(src);

   // Add sources from ossim preferences.
   ossimString keyBase = "pqe_backdrop";
   buildSourceList(keyBase, theBackdropSources);

   std::vector<ossimPqeAccSrc>::const_iterator i = theBackdropSources.begin();
   while (i != theBackdropSources.end())
   {
      // Insert into pulldown.
      theBackdropAccuracyComboBox->insertItem((*i).theName.c_str());
      ++i;
   }
}

void ossimQtQualityEvaluatorTab::buildElevAccPulldown()
{
   if (!theElevationAccuracyComboBox)
   {
      return;
   }

   //---
   // Keyword example "name,ce90,le90":
   // pqe_backdrop1: JOG-A,25,25
   //---

   // Start with a clean list.
   theElevationSources.clear();

   ossimPqeAccSrc src;
   src.theName = "source";
   theElevationSources.push_back(src);
   
   // Add sources from ossim preferences.
   ossimString keyBase = "pqe_elev";
   buildSourceList(keyBase, theElevationSources);

   // If no sources were in the preferences put one in with an n/a.
   if (theElevationSources.size() == 1)
   {
      ossimPqeAccSrc src;
      src.theName = ELEV_SOURCE_IGNORE_NAME;
      theElevationSources.push_back(src);
   }

   std::vector<ossimPqeAccSrc>::const_iterator i = theElevationSources.begin();
   while (i != theElevationSources.end())
   {
      // Insert into pulldown.
      theElevationAccuracyComboBox->insertItem((*i).theName.c_str());
      ++i;
   }
}

void ossimQtQualityEvaluatorTab::buildSourceList(
   const ossimString& keyBase, std::vector<ossimPqeAccSrc> & result)
{
   const ossimKeywordlist KWL = ossimPreferences::instance()->preferencesKWL();

   ossim_uint32 numberOfSources = KWL.numberOf(keyBase.c_str());
   ossim_uint32 found = 0;
   ossim_uint32 idx = 0;
   const ossim_uint32 MAX = numberOfSources + 10;
   while (found <  numberOfSources)
   {
      ossimString key = keyBase;
      key += ossimString::toString(idx);
      const char* lookup = KWL.find(key);
      if (lookup)
      {
         ossimString lookupString = lookup;
         
         // split on commas name,ce90,le90
         std::vector<ossimString> value;
         lookupString.split(value, ossimString(","));
         
         if (value.size() == 3)
         {
            ossimPqeAccSrc src;
            src.theName = value[0];
            src.theCe90 = value[1].toFloat64();
            src.theLe90 = value[2].toFloat64();
            
            // Store it.
            result.push_back(src);
            
            ++found;
         }
      }
      ++idx;

      // Since our while loop can skip numbers add sanity check.
      if ( idx > MAX) 
      {
         break;
      }
   }
}

void ossimQtQualityEvaluatorTab::updateLatitudeField()
{
   if (theLatitudeLineEdit)
   {
      QString     qs     = theLatitudeLineEdit->text();
      ossimString os     = qs.ascii();
      theGroundPt.latd( getDegrees(os, true) );
      setLatString();
   }
}

ossimImageChain* ossimQtQualityEvaluatorTab::getChain() const
{
   ossimImageChain* ic = 0;
   if (theWidget)
   {
      ossimConnectableObject* obj = theWidget->getInput(0);
      if (obj)
      {
         ic = PTR_CAST(ossimImageChain, obj);
      }
      else
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL ossimQtImageChainEditorController::getChain: "
            << "getInput failed!" << std::endl;
      }
   }
   return ic;
}

ossimImageHandler* ossimQtQualityEvaluatorTab::getImageHandler() const
{
   ossimImageHandler* handler = 0;
   ossimImageChain* ic = getChain();
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

ossimImageViewTransform* ossimQtQualityEvaluatorTab::getImageViewTransform() const
{
   ossimImageViewTransform* trans = 0;

   ossimImageChain* ic = getChain();
   if (ic)
   {
      // ossimImageRenderer* obj = (ossimImageRenderer*)ic->
      // findObjectOfType("ossimImageRenderer",
      // ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT);

      ossimTypeNameVisitor visitor( ossimString("ossimImageRender"),
                                    true, // firstofTypeFlag
                                    (ossimVisitor::VISIT_INPUTS |
                                     ossimVisitor::VISIT_CHILDREN) );
      ic->accept( visitor );
      ossimRefPtr<ossimImageRenderer> obj =  visitor.getObjectAs<ossimImageRenderer>(0);
      if( obj.valid() )
      {
         trans = obj->getImageViewTransform();
      }
   }
   return trans;
}

ossimImageGeometry* ossimQtQualityEvaluatorTab::getImageGeometry() const
{
   ossimImageViewTransform* trans = getImageViewTransform();
   if (trans)
   {
      ossimImageViewProjectionTransform* ivpt
         = PTR_CAST(ossimImageViewProjectionTransform, trans);
      if(ivpt)
      {
         return ivpt->getImageGeometry();
      }
   }
   
   return 0;
}

ossimImageGeometry* ossimQtQualityEvaluatorTab::getViewGeometry() const
{
   ossimImageViewTransform* trans = getImageViewTransform();
   if (trans)
   {
      ossimImageViewProjectionTransform* ivpt
         = PTR_CAST(ossimImageViewProjectionTransform, trans);
      if(ivpt)
      {
         return ivpt->getViewGeometry();
      }
   }
   
   return 0;
}

ossimSensorModel* ossimQtQualityEvaluatorTab::getSensorModel() const
{
   ossimSensorModel* model = 0;
   
   ossimImageGeometry* geom = getImageGeometry();
   if (geom&&geom->getProjection())
   {
      model = dynamic_cast<ossimSensorModel*>(geom->getProjection());
   }
   
   return model;
}

ossimGeoAnnotationEllipseObject* ossimQtQualityEvaluatorTab::getEllipse() const
{
   ossimGeoAnnotationEllipseObject* ellipse = 0;
   
   ossimImageChain* ic = getChain();
   if (ic)
   {
      // ellipse = (ossimGeoAnnotationEllipseObject*)ic->
      //    findObjectOfType("ossimGeoAnnotationEllipseObject",
      //                     ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT);

      ossimTypeNameVisitor visitor( ossimString("ossimGeoAnnotationEllipseObject"),
                                    true, // firstofTypeFlag
                                    (ossimVisitor::VISIT_INPUTS |
                                     ossimVisitor::VISIT_CHILDREN) );
      ic->accept( visitor );
      ellipse =  visitor.getObjectAs<ossimGeoAnnotationEllipseObject>(0);
   }
   
   return ellipse;
}

void ossimQtQualityEvaluatorTab::addAnnotatorToChain()
{
   if (!theAnnotator)
   {
      theAnnotator = new ossimGeoAnnotationSource();
      ossimImageGeometry* geom = getViewGeometry();
      if (geom)
         theAnnotator->setImageGeometry(geom); // false for does not own it.

      ossimImageChain* ic = getChain();
      if (ic)
      {
         ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject,
                                                theAnnotator.get());
         if (obj)
         {
            //---
            // This is the most right object in the chain with the image
            // handler being the left most object.
            //---
            ic->addFirst(obj);
         }
      }

      // enable/disable
      theAnnotator->setEnableFlag(theGraphicsEnabledFlag);
   }
}

void ossimQtQualityEvaluatorTab::removeAnnotatorFromChain()
{
   if (theAnnotator.get())
   {
      ossimImageChain* ic = getChain();
      if (ic)
      {
         ossimConnectableObject* obj =
            PTR_CAST(ossimConnectableObject, theAnnotator.get());
         if (obj)
         {
            ic->removeChild(obj);
            if (theWidget)
            {
               refresh();
            }
         }
      }
   }
}

void ossimQtQualityEvaluatorTab::updateEllipse()
{
   if (!theAnnotator) // First time through.
   {
      addAnnotatorToChain();
   }

   if (!theEllipse) // First time through.
   {
      theEllipse = new ossimGeoAnnotationEllipseObject();

      // Setup the options, fill, thickness, axes and color.
      setEllipseOptions();

      // Parent to theAnnotator.  The annotator handles memory.
      theAnnotator->addObject(theEllipse);
   }

   theEllipse->setCenter(theGroundPt);
   theEllipse->setEllipseWidthHeightUnitType(OSSIM_METERS);

   // We use "Width" for "Minor" and "Height" for "Major".
   theEllipse->setWidthHeight(ossimDpt(theSemiMinorAxis,theSemiMajorAxis));
   theEllipse->setAzimuth(theAzimuth);
   ossimImageGeometry* geom = getViewGeometry();
   if (geom)
   {
      theAnnotator->transformObjects(geom);
   }
   
   refresh();
}

double ossimQtQualityEvaluatorTab::getDegrees(const ossimString& s,
                                              bool lat_flag) const
{
   ossimDms dms(0.0, lat_flag);
   dms.setDegrees(s.c_str());
   return dms.getDegrees();
}

void ossimQtQualityEvaluatorTab::refresh()
{
   ossimImageChain* ic = getChain();
   if (ic)
   {
      ossimQtDisplayUtility::flushAllOutputs(ic);
   }
}

void ossimQtQualityEvaluatorTab::setEllipseOptions()
{
   if (!theEllipse)
   {
      return;
   }

   // Default color to white.
   ossim_uint8 r = 255;
   ossim_uint8 g = 255;
   ossim_uint8 b = 255;
   
   // Look for pen color in preferences.
   const char* lookup = ossimPreferences::instance()->
      findPreference("pqe_normalized_rgb_ellipse_color");
   
   if (lookup)
   {
      ossim_float64 rNorm;
      ossim_float64 gNorm;
      ossim_float64 bNorm;

      std::istringstream in(lookup);
      in >> rNorm >> gNorm >> bNorm;
      
      if ( (rNorm >= 0.0) && (rNorm <=1.0) )
      {
         r = static_cast<ossim_uint8>(rNorm * 255.0 + 0.5);
      }
      if ( (gNorm >= 0.0) && (gNorm <=1.0) )
      {
         g = static_cast<ossim_uint8>(gNorm * 255.0 + 0.5);
      }
      if ( (bNorm >= 0.0) && (bNorm <=1.0) )
      {
         b = static_cast<ossim_uint8>(bNorm * 255.0 + 0.5);
      }
   }
   
   theEllipse->setFillFlag(false);
   theEllipse->setThickness(1);
   theEllipse->setDrawAxesFlag(true);
   theEllipse->setColor(r, g, b);
}

bool ossimQtQualityEvaluatorTab::checkElevForPoint(const ossimGpt& gpt)
{
   //---
   // At a minimum, users should have a geoid grid with the offset from
   // the ellipsiod.  Consider this an error and return false if a nan is
   // picked up.
   //---
   ossim_float64 d = ossimGeoidManager::instance()->offsetFromEllipsoid(gpt);
   if ( ossim::isnan(d) )
   {
      QString caption = "Elevation ERROR:";
      QString text = "\
Check for elevation coverage.\n\
Minimum of geoid grid coverage required!";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      return false;
   }

   //---
   // Send warning to user if no height above msl.
   // Return true though as the image could be over the ocean.
   //---
   d = ossimElevManager::instance()->getHeightAboveMSL(gpt);
   if ( ossim::isnan(d) )
   {
      QString caption = "Elevation WARNING:";
      QString text = "\
No height above mean sea level found!\n\
Over ocean perhaps?";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
   }   

   return true;
}

void ossimQtQualityEvaluatorTab::getCeLe(ossim_float64& ce,
                                         ossim_float64& le)
{
   int elevSourceIdx = theElevationAccuracyComboBox->currentItem();

   if ( theElevationSources[elevSourceIdx].theName == "source")
   {
      getSourceCeLe(ce, le);
   }
   else
   {
      getDemoCeLe(ce, le);
   }
}

void ossimQtQualityEvaluatorTab::getSourceCeLe(ossim_float64& ce,
                                               ossim_float64& le)
{
  ossimElevationAccuracyInfo info;

  if(ossimElevManager::instance()->getAccuracyInfo(info, theGroundPt))
  {
    ce = info.m_absoluteCE;
    le = info.m_absoluteLE;
    if (traceDebug())
    {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQtQualityEvaluatorTab::getSourceCeLe"
         << "\nSurface accuracy derived from source elevation: "
         << "\nce=" << ce << " le=" << le
         << std::endl;
    }
  }
  else
  {
      QString caption = "WARNING:";
      QString text = "Elevation source does not have accuracy info.";
      text += "\nNo surface accuracy used in solution!";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
  }
}

void ossimQtQualityEvaluatorTab::getDemoCeLe(ossim_float64& ce,
                                               ossim_float64& le) const
{
   int elevSourceIdx = theElevationAccuracyComboBox->currentItem();

   // Get the circular error.
   ce = theElevationSources[elevSourceIdx].theCe90;

   // Get the linear error.
   le = theElevationSources[elevSourceIdx].theLe90;

      if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQtQualityEvaluatorTab::getDemoCeLe"
         << "\nSurface accuracy derived from ossim preferences demo settings:"
         << "\nce=" << ce << " le=" << le
         << std::endl;
   }
}

ossimString ossimQtQualityEvaluatorTab::getImageName(
   bool baseNameOnlyFlag) const
{
   ossimString result;
   ossimImageHandler* handler = getImageHandler();
   if (handler)
   {
      if (baseNameOnlyFlag)
      {
         result = handler->getFilename().file();
      }
      else
      {
         result = handler->getFilename();
      }
   }
   return result;
}

ossimString ossimQtQualityEvaluatorTab::getUncertaintyType() const
{
   if (isRpcModel())
   {
      return ossimString("rpc");
   }
   return ossimString("cov");
}

bool ossimQtQualityEvaluatorTab::isRpcModel() const
{
   ossimSensorModel* sensor = getSensorModel();
   if (sensor)
   {
      ossimRpcModel* rpc = PTR_CAST(ossimRpcModel, sensor);
      if (rpc)
      {
         return true;
      }
   }
   return false;
}

bool ossimQtQualityEvaluatorTab::saveToFile(const ossimFilename& f,
                                            bool concatenateFlag) const
{
   const ossimString ROOT_TAG        = "PqeReport";
   const ossimString UPDATE_TIME_TAG = "pqeUpdateTime";
   const ossimString IMAGE_TAG       = "image";
   const ossimString PQE_POINT_TAG   = "pqePoint";
   const ossimString GML_POINT_TAG   = "gml:Point";

   ossimXmlDocument doc;
   ossimRefPtr<ossimXmlNode> rootNode   = 0;
   ossimRefPtr<ossimXmlNode> timeNode   = 0;
   ossimRefPtr<ossimXmlNode> imageNode  = 0;
   ossimRefPtr<ossimXmlNode> pqeNode    = 0;
   ossimRefPtr<ossimXmlNode> tmpNode    = 0;
   ossimRefPtr<ossimXmlNode> tmpSubNode = 0;
   
   ossimString tag;
   ossimString attribute;
   ossimString value;
   ossimString text;
   ossim_uint32 idIndex = 1;
   ossimString imageName = getImageName(true);

   if (f.exists() && concatenateFlag)
   {
      if (doc.openFile(f) == false)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimQtPositionQualityDialog::saveToFile WARNING\n"
            << "Could not open:  " << f.c_str() << std::endl;
         return false;
      }

      rootNode = doc.getRoot();
      if (rootNode->getTag() != ROOT_TAG)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimQtPositionQualityDialog::saveToFile WARNING\n"
            << "Root tag from xml document not " << ROOT_TAG << std::endl;
         return false;
      }

      // To hold nodes found.
      std::vector<ossimRefPtr<ossimXmlNode> > xmlNodes;

      // For path:
      ossimString xpath;

      // For iterations.
      std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator i;
      
      // Get an array of nodes matching <image>.
      xpath = "/" + ROOT_TAG + "/" + IMAGE_TAG;
      doc.findNodes(xpath, xmlNodes);

      // Find the <image> node that matches our file name if one exists.
      i = xmlNodes.begin();
      attribute = "filename";
      while (i != xmlNodes.end())
      {
         (*i)->getAttributeValue(value, attribute);
         if (value == imageName)
         {
            imageNode = (*i);
            break;
         }
         ++i;
      }

      // Find the <pqeUpdateTime> update time tag.
      xpath = "/" + ROOT_TAG + "/" + UPDATE_TIME_TAG;
      xmlNodes.clear();
      doc.findNodes(xpath, xmlNodes);
      if (xmlNodes.size())
      {
         timeNode = xmlNodes[0].get();
      }

      // Find the last id index
      xpath = "/" + ROOT_TAG + "/" + IMAGE_TAG + "/" + PQE_POINT_TAG + "/"
         + GML_POINT_TAG;
      xmlNodes.clear();
      doc.findNodes(xpath, xmlNodes);
      idIndex = xmlNodes.size() + 1;
   }

   // The root node can be set or not set so check and make if needed.
   if ( !rootNode.get() )
   {
      rootNode = new ossimXmlNode();
      rootNode->setTag(ROOT_TAG);

      attribute = "xmlns";
      value = "urn:pqe";
      rootNode->addAttribute(attribute, value);

      attribute = "xmlns:igm";
      value = "http://www.opengis.net/igm/0.0";
      rootNode->addAttribute(attribute, value);
      
      attribute = "xmlns:gml";
      value = "http://www.opengis.net/gml";
      rootNode->addAttribute(attribute, value);

      attribute = "xmlns:xsi";
      value = "http://www.w3.org/2001/XMLSchema-instance";
      rootNode->addAttribute(attribute, value);

      attribute = "xsi:schemaLocation";
      value = "urn:pqe pqeReport.xsd";
      rootNode->addAttribute(attribute, value);
      
      attribute = "id";
      value = "Example PQE Report";
      rootNode->addAttribute(attribute, value);

      // Parent to document.
      doc.initRoot(rootNode);

      tmpNode = new ossimXmlNode();
      tag = "pqeOriginatingSystem";
      tmpNode->setTag(tag);
      text = "OSSIM ImageLinker PQE";
      tmpNode->setText(text);
      rootNode->addChildNode(tmpNode);
   }

   // The time node can be set or not set so check and make if needed.
   if ( !timeNode.get() )
   {
      timeNode = new ossimXmlNode();
      tag = "pqeUpdateTime";
      timeNode->setTag(tag);
      rootNode->addChildNode(timeNode);
   }

   // Set the time in the format of: yyyy-mm-ddThh:mm:ss
   char s[20];
   s[19] = '\0';
   time_t t;
   time(&t);
   tm* lt = localtime(&t);
   strftime(s, 20, "%FT%T", lt);
   text = s;
   timeNode->setText(text);
   
   // The image node can be set or not set so check and make if needed.
   if ( !imageNode.get() )
   {
      imageNode = new ossimXmlNode();
      imageNode->setTag(IMAGE_TAG);
      attribute = "filename";
      imageNode->addAttribute(attribute, imageName);

      // Parent to root.
      rootNode->addChildNode(imageNode);  
   }

   if (isRpcModel())
   {
      addRpcPoint(imageNode.get(), idIndex);
   }
   else
   {
      addCovPoint(imageNode.get(), idIndex);
   }

   return doc.write(f);
}

void ossimQtQualityEvaluatorTab::addRpcPoint(ossimXmlNode* parent,
                                             ossim_uint32 idIndex) const
{
   ossimRefPtr<ossimXmlNode> pqeNode  = 0;
   ossimRefPtr<ossimXmlNode> node     = 0;
   ossimRefPtr<ossimXmlNode> subNode  = 0;
   ossimRefPtr<ossimXmlNode> subNode2 = 0;
   ossimString value;

   // Node for this point.
   pqeNode = new ossimXmlNode();
   pqeNode->setTag(ossimString("pqePoint"));
   value = "Using geometrically-specified uncertainty input;";
   pqeNode->addAttribute(ossimString("remark"), value);
   parent->addChildNode(pqeNode);

   addImagePosition(pqeNode.get());

   addGroundPosition(pqeNode.get(), idIndex);

   node = new ossimXmlNode();
   node->setTag(ossimString("InputUncertainty"));
   node->addAttribute(ossimString("type"),
                      ossimString("losSurfaceIntersection"));
   pqeNode->addChildNode(node);

   subNode = new ossimXmlNode();
   subNode->setTag(ossimString("losBiasError"));
   subNode->addAttribute(ossimString("uom"), ossimString("m"));
   subNode->setText(ossimString::toString(theRpcPqeInputs.theRpcBiasError, 2));
   node->addChildNode(subNode);
   
   subNode = new ossimXmlNode();
   subNode->setTag(ossimString("losRandError"));
   subNode->addAttribute(ossimString("uom"), ossimString("m"));
   subNode->setText(ossimString::toString(theRpcPqeInputs.theRpcRandError, 2));
   node->addChildNode(subNode);

   subNode = new ossimXmlNode();
   subNode->setTag(ossimString("losElevationAngle"));
   subNode->addAttribute(ossimString("uom"), ossimString("degrees"));
   subNode->setText(ossimString::toString(theRpcPqeInputs.theRpcElevationAngle,
                                          2));
   node->addChildNode(subNode);

   subNode = new ossimXmlNode();
   subNode->addAttribute(ossimString("uom"), ossimString("degrees"));
   subNode->setTag(ossimString("losAzimuthAngle"));
   subNode->setText(ossimString::toString(theRpcPqeInputs.theRpcAzimuthAngle,
                                          2));
   node->addChildNode(subNode);

   subNode = new ossimXmlNode();
   subNode->setTag(ossimString("surfaceCharacter"));
   node->addChildNode(subNode);

   value = ossimString::toString(theRpcPqeInputs.theSurfaceNormalVector[0],5);
   value += " ";
   value += ossimString::toString(theRpcPqeInputs.theSurfaceNormalVector[1],5);
   value += " ";
   value += ossimString::toString(theRpcPqeInputs.theSurfaceNormalVector[2],5);
   subNode->addChildNode(ossimString("gml:vector"), value);

   addMatrix(subNode.get(), theRpcPqeInputs.theSurfaceCovMatrix);

   addEvaluation(pqeNode.get());
}

void ossimQtQualityEvaluatorTab::addCovPoint(ossimXmlNode* parent,
                                             ossim_uint32 idIndex) const
{
   ossimRefPtr<ossimXmlNode> pqeNode = 0;
   ossimRefPtr<ossimXmlNode> node    = 0;
   ossimRefPtr<ossimXmlNode> subNode = 0;
   ossimString value;

   // Node for this point.
   pqeNode = new ossimXmlNode();
   pqeNode->setTag(ossimString("pqePoint"));
   value = "Using covariance matrix uncertainty input;";
   pqeNode->addAttribute(ossimString("remark"), value);
   parent->addChildNode(pqeNode);
   
   addImagePosition(pqeNode.get());

   addGroundPosition(pqeNode.get(), idIndex);

   node = new ossimXmlNode();
   node->setTag(ossimString("InputUncertainty"));
   node->addAttribute(ossimString("type"),
                      ossimString("covarianceMatrix"));
   pqeNode->addChildNode(node);

   addMatrix(node.get(), theCovMat);

   addEvaluation(pqeNode.get());
}

void ossimQtQualityEvaluatorTab::addImagePosition(ossimXmlNode* parent) const
{
   // image point
   ossimRefPtr<ossimXmlNode> node = new ossimXmlNode();
   node->setTag(ossimString("imagePosition"));
   node->addChildNode(ossimString("samp"),
                      ossimString::toString(theImagePt.x, 1));
   node->addChildNode(ossimString("line"),
                      ossimString::toString(theImagePt.y, 1));
   parent->addChildNode(node);
}

void ossimQtQualityEvaluatorTab::addGroundPosition(
   ossimXmlNode* parent, ossim_uint32 idIndex) const
{
   ossimString value;
   
   ossimRefPtr<ossimXmlNode> node = new ossimXmlNode();
   node->setTag(ossimString("gml:Point"));
   node->addAttribute(ossimString("srsName"),
                      ossimString("urn:ogc:def:crs:EPSG::4979"));

   value = "p";
   value += ossimString::toString(idIndex);
   node->addAttribute(ossimString("gml:id"), value);
   parent->addChildNode(node);
   
   value = ossimString::toString(theGroundPt.latd());
   value += " ";
   value += ossimString::toString(theGroundPt.lond());
   value += " ";
   value += ossimString::toString(theGroundPt.height(), 2);
   node->addChildNode(ossimString("gml:pos"), value);
}

void ossimQtQualityEvaluatorTab::addEvaluation(ossimXmlNode* parent) const
{
   ossimRefPtr<ossimXmlNode> node     = 0;
   ossimRefPtr<ossimXmlNode> subNode  = 0;
   ossimRefPtr<ossimXmlNode> subNode2 = 0;
   ossimString value;
   
   node = new ossimXmlNode();
   node->setTag(ossimString("OriginatingEvaluation"));
   parent->addChildNode(node);
   
   subNode = new ossimXmlNode();
   subNode->setTag(ossimString("ErrorEllipse"));
   node->addChildNode(subNode);
   
   subNode2 = new ossimXmlNode();
   subNode2->setTag(ossimString("semiMajorAxis"));
   subNode2->addAttribute(ossimString("uom"), ossimString("m"));
   subNode2->setText(ossimString::toString(theSemiMajorAxis, 2));
   subNode->addChildNode(subNode2);
   
   subNode2 = new ossimXmlNode();
   subNode2->setTag(ossimString("semiMinorAxis"));
   subNode2->addAttribute(ossimString("uom"), ossimString("m"));
   subNode2->setText(ossimString::toString(theSemiMinorAxis, 2));
   subNode->addChildNode(subNode2);
   
   subNode2 = new ossimXmlNode();
   subNode2->setTag(ossimString("azimuth"));
   subNode2->addAttribute(ossimString("uom"), ossimString("degrees"));
   subNode2->setText(ossimString::toString(theAzimuth, 2));
   subNode->addChildNode(subNode2);
   
   subNode->addChildNode(ossimString("pLevel"),
                         ossimString::toString(getProbabilityLevel()));

   subNode = new ossimXmlNode();
   subNode->setTag(ossimString("CE"));
   subNode->addAttribute(ossimString("uom"), ossimString("m"));
   subNode->setText(ossimString::toString(theCe90, 2));
   node->addChildNode(subNode);

   subNode = new ossimXmlNode();
   subNode->setTag(ossimString("LE"));
   subNode->addAttribute(ossimString("uom"), ossimString("m"));
   subNode->setText(ossimString::toString(theLe90, 2));
   node->addChildNode(subNode);

   addMatrix(node.get(), theCovMat);
}

void ossimQtQualityEvaluatorTab::addMatrix(
   ossimXmlNode* parent, const NEWMAT::Matrix& matrix) const
{
   ossimRefPtr<ossimXmlNode> node = new ossimXmlNode();
   node->setTag(ossimString("igm:CompleteMatrix"));
   parent->addChildNode(node);

   node->addChildNode(ossimString("igm:valueUnit"), ossimString("m m m"));

   // Upper triangular covariance matrix.
   ossimString value;

   value += ossimString::toString(matrix[0][0], 6);
   value+= " ";
   value += ossimString::toString(matrix[0][1], 6);
   value+= " ";
   value += ossimString::toString(matrix[0][2], 6);
   value+= " ";
   value += ossimString::toString(matrix[1][1], 6);
   value+= " ";
   value += ossimString::toString(matrix[1][2], 6);
   value+= " ";
   value += ossimString::toString(matrix[2][2], 6);
   
   node->addChildNode(ossimString("igm:valuesList"), value);
}

ossim_int32 ossimQtQualityEvaluatorTab::getProbabilityLevel() const
{
   ossim_int32 result = 0;
   
  pqeProbLev_t pLev = static_cast<pqeProbLev_t>(
     theProbabilityLevelComboBox->currentItem());
  switch(pLev)
  {
     case ONE_SIGMA:
        result = 68;
        break;
     case P50:
        result = 50;
        break;
     case P90:
        result = 90;
        break;
     case P95:
        result = 95;
        break;
     default:
        result = 0;
  };
  return result;
}

ossimQtReportGeneratorTab::ossimQtReportGeneratorTab(QWidget *parent)
   : QWidget(parent),
     theController(0),
     thePqeTab(0),
     theReportFormatComboBox(0)
{
   QGroupBox*   hBoxRow1 = new QGroupBox();
   QHBoxLayout* layout1  = new QHBoxLayout;
   
   theReportFormatComboBox = new QComboBox();
   theReportFormatComboBox->insertItem(ANNOTATED_CHIP_OPTION);
   theReportFormatComboBox->insertItem(ORP_OPTION);

#ifdef HAS_PQE_INSERTS
#  include <insertion-code/pqeInsert1.h>
#endif
   
   layout1->addWidget(theReportFormatComboBox);

   QPushButton* saveButton = new QPushButton(tr("Save"));
   layout1->addWidget(saveButton);

   hBoxRow1->setLayout(layout1);

   connect(saveButton, SIGNAL(clicked()), this,
           SLOT(saveButtonClicked()));

   QVBoxLayout* mainLayout = new QVBoxLayout;
   mainLayout->addWidget(hBoxRow1);
   setLayout(mainLayout);
   
   setWindowTitle(tr("Report Generator Dialog"));
}

ossimQtReportGeneratorTab::~ossimQtReportGeneratorTab()
{
}

void ossimQtReportGeneratorTab::setController(
   ossimQtImageWindowController* controller)
{
   theController = controller;
}

void ossimQtReportGeneratorTab::setQualityEvaluatorTab(
   ossimQtQualityEvaluatorTab* pqeTab)
{
   thePqeTab = pqeTab;
}

void ossimQtReportGeneratorTab::saveButtonClicked()
{
   if (!theReportFormatComboBox || !thePqeTab)
   {
      return;
   }

   //---
   // Verify the user has dropped a point.  At construction of this dialog
   // "theAzimuth" is set to nan for this purpose.  If a point has been dropped
   // it should NOT be nan.
   //---
   if (ossim::isnan(thePqeTab->theAzimuth))
   {
      QString caption = "Notice:";
      QString text = "No point has been dropped yet.  Nothing to save!";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }

   QString qs = theReportFormatComboBox->currentText();
   if (qs == ANNOTATED_CHIP_OPTION)
   {
      if (theController)
      {
         // Turn off mouse capture.
         thePqeTab->thePqeEnabledCheckBox->setChecked(false);
         
         theController->igenExport();
      }
   }
   else if (qs == ORP_OPTION)
   {
      QFileDialog* fd = new QFileDialog( 0 );
      fd->setMode( QFileDialog::AnyFile );
      QString types ("Any files (*.xml *.XML)");
      fd->setFilter(types);
      QString fileName;
      if ( fd->exec() == QDialog::Accepted )
      {
         fileName = fd->selectedFile();
         bool concatenateFlag = false;
         if ( QFile::exists( fileName ) )
         {
            int answer = QMessageBox::question(
               this,
               tr("Concatenate File?"),
               tr("File exists: %1\n"
                  "Do you want to concatenate your point to it?\n"
                  "Choose Yes to concatenate or No to overwrite.")
               .arg( fileName ),
               QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);
            if (answer == QMessageBox::Yes)
            {
               concatenateFlag = true;
            }
         }

         ossimFilename f = fileName.ascii();
         if ( f != thePqeTab->getImageName(false))
         {
            if (thePqeTab->saveToFile(f, concatenateFlag) == true)
            {
               ossimNotify(ossimNotifyLevel_NOTICE)
                  << "Wrote file: " << f << endl;
            }
         }
         else
         {
            QString caption = "Cannot save warning:";
            QString text = "Report file and image file are the same!";
            QMessageBox::information( this,
                                      caption,
                                      text,
                                      QMessageBox::Ok );
         }
      }
      delete fd;
      fd = 0;
   }
#ifdef HAS_PQE_INSERTS
#  include <insertion-code/pqeInsert2.h>
#endif   
}
