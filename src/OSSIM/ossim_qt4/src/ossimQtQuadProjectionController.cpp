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
// $Id: ossimQtQuadProjectionController.cpp 21853 2012-10-21 20:20:17Z dburken $

#include <ossimQt/ossimQtQuadProjectionController.h>
#include <ossim/base/ossimConstants.h>
#include <ossimQt/ossimQtQuadProjectionDialog.h>
#include <ossimQt/ossimQtImageInfoController.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossimQt/ossimQtDisplayUtility.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/projection/ossimBilinearProjection.h>

#include <QtCore/QEvent>
#include <QtCore/QStringList>
#include <QtGui//QAction>
#include <QtGui/QApplication>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtGui/QStatusBar>
#include <QtGui/QMessageBox>
#include <QtGui/QWidget>
#include <QtGui/QDialog>
#include <Qt3Support/Q3FileDialog>
#include <Qt3Support/Q3Table>
#include <Qt3Support/Q3Header>
#include <QtGui/QFontInfo>

#include <vector>
#include <sstream>
using namespace std;



static const ossimString LAT_FORMAT = "dd@ mm' ss.ssss\" C";
static const ossimString LON_FORMAT = "ddd@ mm' ss.ssss\" C";

static ossimTrace traceDebug("ossimQtQuadProjectionController:degug");

static const char* COLUMN_NAME[] = { "line",
                                     "sample",
                                     "latitude",
                                     "longitude",
                                     "height" };

ossimQtQuadProjectionController::ossimQtQuadProjectionController(ossimQtQuadProjectionDialog* dialog)
   :
      QObject(),
      theDialog(dialog),
      theWidget(NULL),
      theWidgetLayer(0),
      theRoamingFlag(false)
{
}

ossimQtQuadProjectionController::~ossimQtQuadProjectionController()
{
}

void ossimQtQuadProjectionController::setImageWidget(ossimQtScrollingImageWidget* widget, ossim_uint32 widget_layer)
{
   theWidget      = widget;
   theWidgetLayer = widget_layer;

   connect(theWidget,
           SIGNAL(scrollingImageWidgetMouseEvent(ossimQtMouseEvent*)),
           this,
           SLOT(trackScrollingImageWidget(ossimQtMouseEvent*)));

   if (theDialog)
   {
      initializeDialog();
   }
}

void ossimQtQuadProjectionController::initializeDialog() const
{
   if (!theDialog || !theWidget) return;
   
//   theCallBackDisabled = true;

   buildTable();
   buildDatumMenu();

   if ( updateDialogFromImageHandler() == false )
   {
      clearDialog();
   }

//   theCallBackDisabled = false;
}

void ossimQtQuadProjectionController::buildTable() const
{
   //---
   // NOTE:
   // setAlignment() method coming in QT version 4.0 according to header file.
   // For now we'll have to live with odd justification...
   //---

   // Default cell widths.
   int line_sample_width = 128;
   int lat_lon_width     = 196;
   int height_width      = 128;

   QString qs;
   QSize   qz;
   
   // Get the width for line and sample cells.
   qs = "999999.00";
   theDialog->theLineSampleGeographicTable->setText(0, 0, qs);
   qz = theDialog->theLineSampleGeographicTable->item(0,0)->sizeHint();
   theDialog->theLineSampleGeographicTable->clearCell(0, 0);
   line_sample_width = qz.width();

   // Get the width for latitude and longitude cells.
   qs = "-121.123456789012345";
   theDialog->theLineSampleGeographicTable->setText(0, 3, qs);
   qz = theDialog->theLineSampleGeographicTable->item(0,3)->sizeHint();
   theDialog->theLineSampleGeographicTable->clearCell(0, 3);
   lat_lon_width = qz.width();

   // Get the width for the height cell.
   qs = "99000.123";
   theDialog->theLineSampleGeographicTable->setText(0, 4, qs);
   qz = theDialog->theLineSampleGeographicTable->item(0,4)->sizeHint();
   theDialog->theLineSampleGeographicTable->clearCell(0, 4);
   height_width = qz.width();
   
   // Set the table names across the top.
   theDialog->theLineSampleGeographicTable->
      horizontalHeader()->setLabel(0, QString("line"));
   
   theDialog->theLineSampleGeographicTable->setColumnWidth(0,
                                                           line_sample_width);
   
   theDialog->theLineSampleGeographicTable->
      horizontalHeader()->setLabel(1, QString("sample"));

   theDialog->theLineSampleGeographicTable->setColumnWidth(1,
                                                           line_sample_width);
   
   theDialog->theLineSampleGeographicTable->
      horizontalHeader()->setLabel(2, QString("latitude"));
   
   theDialog->theLineSampleGeographicTable->setColumnWidth(2,
                                                           lat_lon_width);
   
   theDialog->theLineSampleGeographicTable->
      horizontalHeader()->setLabel(3, QString("longitude"));

   theDialog->theLineSampleGeographicTable->setColumnWidth(3,
                                                           lat_lon_width);
   
   theDialog->theLineSampleGeographicTable->
      horizontalHeader()->setLabel(4, QString("height"));
   
   theDialog->theLineSampleGeographicTable->setColumnWidth(4,
                                                           height_width);
}

