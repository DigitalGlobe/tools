#include <ossimPlanetQt/ossimPlanetQtApplication.h>

#include <QtCore/QDir>
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossim/init/ossimInit.h>
#include <iostream>
#include <osgDB/Registry>
#include <osg/ArgumentParser>
#include <osg/Drawable>
#include <osg/Texture>
#include <osg/ApplicationUsage>
#include <ossim/base/ossimArgumentParser.h>
#include <wms/wms.h>
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimGeoidEgm96.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossimPlanet/ossimPlanetInteractionController.h>
#include <ossimPlanet/ossimPlanetAction.h>
#include <ossimPlanet/ossimPlanetActionRouter.h>

QSettings*                       ossimPlanetQtApplication::theSettings = 0;
QString                          ossimPlanetQtApplication::theUserSupportDirectory;
QString                          ossimPlanetQtApplication::theUserDirectory;
ossimPlanetQtApplication::ReferenceImageListType ossimPlanetQtApplication::theReferenceImages;
std::vector<ossimFilename>       ossimPlanetQtApplication::thePlugins;
// bool                             ossimPlanetQtApplication::theElevEnabled  = true;
// ossim_float32                    ossimPlanetQtApplication::theElevExag     = 1.0;
// ossim_float32                    ossimPlanetQtApplication::theSplitMetricRatio = 3.0;
// ossim_uint32                     ossimPlanetQtApplication::theElevEstimate = 16;
// ossimFilename                    ossimPlanetQtApplication::theElevCache    = "";
// ossimPlanetLandType              ossimPlanetQtApplication::theLandType     = ossimPlanetLandType_NORMALIZED_ELLIPSOID;
// ossim_uint32                     ossimPlanetQtApplication::theLevelOfDetail= 20;
// bool                             ossimPlanetQtApplication::theHudEnabled   = true;
// bool                             ossimPlanetQtApplication::theMipMapping   = true;
ossimFilename                    ossimPlanetQtApplication::theThemePath      = "";
ossimFilename                    ossimPlanetQtApplication::theCompassRing = ""; 
//ossimFilename                    ossimPlanetQtApplication::theCompassRose = ""; 
//#define OSSIMPLANET_3D_CONNEXION
// NOTE THIS IS ONLY TEST CODE for ossimPlanet roadmap docs to enhance future versions
#ifdef OSSIMPLANET_3D_CONNEXION
#include "3DconnexionClient/ConnexionClientAPI.h"
#ifdef __MACOSX__
// extern OSErr InstallConnexionHandlers() __attribute__((weak_import));

static void tdx_drv_handler(io_connect_t connection, 
                            natural_t messageType, 
                            void *messageArgument);
typedef struct
{
	UInt16 				clientID; /* ID assigned by the driver */
	Boolean				showClientEventsOnly;
} TdxDeviceWrapperInfo, *TdxDeviceWrapperInfoPtr;

TdxDeviceWrapperInfo gDevWrapperInfo;


/* ========================================================================== */
// #pragma mark -
// #pragma mark * Façade of 3DconnexionClient framework APIs *

/* -------------------------------------------------------------------------- */
long ossimPlanetQtApplication::initInputDevices()
   // (UInt32 appID, 
