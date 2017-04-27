#include <ossimPlanetQt/ossimPlanetQtApplication.h>
#include <ossimPlanetQt/ossimPlanetQtMainWindow.h>
#include <ossimPlanet/ossimPlanet.h>
#include <ossim/init/ossimInit.h>
#include <ossimPlanet/ossimPlanet.h>
#include <ossimPlanet/ossimPlanetLand.h>
#include <ossimPlanet/ossimPlanetTextureLayerRegistry.h>
// #include <osg/ArgumentParser>
// #include <osg/ApplicationUsage>
#include <osgDB/FileUtils>
// #include <wms/wms.h>
// #include <ossim/base/ossimArgumentParser.h>
#include <osg/Texture>
#include <QtGui/QSplashScreen>
#include <QtGui/QPixmap>

#ifdef WIN32
int main(int argc, char *argv[]);
#include <windows.h>
#include <shellapi.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
	int argc = 1;
	char *argv[] = {"ossimplanet"};
    int ret = main( argc, argv );
    return ret;
}
#endif
int main(int argc, char *argv[])
{
   ossimPlanetQtApplication app(argc, argv);
   Q_INIT_RESOURCE(ossimPlanetQt);
   QSplashScreen splash(QPixmap(":/ossimPlanetSplash"));
   splash.setWindowFlags(splash.windowFlags()|Qt::WindowStaysOnTopHint);
   splash.show();
//    app.processEvents();
//     splash.showMessage("Initializing planet core");
//    app.processEvents();
   ossimPlanetQtApplication::initWithArgs(argc, argv);
//    splash.showMessage("Starting Visualization system");
//    app.processEvents();
   ossimPlanetQtMainWindow*  mainWindow = new ossimPlanetQtMainWindow();
   mainWindow->setAttribute(Qt::WA_DeleteOnClose);
   mainWindow->show();
   ossimPlanetQtApplication::initInputDevices();
//    splash.showMessage("Finished initialization");
   splash.finish(mainWindow);
   int result = app.exec();
   return result;
}