void ossimQtQuadProjectionController::buildDatumMenu() const
{
   // Build the datum menu.
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

   // Set to WGE for default (WGS-84).
   const int COUNT = theDialog->theDatumComboBox->count();
   for (int i = 0; i < COUNT; ++i)
   {
      ossimString name = theDialog->theDatumComboBox->text(i).ascii();
      
      if (name.contains("WGE"))
      {
         theDialog->theDatumComboBox->setCurrentItem(i);
         break;
      }
   }   
}

bool ossimQtQuadProjectionController::updateDialogFromImageHandler() const
{
   ossimImageHandler* ih = getImageHandler();
   if (!ih)
      return false;
   
   ossimRefPtr<ossimImageGeometry> geom = ih->getImageGeometry();
   if (!geom)
      return false;

   ossimBilinearProjection* proj =  PTR_CAST(ossimBilinearProjection, geom->getProjection());
   if (!proj)
         return false;
   
   std::vector<ossimDpt>  lineSamplePt;
   std::vector<ossimGpt>  geographicPt;
   
   const ossim_uint32 numTps = lineSamplePt.size();
   if ( (numTps == 0) || (numTps != geographicPt.size()) )
      return false;

   if (theDialog->theLineSampleGeographicTable->numRows() != static_cast<int>(numTps))
   {
      int rows = static_cast<int>(numTps);
      theDialog->theLineSampleGeographicTable->setNumRows(rows);
   }

   setDatumMenu(geographicPt[0].datum()->code());
   for (ossim_uint32 i=0; i<numTps; ++i)
   {
      setTableRow(static_cast<int>(i), geographicPt[i], lineSamplePt[i]);
   }
   
   return true;
}

void ossimQtQuadProjectionController::setDatumMenu(const ossimKeywordlist& kwl) const
{
   ossimString code = "WGE"; // Default datum...
   
   const char* lookup = kwl.find(ossimKeywordNames::DATUM_KW);
   if (lookup)
   {
      code = lookup;
   }

   setDatumMenu(code);
}

void ossimQtQuadProjectionController::setDatumMenu(const ossimString& code) const
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

void ossimQtQuadProjectionController::setTableRow(int row,
                                                  const ossimGpt& gp,
                                                  const ossimDpt& dp) const
{
   setTableRow(row, dp);
   setTableRow(row, gp);
}

void ossimQtQuadProjectionController::setTableRow(int row,
                                                  const ossimGpt& gp) const
{
   QString qs;
   ossimString os;
   
   // Set the latitude.
   ossimDms lat_dms(gp.latd(), true);
   os = lat_dms.toString(LAT_FORMAT);
   qs = os.c_str();
   theDialog->theLineSampleGeographicTable->setText(row, 2, qs);
   
   // Set the longitude.
   ossimDms lon_dms(gp.lond(), false);
   os = lon_dms.toString(LON_FORMAT);
   qs = os.c_str();
   theDialog->theLineSampleGeographicTable->setText(row, 3, qs);

   // Set the height.
   os = ossimString::toString(gp.height(), 3);
   qs = os.c_str();
   theDialog->theLineSampleGeographicTable->setText(row, 4, qs);
}

void ossimQtQuadProjectionController::setTableRow(int row,
                                                  const ossimDpt& dp) const
{
   QString qs;
   ossimString os;
   
   // Set the line.
   os = ossimString::toString(dp.y, 2);
   qs = os.c_str();
   theDialog->theLineSampleGeographicTable->setText(row, 0, qs);

   // Set the sample.
   os = ossimString::toString(dp.x, 2);
   qs = os.c_str();
   theDialog->theLineSampleGeographicTable->setText(row, 1, qs);
}

const ossimDatum* ossimQtQuadProjectionController::getDatum() const
{
   const ossimString SEPARATOR = ":";
   ossimString code = theDialog->theDatumComboBox->currentText().ascii();
   code = code.before(SEPARATOR);
   const ossimDatum* datum = ossimDatumFactory::instance()->create(code);
   if (!datum)
   {
      return ossimDatumFactory::instance()->wgs84();
   }
   
   return datum;
}