//               Boolean showOnlyMyClientEvents, 
//               UInt16 mode, 
//               UInt32 mask)
{
   OSStatus err;
   gDevWrapperInfo.clientID = 0;
   gDevWrapperInfo.showClientEventsOnly = true;
   
//    gDevWrapperInfo.mainEventQueue = GetMainEventQueue();
   
   /* make sure the framework is installed */
   if (InstallConnexionHandlers == NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "3Dconnexion framework not found!\n" << std::endl;
//       fprintf(stderr, "3Dconnexion framework not found!\n");
      return -1;
   }
   
   /* install 3dx message handler in order to receive driver events */
   err = InstallConnexionHandlers(tdx_drv_handler, 0L, 0L);
//    assert(err == 0);
//    if (err)
//       return err;
   
   /* register our app with the driver */
//    gDevWrapperInfo.clientID = RegisterConnexionClient('osPt', (UInt8*)"ossimplanet", kConnexionClientModeTakeOver, kConnexionMaskAll);
   gDevWrapperInfo.clientID = RegisterConnexionClient('osPt', 0, kConnexionClientModeTakeOver, kConnexionMaskAll);
//    if (gDevWrapperInfo.clientID == 0)
//       return -2;
//    std::cout << "CLIENT ID = " <<    gDevWrapperInfo.clientID << std::endl;
//    fprintf(stderr, "3Dconnexion device initialized. Client ID: %d\n", 
   //gDevWrapperInfo.clientID);

   ossimPlanetAction(":iac tie axis0 LON").execute();
   ossimPlanetAction(":iac tie axis1 LAT").execute();
   ossimPlanetAction(":iac tie axis2 ZOOM").execute();
   ossimPlanetAction(":iac tie axis3 PITCH").execute();
//    ossimPlanetAction(":iac tie axis4 ROLL").execute();
   ossimPlanetAction(":iac tie axis5 YAW").execute();
   
   return err;
}

void ossimPlanetQtApplication::terminateInputDevices()
{
   UInt16 wasConnexionOpen = gDevWrapperInfo.clientID;

   if (InstallConnexionHandlers == NULL)
   {
      return;
   }
    
   /* make sure the framework is installed */
//	if (InstallConnexionHandlers == NULL)
//        return;
   
   /* close the connection with the 3dx driver */
   if (wasConnexionOpen)
   {
      UnregisterConnexionClient(gDevWrapperInfo.clientID);
   }
   
   CleanupConnexionHandlers();
   
   ///fprintf(stderr, "Terminated connection with 3Dconnexion device.\n");
}

