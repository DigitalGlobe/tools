//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Raster Import Tool (IVIEW) application main.
//
// $Id: iview.cpp 18478 2010-12-01 15:29:51Z gpotts $
//----------------------------------------------------------------------------
#include <iostream>
#include <QtGui/QApplication>
#include <ossimQt/ossimQtSingleImageWindow.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/init/ossimInit.h>
#ifdef OSSIMQT_USE_WINDOWS_STYLE
#include <QWindowsStyle>
#endif


using namespace std;


#ifdef WIN32
int main(int argc, char *argv[]);
#include <windows.h>
#include <shellapi.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
	int argc = 1;
	char *argv[] = {"iview"};
    int ret = main( argc, argv );
    return ret;
}
#endif
int main(int argc, char *argv[])
{
   ossimArgumentParser argumentParser(&argc, argv);
   ossimInit::instance()->addOptions(argumentParser);
   ossimInit::instance()->initialize(argumentParser);
   argumentParser.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");
   argumentParser.getApplicationUsage()->setApplicationName(argumentParser.getApplicationName());
   argumentParser.getApplicationUsage()->setDescription(argumentParser.getApplicationName()+" GUI application for the ossim core library");
   argumentParser.getApplicationUsage()->setCommandLineUsage(argumentParser.getApplicationName()+" [options]");
  
   if (argumentParser.read("-h") || argumentParser.read("--help"))
   {
      argumentParser.getApplicationUsage()->write(std::cout);
      exit(0);
   }
   argumentParser.reportRemainingOptionsAsUnrecognized();

   //---
   // This was put here since static builds on windows don't load.
   // this entrance is a singleton so if called more than once its ok.
   // This load the images uses for icons and about windows.
   //---
  
   QApplication app(argc, argv);
#ifdef OSSIMQT_USE_WINDOWS_STYLE
   QWindowsStyle *style = new QWindowsStyle();
   app.setStyle(style);
#endif

   ossimQtSingleImageWindow* mainWindow
      = new ossimQtSingleImageWindow(NULL, "mainWindow");

   app.setMainWidget(mainWindow);
   
   mainWindow->show();

   int result = app.exec();

   ossimInit::instance()->finalize();
   

   return result;
}