ossimImageHandler* ossimQtQuadProjectionController::getImageHandler() const
{
   ossimImageHandler* handler = NULL;
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

ossimImageChain* ossimQtQuadProjectionController::getChain() const
{
   ossimImageChain* ic = NULL;
   ossimConnectableObject* obj = theWidget->getInput(theWidgetLayer);
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
   return ic;
}

ossimImageViewTransform* ossimQtQuadProjectionController::getImageViewTransform() const
{
   ossimImageViewTransform* trans = NULL;

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

ossimProjection* ossimQtQuadProjectionController::getImageProjection() const
{
   ossimProjection* proj = NULL;
   ossimImageViewTransform* trans = getImageViewTransform();
   if (trans)
   {
      ossimImageViewProjectionTransform* ivpt
         = PTR_CAST(ossimImageViewProjectionTransform, trans);
      if(ivpt)
      {
         ossimImageGeometry* image_geom = ivpt->getImageGeometry();
         if (image_geom)
            proj = image_geom->getProjection();
      }
   }
   
   return proj;
}

void ossimQtQuadProjectionController::save()
{
   const int ROWS = theDialog->theLineSampleGeographicTable->numRows();
   const int MINIMUM_POINTS = 4;

   if (ROWS < MINIMUM_POINTS)
   {
      QString caption("Sorry:");
      QString text = "At least for points (rows) must be set.\n";
      text += "Use the add button as needed.";
      QMessageBox::information( theDialog,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }

   // Get the datum.
   const ossimDatum* datum = getDatum();

   ossimKeywordlist kwl;

   // Add the projection type.
   kwl.add(ossimKeywordNames::TYPE_KW, "ossimBilinearProjection");
   
   for (int row = 0; row < ROWS; ++row)
   {
      ossimString line_string = theDialog->theLineSampleGeographicTable->
         text(row, LINE_COLUMN).ascii();
      if (line_string.empty())
      {
         sendEmptyColumnMessage(row, LINE_COLUMN);
         return;
      }
      ossimString sample_string = theDialog->theLineSampleGeographicTable->
         text(row, SAMPLE_COLUMN).ascii();
      if (sample_string.empty())
      {
         sendEmptyColumnMessage(row, SAMPLE_COLUMN);
         return;
      }
      ossimString latitude_string = theDialog->theLineSampleGeographicTable->
         text(row, LATITUDE_COLUMN).ascii();
      if (latitude_string.empty())
      {
         sendEmptyColumnMessage(row, LATITUDE_COLUMN);
         return;
      }
      ossimString longitude_string = theDialog->theLineSampleGeographicTable->
         text(row, LONGITUDE_COLUMN).ascii();
      if (longitude_string.empty())
      {
         sendEmptyColumnMessage(row, LONGITUDE_COLUMN);
         return;
      }
      // let height be empty.
      ossimString height_string = theDialog->theLineSampleGeographicTable->
         text(row, HEIGHT_COLUMN).ascii();

      // OK now add the data to the keyword list.

      // Add the line sample point.
      ossimString row_string = ossimString::toString(row);
      ossimString kw = "dpt";
      kw += row_string;
      ossimDpt dp;
      dp.y = line_string.toDouble();
      dp.x = sample_string.toDouble();
      ostringstream dos;
      dos << dp;
      kwl.add(kw, dos.str().c_str());

      // Add the geographic point.
      kw = "gpt";
      kw += row_string;
      ossimGpt gp;
      gp.latd( getDegrees(latitude_string, true) );
      gp.lond( getDegrees(longitude_string, false) );
      gp.height( height_string.toDouble() );
      gp.datum( datum );
      ostringstream gos;
      gos << gp;
      kwl.add(kw, gos.str().c_str());
      
   }  // End of "for (int row = 0; row < ROWS; ++row)"

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
                  ih->setImageGeometry(geom); // image handler assumes mem management
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

void ossimQtQuadProjectionController::sendEmptyColumnMessage(int row, ossimQtQuadProjectionController::Column col) const
{
   QString caption("Sorry:");
   QString text = "Required cell for row ";
   text += ossimString::toString(row+1).c_str();
   text += ", ";
   text += COLUMN_NAME[col];
   text += " column must be intitialized.\n";
   text += "Geometry file not saved.";
   QMessageBox::information( theDialog,
                             caption,
                             text,
                             QMessageBox::Ok );
}

void ossimQtQuadProjectionController::reset()
{
   if (!theDialog || !theWidget)
   {
      return;
   }
   if (updateDialogFromImageHandler() == false)
   {
      clearDialog();
   }
}

void ossimQtQuadProjectionController::clearDialog() const
{
   if (theDialog->theLineSampleGeographicTable->numRows() != 4)
   {
      theDialog->theLineSampleGeographicTable->setNumRows(4);
   }

   const int ROWS = theDialog->theLineSampleGeographicTable->numRows();
   const int COLS = theDialog->theLineSampleGeographicTable->numCols();
   
   for (int row = 0; row < ROWS; ++row)
   {
      for (int col = 0; col < COLS; ++col)
      {
         theDialog->theLineSampleGeographicTable->clearCell(row, col);
      }
   }
}

double ossimQtQuadProjectionController::getDegrees(const ossimString& s,
                                                   bool lat_flag) const
{
   ossimDms dms(0.0, lat_flag);
   dms.setDegrees(s.c_str());
   return dms.getDegrees();
}

void ossimQtQuadProjectionController::deleteRow() const
{
   if (!theDialog)
   {
      return;
   }

   int row = theDialog->theLineSampleGeographicTable->currentRow();
   ossimString row_string = ossimString::toString(row+1);
   
   QString caption("Question:");
   QString text = "Are you sure you want to delete row ";
   text += row_string.c_str();
   text += "?";
   int answer = QMessageBox::question( theDialog,
                                       caption,
                                       text,
                                       QMessageBox::Yes,
                                       QMessageBox::No);
   if (answer == QMessageBox::Yes)
   {
      theDialog->theLineSampleGeographicTable->removeRow(row);
   }
}

void ossimQtQuadProjectionController::addRow() const
{
   if (!theDialog)
   {
      return;
   }
   int newRow =  theDialog->theLineSampleGeographicTable->numRows();
   theDialog->theLineSampleGeographicTable->insertRows(newRow);
}

void ossimQtQuadProjectionController::cellValueChanged(int row, int col) const
{
   if (!theDialog)
   {
      return;
   }
   
   if ( (col == LATITUDE_COLUMN) || (col == LONGITUDE_COLUMN) )
   {
      ossimString os = theDialog->theLineSampleGeographicTable->
         text(row, col).ascii();
      bool lat_flag = (col == LATITUDE_COLUMN) ? true : false;
      double d = getDegrees(os, lat_flag);
      ossimDms dms(d, lat_flag); // true = lat flag...
      if (lat_flag)
      {
         os = dms.toString(LAT_FORMAT);
      }
      else
      {
         os = dms.toString(LON_FORMAT);
      }
      QString qs = os.c_str();
      theDialog->theLineSampleGeographicTable->setText(row, col, qs);
   }
}

void ossimQtQuadProjectionController::trackScrollingImageWidget(ossimQtMouseEvent* event)
{
   if (theRoamingFlag)
   {
      event->getEvent()->ignore();
      return;
   }
   
   // Activate on left press only...
   if ( ( event->getButton() == Qt::LeftButton) &&
        ( event->getQtEventType() == QEvent::MouseButtonPress ) )
   {
      // Get the mouse point.
      ossimDpt image_pt = event->getShiftPoint();
      
      ossimImageViewTransform* view = getImageViewTransform();
      if (view)
      {
         // Use the view to convert to image point.
         view->viewToImage(image_pt, image_pt);
      }
         
      // Get the selected row from the table.
      int row = theDialog->theLineSampleGeographicTable->currentRow();
      ossimString os = ossimString::toString(row+1);
      QString caption("Question:");
      QString text = "Adjust point for row ";
      text += os.c_str();
      text += "?";
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::Yes)
      {
         // Set the line and sample.
         setTableRow(row, image_pt);
      }

      ossimProjection* proj = getImageProjection();
      if (proj)
      {
         caption = "Question:";
         text = "Insert ground point from current view?\n";
         text += "Select no if you would like to tweak the current\n";
         text += "point in the table for row ";
         text += os.c_str();
         int answer = QMessageBox::question( theDialog,
                                             caption,
                                             text,
                                             QMessageBox::Yes,
                                             QMessageBox::No);
         if (answer == QMessageBox::Yes)
         {
            ossimProjection* proj = getImageProjection();
            if (proj)
            {
               ossimGpt gp;
               proj->lineSampleToWorld(image_pt, gp);
               setTableRow(row, gp);
            }
         }
      }
      
   } // End of mouse event check.
}

void ossimQtQuadProjectionController::enableRoaming(bool flag)
{
   theRoamingFlag = flag;
}

// Private to disallow copy constructor, assignment operator use...
ossimQtQuadProjectionController::ossimQtQuadProjectionController(const ossimQtQuadProjectionController&)
   :
      QObject(),
      theDialog(NULL),
      theWidget(NULL),
      theWidgetLayer(0),
      theRoamingFlag(false)
{
}

ossimQtQuadProjectionController& ossimQtQuadProjectionController::operator=(const ossimQtQuadProjectionController&)
{
   return *this;
}