// bool ossimPlanetQtApplication::macEventFilter ( EventHandlerCallRef caller, EventRef event )
// {
//    long kind = GetEventKind(event);
//    if(gDevWrapperInfo.mainEventQueue)
//    {
//       std::cout << GetNumEventsInQueue(gDevWrapperInfo.mainEventQueue) << std::endl;
//    }
//    return QApplication::macEventFilter(caller, event);
// }
inline double dampen(double value)
{
//    return value;
#if 1
   if(value > 1.0 || value < -1.0) return value;

   if(value < 0.0) return -(value*value);

   return value*value;
#endif
}
static void tdx_drv_handler(io_connect_t connection, 
                            natural_t messageType, 
                            void *messageArgument)
{
   static double normalizer = 500.0;
   static int minAxisValue = 1;
   static ConnexionDeviceState	lastState;
   ConnexionDeviceStatePtr state = (ConnexionDeviceStatePtr)messageArgument;
   ossimPlanetInteractionController* iac = ossimPlanetInteractionController::instance();
   
   switch(messageType)
   {
      case kConnexionMsgDeviceState:
//          std::cout << "client = " << state->client << std::endl;
//          std::cout << "myclient = " << gDevWrapperInfo.clientID << std::endl;
         /* Device state messages are broadcast to all clients.  It is up to
          * the client to figure out if the message is meant for them. This
          * is done by comparing the "client" id sent in the message to our
          * assigned id when the connection to the driver was established.
          * 
          * There is a special mode wherein all events are sent to this 
          * client regardless if it was meant for it or not.  This mode is 
          * determined by the showClientEventOnly flag.
          */
         if (!gDevWrapperInfo.showClientEventsOnly 
             || state->client == gDevWrapperInfo.clientID)
         {
            switch (state->command)
            {
               case kConnexionCmdHandleAxis:
               {
                  if((std::abs(state->axis[0]) <= minAxisValue)&&
                     (std::abs(state->axis[1]) <= minAxisValue))
                  {
                     if((std::abs(lastState.axis[0]) > minAxisValue)||
                        (std::abs(lastState.axis[1]) > minAxisValue))
                     {
                        iac->updateInteractionValuators("axis0", 0.0);
                        iac->updateInteractionValuators("axis1", 0.0);
                        ossimPlanetAction(":navigator rotatestop").execute();
                     }
                  }
                  else 
                  {
                     if((std::abs(lastState.axis[0]) <= minAxisValue)&&
                        (std::abs(lastState.axis[1]) <= minAxisValue))
                     {
                        iac->updateInteractionValuators("axis0", 0.0);
                        iac->updateInteractionValuators("axis1", 0.0);
                        ossimPlanetAction(":navigator rotatestart").execute();
                     }
                  }


                  // now do axis z
                  if(std::abs(state->axis[2]) <= minAxisValue)
                  {
                     if(std::abs(lastState.axis[2])> minAxisValue)
                     {
                        iac->updateInteractionValuators("axis2", 0.0);
                        ossimPlanetAction(":navigator loszoomstop").execute();
                     }
                  }
                  else
                  {
                     if(std::abs(lastState.axis[2]) <= minAxisValue)
                     {
                        iac->updateInteractionValuators("axis2", 0.0);
                        
                        ossimPlanetAction(":navigator loszoomstart").execute();
                     }                     
                  }

                  // now handle yaw and pitch that are associate to losrotatestart
                  if((std::abs(state->axis[3]) <= minAxisValue)&&
                     (std::abs(state->axis[5]) <= minAxisValue))
                  {
                     if((std::abs(lastState.axis[3]) > minAxisValue)||
                        (std::abs(lastState.axis[5]) > minAxisValue))
                     {
                         iac->updateInteractionValuators("axis3", 0.0);
                         iac->updateInteractionValuators("axis5", 0.0);
                         ossimPlanetAction(":navigator losrotatestop").execute();
                     }
                  }
                  else 
                  {
                     if((std::abs(lastState.axis[3]) < minAxisValue)&&
                        (std::abs(lastState.axis[5]) < minAxisValue))
                     {
                         iac->updateInteractionValuators("axis3", 0.0);
                         iac->updateInteractionValuators("axis5", 0.0);
                        ossimPlanetAction(":navigator losrotatestart").execute();
                     }
                  }

                  iac->updateInteractionValuators("axis0", dampen(state->axis[0]/normalizer));
                  iac->updateInteractionValuators("axis1", dampen(state->axis[1]/normalizer));
                  iac->updateInteractionValuators("axis2", dampen(state->axis[2]/normalizer));
                  iac->updateInteractionValuators("axis3", dampen(state->axis[3]/normalizer));
                  iac->updateInteractionValuators("axis5", dampen(state->axis[5]/normalizer));
                  break;
               }
               
               case kConnexionCmdHandleButtons:
               {
                  
                  break;
               }
               
               default:
                  break;
                  
            } /* switch */
         }
         BlockMoveData(state, &lastState, (long)sizeof(ConnexionDeviceState));
         
         break;
         
      default:
         /* other messageTypes can happen and should be ignored */
         break;
   }
   /*
     printf("connection: %X\n", connection);
     printf("messageType: %X\n", messageType);
     printf("version: %d\n", msg->version);
     printf("front app client: %d  ourID: %d\n", 
     msg->client, gDevWrapperInfo.clientID);
     printf("command: %u\n", msg->command);
     printf("value: %ld\n", msg->value);
     printf("param: %hd\n", msg->param);
     for (int i=0; i<8; i++)
     printf("report[%d]: %d\n", i, msg->report[i]);
     printf("buttons: %d\n", msg->buttons);
     printf("TX: %d\n", msg->axis[0]);
     printf("TY: %d\n", msg->axis[1]);
     printf("TZ: %d\n", msg->axis[2]);
     printf("RX: %d\n", msg->axis[3]);
     printf("RY: %d\n", msg->axis[4]);
     printf("RZ: %d\n", msg->axis[5]);
    printf("-----------------------------------------\n\n");
   */
//      printf("TX: %d\n", state->axis[0]);
//      printf("TY: %d\n", state->axis[1]);
//      printf("TZ: %d\n", state->axis[2]);
//      printf("RX: %d\n", state->axis[3]);
//      printf("RY: %d\n", state->axis[4]);
//      printf("RZ: %d\n", state->axis[5]);
}
#else
long ossimPlanetQtApplication::initInputDevices()
{
   return 0;
}

void ossimPlanetQtApplication::terminateInputDevices()
{
   return;
}
#endif // __MACOSX__

#else
long ossimPlanetQtApplication::initInputDevices()
{
   return 0;
}

