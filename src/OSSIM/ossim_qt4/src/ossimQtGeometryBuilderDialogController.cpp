#include <ossimQt/ossimQtGeometryBuilderDialogController.h>
#include <ossimQt/ossimQtGeometryBuilderDialog.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimRefPtr.h>
#include <QtGui/QTextEdit>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QTabWidget>
#include <QtGui/QComboBox>
#include <QtGui/QFileDialog>
#include <QtCore/QTextStream>

#include <sstream>

ossimQtGeometryBuilderDialogController::ossimQtGeometryBuilderDialogController(ossimQtGeometryBuilderDialog* dialog)
  :theDialog(dialog)
{
   generateProjections();
}

void ossimQtGeometryBuilderDialogController::convertFromWkt()
{
   if(theDialog)
   {
      ossimKeywordlist kwl;
      ossimString wktString(theDialog->theWktProjectionString->text().toStdString());
      wktString = wktString.trim();
//     wktString = wktString.substitute("\n","",true);
//     wktString = wktString.substitute("\r","",true);
      if( wktString!= "")
      {
// 	kwl.add(ossimKeywordNames::TYPE_KW,
// 		wktString,
// 		true);
         
         ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->createProjection(wktString);
         ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj.get());
         if(!mapProj)
         {
	    QMessageBox::warning(theDialog,
				 "Wkt To OSSIM Geom creation error", 
				 "Unable to create an OSSIM projection.\n"
				 "Please submit a bug report with the WKT\nstring being used and any additional parameters", 
				 QMessageBox::Ok,
				 QMessageBox::NoButton,  
				 QMessageBox::NoButton);
	    
         }
         else
         {
// 	    ossimUnitType unit = (ossimUnitType)ossimUnitTypeLut::instance()->getEntryNumber(theDialog->theGdalWktUnits->currentText().ascii());
// 	    ossimUnitConversionTool tool(1.0,
// 					 unit);
// 	    ossimString dx;
// 	    ossimString dy;
// 	    ossimString tieX = theDialog->theGdalWktTieX->text().ascii();
// 	    ossimString tieY = theDialog->theGdalWktTieY->text().ascii();
	    
// 	    tieX = tieX.trim();
// 	    tieY = tieY.trim();
// 	    std::istringstream in(theDialog->theGdalWktDeltaPerPixel->text().ascii());

// 	    in >> dx >> dy;

// 	    dx = dx.trim();
// 	    dy = dy.trim();
// 	    if(dy.empty())
// 	      {
// 		dy = dx;
// 	      }
// 	    if((tieX != "")&&
// 	       (tieY != "")&&
// 	       (dx != "")&&
// 	       (dy != ""))
// 	      {
// 		ossimDpt tiePt;
// 		ossimDpt deltaPt;
		
// 		if(mapProj->isGeographic())
// 		  {
// 		    tool.setValue(tieX.toDouble(), unit);
// 		    tiePt.x = tool.getValue(OSSIM_DEGREES);
// 		    tool.setValue(tieY.toDouble(), unit);
// 		    tiePt.y = tool.getValue(OSSIM_DEGREES);
// 		    tool.setValue(dx.toDouble(), unit);
// 		    deltaPt.x = tool.getValue(OSSIM_DEGREES);
// 		    tool.setValue(dy.toDouble(), unit);
// 		    deltaPt.y = tool.getValue(OSSIM_DEGREES);
// 		    mapProj->setUlGpt(ossimGpt(tiePt.lat,
// 					       tiePt.lon,
// 					       0.0,
// 					       mapProj->origin().datum()));
// 		    mapProj->setDecimalDegreesPerPixel(deltaPt);
// 		  }
// 		else
// 		  {
// 		    tool.setValue(tieX.toDouble(), unit);
// 		    tiePt.x = tool.getValue(OSSIM_METERS);
// 		    tool.setValue(tieY.toDouble(), unit);
// 		    tiePt.y = tool.getValue(OSSIM_METERS);
// 		    tool.setValue(dx.toDouble(), unit);
// 		    deltaPt.x = tool.getValue(OSSIM_METERS);
// 		    tool.setValue(dy.toDouble(), unit);
// 		    deltaPt.y = tool.getValue(OSSIM_METERS);
// 		    mapProj->setUlEastingNorthing(tiePt);
// 		    mapProj->setMetersPerPixel(deltaPt);
// 		  }
// 	      }
	    proj->saveState(kwl);
	    
	    theDialog->theOssimGeometryString->setText(kwl.toString().c_str());
	    theDialog->theTab->setCurrentPage(0);
         }
      }
      else
      {
         QMessageBox::warning(theDialog,
                              "Wkt Conversion Error", 
                              "Wkt string is empty", 
                              QMessageBox::Ok,
                              QMessageBox::NoButton,  
                              QMessageBox::NoButton);
         
      }
   }
}

void ossimQtGeometryBuilderDialogController::makeAdjustable()
{
}

void ossimQtGeometryBuilderDialogController::saveOssimGeometry()
{
   ossimString kwlString = theDialog->theOssimGeometryString->text().ascii();
   kwlString = kwlString.trim();
   bool overrideFlag = false;
   
   if(kwlString.empty())
   {
      return;
   }
   ossimKeywordlist geomKwl;
   
   std::istringstream in(kwlString);
   if(!geomKwl.parseStream(in))
   {
      return;
   }
   
   emit saveOssimGeometryOverride(overrideFlag, geomKwl);
   
   if(overrideFlag)
   {
      return;
   }
   QString filename = QFileDialog::getSaveFileName(theFilename.c_str());
   
   if(filename != "")
   {
      std::ofstream out(filename);
      if(!out.fail())
      {
         out << kwlString << std::endl;
         theFilename = filename.ascii();
      }
   }
}

void ossimQtGeometryBuilderDialogController::loadWktStringFromFile()
{
   QString filename = QFileDialog::getOpenFileName(theWktStringFilename.c_str());
   
   if(filename != "")
   {
      QFile file(filename);
      if(file.open(IO_ReadOnly) )
      {
         QTextStream stream(&file);
	 
         theDialog->theWktProjectionString->setText(stream.read());
      }
   }
}

void ossimQtGeometryBuilderDialogController::generateDefaultOssimGeometry(const char* className)
{
   ossimRefPtr<ossimProjection> proj;
   if(className)
   {
      proj = ossimProjectionFactoryRegistry::instance()->createProjection(ossimString(className));
   }
   else
   {
      proj = ossimProjectionFactoryRegistry::instance()->createProjection(ossimString("ossimEquDistCylProjection"));
   }
   
   ossimKeywordlist kwl;
   
   if(proj.valid())
   {
      proj->saveState(kwl);
      theDialog->theOssimGeometryString->setText(kwl.toString().c_str());  
   }
}

void ossimQtGeometryBuilderDialogController::generateProjections()
{
   std::vector<ossimString> typeList;
   ossimMapProjectionFactory::instance()->getTypeNameList(typeList);
   ossimEpsgProjectionFactory::instance()->getTypeNameList(typeList);
   
   for(ossim_uint32 idx = 0; idx < typeList.size(); ++idx)
   {
      theDialog->theOssimProjectionList->insertItem(typeList[idx].c_str());
   }
}