void ossimPlanetQtApplication::terminateInputDevices()
{
   return;
}
#endif //OSSIMPLANET_3D_CONNEXION
#if  !defined(_WIN32) && !defined(__WIN32__) && !defined(_MSC_VER)
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/errno.h>
int setMaximumFiles(int filecount)
{
   struct rlimit lim;
   lim.rlim_cur = lim.rlim_max = (rlim_t)filecount;
   if ( setrlimit(RLIMIT_NOFILE, &lim) == 0 )
   {
      return 0;
   }
   else
   {
      return errno;
   }
}
int getMaximumFiles()
{
    struct rlimit lim;
    if ( getrlimit(RLIMIT_NOFILE, &lim) == 0 )
    {
        return (long)lim.rlim_max;
    }
    return 0;
}
#else
int setMaximumFiles(int /*filecount*/)
{
   return 0;
}
int getMaximumFiles()
{
   return 0;
}
#endif

/*!
  \class ossimPlanetQtApplication
  \brief The ossimPlanetQtApplication class manages application-wide information.

  This is a subclass of QApplication and should be instantiated in place of
  QApplication. Most methods are static in keeping witn the design of QApplication.

  This class hides platform-specific path information and provides
  a portable way of referencing specific files and directories.
  Ideally, hard-coded paths should appear only here and not in other modules
  so that platform-conditional code is minimized and paths are easier
  to change due to centralization.
*/
ossimPlanetQtApplication::ossimPlanetQtApplication(int & argc, char ** argv, bool guiEnabled)
: QApplication(argc, argv, guiEnabled)
{
   osg::Referenced::setThreadSafeReferenceCounting(true);
   setOrganizationName("OSSIM");
   setOrganizationDomain("planet.ossim.org");
   setApplicationName("OSSIM Planet");
   theSettings = new QSettings("planet.ossim.org");
   theUserSupportDirectory = ossimEnvironmentUtility::instance()->getUserOssimSupportDir();   
   theUserDirectory = ossimEnvironmentUtility::instance()->getUserDir();   
}

ossimPlanetQtApplication::~ossimPlanetQtApplication()
{
   if(theSettings)
   {
      delete theSettings;
      theSettings = 0;
   }
   ossimPlanetActionRouter::instance()->shutdown();
}

bool ossimPlanetQtApplication::initWithArgs(int& argc, char** argv)
{
   //osg::Texture::setMinimumNumberOfTextureObjectsToRetainInCache(0);
   osg::Drawable::setMinimumNumberOfDisplayListsToRetainInCache(0);
   //osg::DisplaySettings::instance()->setMaxNumberOfGraphicsContexts(1);

   if(getMaximumFiles() < 1024)
   {
            setMaximumFiles(1024);
   }
   //    setMaximumFiles(25);

   //osgDB::DatabasePager* databasePager = osgDB::Registry::instance()->getOrCreateDatabasePager();
   
   ossimArgumentParser argumentParser(&argc, argv);
   
   wmsInitialize();
   ossimInit::instance()->setPluginLoaderEnabledFlag(false);
   ossimInit::instance()->initialize(argumentParser);
   osg::ArgumentParser arguments(&argumentParser.argc(),argumentParser.argv());
   
   std::string tempString;
   osg::ArgumentParser::Parameter stringParam(tempString);
   addCommandLineOptions(arguments);
   
   if(arguments.read("-h") || arguments.read("--help"))
   {
      arguments.getApplicationUsage()->write(std::cout);
      return false;
   }
   
//    if(arguments.read("--enable-flatland"))
//    {
//       theLandType = ossimPlanetLandType_FLAT;
//    }
   if(arguments.read("--wms-timeout", stringParam))
   {
      setWmsNetworkTimeoutInSeconds(ossimString(tempString).toDouble());
   }
   if(arguments.read("--disable-elevation"))
   {
//       theElevEnabled = false;
      writePreferenceSetting("elev-flag",
                             "false");
   }
   if(arguments.read("--elev-estimation", stringParam))
   {
//       theElevEstimate = 1<<ossimString(tempString.c_str()).toUInt32();
      writePreferenceSetting("elev-patchsize",
                             ossimString::toString(1<<ossimString(tempString.c_str()).toUInt32()).c_str());
  }
   if(arguments.read("--elev-patchsize", stringParam))
   {
      writePreferenceSetting("elev-patchsize", tempString.c_str());
//       theElevEstimate = ossimString(tempString.c_str()).toUInt32();
   }
   if(arguments.read("--elev-exag", stringParam))
   {
//       theElevExag = ossimString(tempString.c_str()).toDouble();
      writePreferenceSetting("elev-exag", tempString.c_str());
   }
   if(arguments.read("--split-metric", stringParam))
   {
//       theSplitMetricRatio = ossimString(tempString.c_str()).toDouble();
      writePreferenceSetting("split-metric", tempString.c_str());
   }
   if(arguments.read("--elev-cache", stringParam))
   {
//       theElevCache = tempString.c_str();
      writePreferenceSetting("elev-cache", tempString.c_str());
   }
   if(arguments.read("--level-detail", stringParam))
   {
//       theLevelOfDetail = ossimString(tempString.c_str()).toUInt32();
      writePreferenceSetting("level-detail", tempString.c_str());
   }
   if(arguments.read("--enable-hud"))
   {
      writePreferenceSetting("hud-flag", "true");
//       theHudEnabled = true;
   }
   else if(arguments.read("--disable-hud"))
   {
      writePreferenceSetting("hud-flag", "false");
   }

   // archive mapping enabled
   if( arguments.read("--enable-archive-mapping-enabled") )
   {
	   writePreferenceSetting("archive-mapping-enabled", "true");
   }
   else if( arguments.read("--disable-archive-mapping-enabled") )
   {
	   writePreferenceSetting("archive-mapping-enabled", "false");
   }

   if(arguments.read("--disable-mipmap"))
   {
      writePreferenceSetting("mipmap-flag", "false");
// 	   theMipMapping = false;
   }
   if(arguments.read("--enable-mipmap"))
   {
      writePreferenceSetting("mipmap-flag", "true");
// 	   theMipMapping = false;
   }
   arguments.reportRemainingOptionsAsUnrecognized();
   
   if (arguments.errors())
   {
      arguments.writeErrorMessages(std::cout);
   }
   ossimFilename currentPath = ossimFilename(argv[0]).path();
   ossimFilename imageBundle;
   ossimFilename referenceImageBundle;
   ossimInit::instance()->setPluginLoaderEnabledFlag(true);
//    ossimString paths = ossimEnvironmentUtility::instance()->getEnvironmentVariable("OSSIM_ELEVATION_PATH");
//    std::vector<ossimString> pathArray;
//    ossimString pathSeparator = ":";
   osgDB::Registry::instance()->getDataFilePathList().push_back(theUserSupportDirectory.toStdString());
   ossimFilename installDir = ossimEnvironmentUtility::instance()->getInstalledOssimSupportDir();
   if(installDir.exists())
   {
      osgDB::Registry::instance()->getDataFilePathList().push_back(installDir);
   }
   ossimFilename userDir    = ossimEnvironmentUtility::instance()->getUserOssimSupportDir();
   ossimFilename userImageDir = userDir.dirCat("images");
   ossimFilename userDataDir = userDir.dirCat("data");
   ossimFilename userImageReferenceDir = userImageDir.dirCat("reference");
   ossimFilename installImageDir = installDir.dirCat("images");
   ossimFilename installDataDir = installDir.dirCat("data");
   ossimFilename instalImageReferenceDir = installImageDir.dirCat("reference");
   userDir = userDir.dirCat("images");
   userDir = userDir.dirCat("reference");
   installDir = installDir.dirCat("images");
   installDir = installDir.dirCat("reference");

   // tmp drb
   // ossimFilename userStatePlaneFile = userDataDir.dirCat("state_plane.csv");
   // ossimFilename installStatePlaneFile = installDataDir.dirCat("state_plane.csv");
   // if(userStatePlaneFile.exists())
   // {
      // allow state plane to override from user directory any installed state plane file.
   //   ossimStatePlaneProjectionFactory::instance()->addCsvFile(userStatePlaneFile);
   // }
#ifdef __MACOSX__
   // test for a bundle
   //
   ossimFilename contentsFolder = currentPath.path();

   // Check for embedded geoid grid nder the resource folder Resources/egm96.grd
   ossimFilename resourceFolder = contentsFolder.dirCat("Resources");
   ossimFilename geoid1996File = resourceFolder.dirCat("egm96.grd");
   ossimFilename statePlaneFile = resourceFolder.dirCat("state_plane.csv");
   if(geoid1996File.exists())
   {
      ossimRefPtr<ossimGeoid> geoid96 = new ossimGeoidEgm96;
      if(geoid96->open(geoid1996File, OSSIM_BIG_ENDIAN))
      {
         ossimGeoidManager::instance()->addGeoid(geoid96.get());
      }
   }
//   if(statePlaneFile.exists())
//   {
//      ossimStatePlaneProjectionFactory::instance()->addCsvFile(statePlaneFile);
//   }
   // Check for embedded plugins and reference images
   ossimFilename resources = contentsFolder.dirCat("Resources");
   ossimFilename ossimPluginsBundle = contentsFolder.dirCat("plugins");
   ossimFilename osgPlugins         = contentsFolder.dirCat("osgplugins");
   referenceImageBundle = contentsFolder.dirCat("Resources");
   imageBundle = referenceImageBundle.dirCat("images");
   referenceImageBundle = imageBundle.dirCat("reference");
   if(ossimPluginsBundle.exists())
   {
      ossimInit::instance()->loadPlugins(ossimPluginsBundle);
   }
   if(osgPlugins.exists())
   {
      osgDB::Registry::instance()->getLibraryFilePathList().push_front(osgPlugins);
   }
   if(resources.exists())
   {
      osgDB::Registry::instance()->getDataFilePathList().push_front(resources);
      osgDB::Registry::instance()->getDataFilePathList().push_front(resources.dirCat("fonts"));
   }

   theThemePath = resourceFolder;
   theThemePath = theThemePath.dirCat("themes");

   ossimFilename elevation = resourceFolder.dirCat("elevation");
   if(elevation.exists())
   {
      ossimElevManager::instance()->loadElevationPath(elevation);
   }
//    addLibraryPath(contentsFolder.dirCat("qtplugins").c_str());
#endif
#ifdef WIN32
   ossimFilename geoid = currentPath.dirCat("geoid1996");
   ossimFilename geoid1996File = geoid.dirCat("egm96.grd");
   if(!geoid1996File.exists())
   {
	   geoid = currentPath.dirCat("geoids");
	   geoid = geoid.dirCat("geoid1996");
	   geoid1996File = geoid.dirCat("egm96.grd");
   }
   if(geoid1996File.exists())
   {
      ossimRefPtr<ossimGeoid> geoid96 = new ossimGeoidEgm96;
      if(geoid96->open(geoid1996File, OSSIM_BIG_ENDIAN))
      {
         ossimGeoidManager::instance()->addGeoid(geoid96.get());
      }
   }
   ossimFilename osgPluginsBundle = currentPath.dirCat("osgplugins");
// ossimFilename ossimPluginsBundle = currentPath.dirCat("plugins");
   referenceImageBundle = currentPath;
   imageBundle = referenceImageBundle.dirCat("images");
   referenceImageBundle = imageBundle.dirCat("reference");
   if(osgPluginsBundle.exists())
   {
	   osgDB::Registry::instance()->getLibraryFilePathList().push_back(osgPluginsBundle);
   }
//   if(ossimPluginsBundle.exists())
//   {
//      ossimInit::instance()->loadPlugins(ossimPluginsBundle);
//   }
   ossimFilename elevation = currentPath.dirCat("elevation");
   if(elevation.exists())
   {
      ossimElevManager::instance()->loadElevationPath(elevation);
   }
//    pathSeparator = ";";
#endif
   ossimFilename dataDir = currentPath.dirCat("data");

   // tmp drb
   // ossimFilename statePlane = dataDir.dirCat("state_plane.csv");

   // if(statePlane.exists())
   // {
   // ossimStatePlaneProjectionFactory::instance()->addCsvFile(statePlane);
   //}
  // we will now initialize any other plugins outside the bundle in standard locations
   //
   // if(installStatePlaneFile.exists())
   // {
   // ossimStatePlaneProjectionFactory::instance()->addCsvFile(installStatePlaneFile);
   // }
   ossimInit::instance()->initializePlugins();
   
   if(arguments.argc() > 1)
   {
      ossimKeywordlist kwl;
      if(kwl.addFile(arguments.argv()[1]))
      {
         osg::ref_ptr<ossimPlanetTextureLayer> layer = ossimPlanetTextureLayerRegistry::instance()->createLayer(kwl.toString());
         if(layer.valid())
         {
            layer->resetLookAt();
            theReferenceImages.push_front(layer);
         }
      }
   }
   else
   {
      ossimFilename refFiles;
      
      if(userDir.exists())
      {
         refFiles = userDir;
      }
      else if(installDir.exists())
      {
         refFiles = installDir;
      }
      else if(referenceImageBundle.exists())
      {
         refFiles = referenceImageBundle;
      }
      if(refFiles.exists())
      {
         ossimDirectory dir;
         if(dir.open(refFiles))
         {
            ossimFilename file;
            dir.getFirst(file);
            do
            {
               ossimString ext = file.ext();
               if((ext != "ovr")&&
                  (ext != "omd")&&
                  (ext != "geom")&&
                  (ext != "his"))
               {
                  osg::ref_ptr<ossimPlanetTextureLayer> layer = ossimPlanetTextureLayerRegistry::instance()->createLayer(ossimString(file.c_str()));
                  if(layer.valid())
                  {
                     layer->resetLookAt();
                     theReferenceImages.push_front(layer);
                  }
               }
               
            }while(dir.getNext(file));
         }
      }      
   }
   if(userImageDir.dirCat("compass.png").exists())
   {
      theCompassRing = userImageDir.dirCat("compass.png");
   }
   else if(imageBundle.dirCat("compass.png").exists())
   {
      theCompassRing = imageBundle.dirCat("compass.png");
   }
   else if(installImageDir.dirCat("compassring.png").exists())
   {
      theCompassRing = installImageDir.dirCat("compassring.png");
      //theCompassRose = installImageDir.dirCat("compassrose.png");
   }
   else if(userImageDir.dirCat("compassring.png").exists())
   {
      theCompassRing = userImageDir.dirCat("compassring.png");
      //theCompassRose = userImageDir.dirCat("compassrose.png");
   }
   else if(imageBundle.dirCat("compassring.png").exists())
   {
      theCompassRing = imageBundle.dirCat("compassring.png");
      //theCompassRose = imageBundle.dirCat("compassrose.png");
   }
   else if(installImageDir.dirCat("compassring.png").exists())
   {
      theCompassRing = installImageDir.dirCat("compassring.png");
      //theCompassRose = installImageDir.dirCat("compassrose.png");
   }
   
   if(autoHistogramStretchMode().isEmpty())
   {
      setAutoHistogramStretchMode("Linear Auto Min Max");
   }
   return true;
}

ossimFilename ossimPlanetQtApplication::compassRing()
{
   return theCompassRing;
}
#if 0
ossimFilename ossimPlanetQtApplication::compassRose()
{
   return theCompassRose;
}
#endif
QStringList ossimPlanetQtApplication::wmsSettingsSubkeyList()
{
   return settingsSubKeyList("/ossim/connections-wms");
}


QString ossimPlanetQtApplication::readWmsSettingsEntry(const QString& key)
{
   return readSettingsEntry("/ossim/connections-wms/" + key);
}

void ossimPlanetQtApplication::writeWmsSettingsEntry(const QString& key,
                                                     const QString& value)
{
   writeSettingsValue("/ossim/connections-wms/" + key, value);
}

void ossimPlanetQtApplication::removeWmsSettingsKey(const QString& key)
{
   settingsRemoveKey("/ossim/connections-wms/" + key);
}

QStringList ossimPlanetQtApplication::preferenceSettingsSubkeyList(const QString& key)
{
   if(key == "")
   {
      return settingsSubKeyList("/planet/preferences");
   }
   return settingsSubKeyList("/planet/preferences/" + key);
}

void ossimPlanetQtApplication::removePreferenceSetting(const QString& key)
{
   settingsRemoveKey("/planet/preferences/" + key);
}

QString ossimPlanetQtApplication::readPreferenceSetting(const QString& key)
{
   return readSettingsEntry("/planet/preferences/" + key);
}

void ossimPlanetQtApplication::writePreferenceSetting(const QString& key,
                                                      const QString& value)
{
   writeSettingsValue("/planet/preferences/" + key, value);
  
}

QString ossimPlanetQtApplication::readSettingsEntry(const QString& entry)
{
   return theSettings->value(entry, "").toString();
}

QStringList ossimPlanetQtApplication::settingsSubKeyList(const QString& key)
{
   theSettings->beginGroup(key);
   QStringList result = theSettings->childGroups();
   theSettings->endGroup();

   return result;
}

void ossimPlanetQtApplication::writeSettingsValue(const QString& key,
                                                  const QString& value)
{
   theSettings->setValue(key, value);
}

void ossimPlanetQtApplication::settingsRemoveKey(const QString& key)
{
   theSettings->remove(key);
}

void ossimPlanetQtApplication::addCommandLineOptions(osg::ArgumentParser& args)
{
   args.getApplicationUsage()->setApplicationName(args.getApplicationName());
   args.getApplicationUsage()->setDescription(args.getApplicationName()+" is the test application for accessing wms servers.");
   args.getApplicationUsage()->setCommandLineUsage(args.getApplicationName()+" [options] ...");
   args.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");
   args.getApplicationUsage()->addCommandLineOption("--enable-flatland", "Uses the flat land model");
   args.getApplicationUsage()->addCommandLineOption("--disable-elevation", "Uses elevation");
   args.getApplicationUsage()->addCommandLineOption("--elev-estimation", "number of levels to estimate.  A value of 4 will say 2^4 or 16 number of rows and cols.");
   args.getApplicationUsage()->addCommandLineOption("--split-metric", "set Split Metric Ratio.  Default is 3.0");
   args.getApplicationUsage()->addCommandLineOption("--elev-patchsize", "number of points in each chunk.");
   args.getApplicationUsage()->addCommandLineOption("--elev-exag", "Multiplier for the height values");
   args.getApplicationUsage()->addCommandLineOption("--elev-cache", "Cache directory for elevation");
   args.getApplicationUsage()->addCommandLineOption("--level-detail", "Maximum level of detail to split to.  Default is 16 levels");
   args.getApplicationUsage()->addCommandLineOption("--enable-hud", "Enables the lat lon read outs");
   args.getApplicationUsage()->addCommandLineOption("--disable-hud", "Disable the lat lon read outs");
   args.getApplicationUsage()->addCommandLineOption("--disable-mipmap", "Doesn't use  MipMapping");
   args.getApplicationUsage()->addCommandLineOption("--enable-mipmap", "Use  MipMapping");
   args.getApplicationUsage()->addCommandLineOption("--wms-timeout", "Time out for WMS get Capabiltites for the WmsDialog specified in seconds");
}

QString ossimPlanetQtApplication::userSupportDirectory()
{
   return theUserSupportDirectory;
}
QString ossimPlanetQtApplication::userDirectory()
{
   return theUserDirectory;
}

QString  ossimPlanetQtApplication::sessionDirectory()
{
   QString result = currentOpenSessionDirectory();
   if(result=="")
   {
      ossimFilename dir(userSupportDirectory().toStdString());
      dir = dir.dirCat("planet");
      dir = dir.dirCat("session");
      result = dir.c_str();
   }
   
   return result;
}

QString ossimPlanetQtApplication::defaultSession()
{
   ossimFilename dir(sessionDirectory().toStdString());

   dir = dir.dirCat("default.session");

   return dir.c_str();
}

QString ossimPlanetQtApplication::defaultWmsCacheDir()
{
   ossimFilename dir(userSupportDirectory().toStdString());
   dir = dir.dirCat("planet");
   dir = dir.dirCat("wms");
   dir = dir.dirCat("cache");
   
   return dir.c_str();
}
