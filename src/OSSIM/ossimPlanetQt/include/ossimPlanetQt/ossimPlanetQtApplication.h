#ifndef ossimPlanetQtApplication_HEADER
#define ossimPlanetQtApplication_HEADER
#include <QtGui/QApplication>
#include <osg/ArgumentParser>
#include <osg/ApplicationUsage>
#include <ossimPlanet/ossimPlanetConstants.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossimPlanet/ossimPlanetArchiveMapping.h>
#include <deque>
#include <ossimPlanet/ossimPlanetTextureLayerRegistry.h>
#include <ossimPlanet/ossimPlanetTextureLayer.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <QtCore/QSettings>

class OSSIMPLANETQT_DLL ossimPlanetQtApplication: public QApplication
{
public:
   typedef std::deque<osg::ref_ptr<ossimPlanetTextureLayer> > ReferenceImageListType;
   virtual ~ossimPlanetQtApplication();
   ossimPlanetQtApplication(int & argc, char ** argv, bool guiEnabled=true);
   
   
   static QString userSupportDirectory();
   static QString userDirectory();
   static QString sessionDirectory();
   static QString defaultSession();
   static QString defaultWmsCacheDir();
   static QString currentOpenSessionDirectory()
   {
      return readPreferenceSetting("current-open-session-directory");
   }
   static void setCurrentOpenSessionDirectory(const QString& dir)
   {
      writePreferenceSetting("current-open-session-directory",
                             dir);
   }
   static QString currentOpenImageDirectory()
   {
      return readPreferenceSetting("current-open-image-directory");
   }
   static void setCurrentOpenImageDirectory(const QString& dir)
   {
      writePreferenceSetting("current-open-image-directory",
                             dir);
   }
   static QString currentOpenKmlDirectory()
   {
      return readPreferenceSetting("current-open-kml-directory");
   }
   static void setCurrentOpenKmlDirectory(const QString& dir)
   {
      writePreferenceSetting("current-open-kml-directory",
                             dir);
   }
   static QString currentOpenVideoDirectory()
   {
      return readPreferenceSetting("current-open-video-directory");
   }
   static void setCurrentOpenVideoDirectory(const QString& dir)
   {
      writePreferenceSetting("current-open-video-directory",
                             dir);
   }
   static void addCommandLineOptions(osg::ArgumentParser& args);
   static bool initWithArgs(int& argc, char** argv);
   static ossimFilename compassRing();
  // static ossimFilename compassRose();
   
   static QString stagingCacheDirectory()
   {
      return readPreferenceSetting("staging-cache-directory");
   }
   static void setStagingCacheDirectory(const QString& dir)
   {
      writePreferenceSetting("staging-cache-directory",
                             dir);
   }
   static bool autoGenerateOverviewFlag()
   {
      ossimString result = readPreferenceSetting("auto-generate-overviews-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      
      return result.toBool();
   }
   static void setAutoGenerateOverviewFlag(bool flag)
   {
      writePreferenceSetting("auto-generate-overviews-flag",
                             ossimString::toString(flag).c_str());
   }
   static void setAutoHistogramStretchMode(const QString& setting)
   {
      writePreferenceSetting("auto-histogram-stretch-mode",
                             setting);
   }
   static QString autoHistogramStretchMode()
   {
      return readPreferenceSetting("auto-histogram-stretch-mode");
   }
   static double wmsNetworkTimeoutInSeconds()
   {
      ossimString result = readPreferenceSetting("wms-network-timeout").toStdString();
      if(!result.empty())
      {
         return result.toUInt32();
      }
      
      return 10;
   }
   static void setWmsNetworkTimeoutInSeconds(double seconds)
   {
      writePreferenceSetting("wms-network-timeout",
                             ossimString::toString(seconds).c_str());
      
   }
   static bool autoViewSyncFlag()
   {
      ossimString result = readPreferenceSetting("auto-view-sync-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      return result.toBool();       
      
   }
   static void setAutoViewSyncFlag(bool flag)
   {
      writePreferenceSetting("auto-view-sync-flag",
                             ossimString::toString(flag).c_str());
   }
   static double autoViewSyncRateInSeconds()
   {
      ossimString result = readPreferenceSetting("auto-view-sync-rate").toStdString();
      if(result.empty())
      {
         return .05;
      }
      return result.toDouble();       
   }
   static void setAutoViewSyncRateInSeconds(double value)
   {
      writePreferenceSetting("auto-view-sync-rate",
                             ossimString::toString(value).c_str());       
   }
   static bool autoHighlightLineOfSitFlag()
   {
      ossimString result = readPreferenceSetting("auto-highlight-line-of-site-flag").toStdString();
      if(result.empty())
      {
         return false;
      }
      return result.toBool();       
   }
   static void setAutoHighlightLineOfSitFlag(bool flag)
   {
      writePreferenceSetting("auto-highlight-line-of-site-flag",
                             ossimString::toString(flag).c_str());
      
   }
   static void setElevFlag(bool flag)
   {
      writePreferenceSetting("elev-flag", ossimString::toString(flag).c_str());
   }
   static bool elevEnabled()
   {
      ossimString result = readPreferenceSetting("elev-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      return result.toBool();
      /*        return theElevEnabled; */
   }
   static std::string textureDetailType()
   {
      ossimString result = readPreferenceSetting("texture-detail-type").toStdString();
      if(result.empty())
      {
         result = "medium";
      }
      return result;
   }
   static void setTextureDetailType(const std::string& type)
   {
      writePreferenceSetting("texture-detail-type", type.c_str());
   }
   static std::string elevationDetailType()
   {
      ossimString result = readPreferenceSetting("elevation-detail-type").toStdString();
      if(result.empty())
      {
         result = "medium-low";
      }
      return result;
   }
   static void setElevationDetailType(const std::string& type)
   {
      writePreferenceSetting("elevation-detail-type", type.c_str());
   }
   static std::string cullAmountType()
   {
      ossimString result = readPreferenceSetting("cull-amount-type").toStdString();
      if(result.empty())
      {
         result = "high";
      }
      return result;
   }
   static void setCullAmountType(const std::string& type)
   {
      writePreferenceSetting("cull-amount-type", type.c_str());
   }
   static void setElevExag(ossim_float32 value)
   {
      writePreferenceSetting("elev-exag", ossimString::toString(value).c_str());
   }
   
   static ossim_float32 elevExag()
   {
      ossimString result = readPreferenceSetting("elev-exag").toStdString();
      if(result.empty())
      {
         return 1.0;
      }
      
      return result.toDouble();
      /*        return theElevExag; */
   }
   static ossim_float32 splitMetricRatio()
   {
      ossimString result = readPreferenceSetting("split-metric").toStdString();
      if(result.empty())
      {
         return 3.0;
      }
      
      return result.toDouble();
      /*        return theSplitMetricRatio; */
   }
   static ossim_uint32 elevEstimate()
   {
      ossimString result = readPreferenceSetting("elev-patchsize").toStdString();
      if(result.empty())
      {
         return 16;
      }
      
      return result.toUInt32();
      /*        return theElevEstimate; */
   }
   static void setEphemerisEnabledFlag(bool flag)
   {
      writePreferenceSetting("ephemeris-flag", ossimString::toString(flag).c_str());
   }
   
   static bool ephemerisEnabled()
   {
      ossimString result = readPreferenceSetting("ephemeris-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      return result.toBool();
   }
   
   static void setEphemerisAutoUpdateToCurrentTimeFlag(bool flag)
   {
      writePreferenceSetting("ephemeris-autoupdate-current-time-flag", ossimString::toString(flag).c_str());
   }
   static bool ephemerisAutoUpdateToCurrentTimeFlag()
   {
      ossimString result = readPreferenceSetting("ephemeris-autoupdate-current-time-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      return result.toBool();
   }
   static void setSunlightEnabledFlag(bool flag)
   {
      writePreferenceSetting("sunlight-flag", ossimString::toString(flag).c_str());
   }
   
   static bool sunlightEnabled()
   {
      ossimString result = readPreferenceSetting("sunlight-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      return result.toBool();
      /*        return theElevEnabled; */
   }
   static void setMoonEnabledFlag(bool flag)
   {
      writePreferenceSetting("moon-flag", ossimString::toString(flag).c_str());
   }
   
   static bool moonEnabled()
   {
      ossimString result = readPreferenceSetting("moon-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      return result.toBool();
      /*        return theElevEnabled; */
   }
   
   static void setSkyEnabledFlag(bool flag)
   {
      writePreferenceSetting("skydome-flag", ossimString::toString(flag).c_str());
   }
   static bool skyEnabled()
   {
      ossimString result = readPreferenceSetting("skydome-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      return result.toBool();
   }

   static void setCloudsEnabledFlag(bool flag)
   {
      writePreferenceSetting("clouds-flag", ossimString::toString(flag).c_str());
   }
   static bool cloudsEnabled()
   {
      ossimString result = readPreferenceSetting("clouds-flag").toStdString();
      if(result.empty())
      {
         return false;
      }
      return result.toBool();
   }
   static void setCloudCoverage(int value)
   {
      writePreferenceSetting("cloud-coverage", ossimString::toString(value).c_str());
   }
   static int cloudCoverage()
   {
      ossimString result = readPreferenceSetting("cloud-coverage").toStdString();
      if(result.empty())
      {
         return 20;
      }
      return result.toInt32();
   }
   static void setCloudSharpness(double value)
   {
      writePreferenceSetting("cloud-sharpness", ossimString::toString(value).c_str());
   }
   static double cloudSharpness()
   {
      ossimString result = readPreferenceSetting("cloud-sharpness").toStdString();
      if(result.empty())
      {
         return .96;
      }
      return result.toDouble();
   }
   static void setPercentVisibility(int value)
   {
      writePreferenceSetting("percent-visibility", ossimString::toString(value).c_str());
   }
   static int percentVisibility()
   {
      ossimString result = readPreferenceSetting("percent-visibility").toStdString();
      if(result.empty())
      {
         return 100;
      }
      return result.toInt32();
   }
   static const ossimFilename elevCacheDir()
   {
      ossimString result = readPreferenceSetting("elev-cache").toStdString();
      
      return ossimFilename(result);
      /*        return theElevCache; */
   }
   /*     static ossimPlanetLandType landType() */
   /*     { */
   /*        return theLandType; */
   /*     } */
   static ossim_uint32 levelOfDetail()
   {
      ossimString result = readPreferenceSetting("level-of-detail").toStdString();
      if(result.empty())
      {
         return 20;
      }
      return result.toUInt32();
      /*        return theLevelOfDetail; */
   }
   static void setAutomaticStagingEnabledFlag(bool flag)
   {
      writePreferenceSetting("automatic-staging-flag", ossimString::toString(flag).c_str());
   }
   static bool automaticStagingEnabledFlag()
   {
      ossimString result = readPreferenceSetting("automatic-staging-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      
      return result.toBool();
   }
   static void setHudFlag(bool flag)
   {
      writePreferenceSetting("hud-flag", ossimString::toString(flag).c_str());
   }
   
   static bool hudEnabled()
   {
      ossimString result = readPreferenceSetting("hud-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      
      return result.toBool();
      /*        return theHudEnabled; */
   }
   static bool mipMapping()
   {
      ossimString result = readPreferenceSetting("mipmap-flag").toStdString();
      if(result.empty())
      {
         return true;
      }
      
      return result.toBool();
      /*        return theMipMapping; */
   }
   
	static void setArchiveMappingEnabledFlag(bool flag)
	{
		writePreferenceSetting("Sousa/archive-mapping-enabled", ossimString::toString(flag).c_str());
	}
	static bool archiveMappingEnabled()
	{
		ossimString result = readPreferenceSetting("Sousa/archive-mapping-enabled").toStdString();
		if( result.empty() )
		{
			return true;
		}
		return result.toBool();
	}
   static void setSousaIdentityUserName(const QString& value)
   {
		writePreferenceSetting("Sousa/Identity/username", value);
   }
   static QString sousaIdentityUserName()
   {
		return readPreferenceSetting("Sousa/Identity/username");
   }
   static void setSousaIdentityDomain(const QString& value)
   {
		writePreferenceSetting("Sousa/Identity/domain", value);
   }
   static QString sousaIdentityDomain()
   {
		return readPreferenceSetting("Sousa/Identity/domain");
   }
   static void setSousaConnectionAutoConnectOnStart(bool value)
   {
		writePreferenceSetting("Sousa/Connection/autoConnectOnStart", ossimString::toString(value).c_str());
   }
   static bool sousaConnectionAutoConnectOnStart()
   {
		ossimString result = readPreferenceSetting("Sousa/Connection/autoConnectOnStart").toStdString();
		if( result.empty() )
		{
			return true;
		}
		return result.toBool();
   }
   
   static void setSousaConnectionName(const QString& value)
   {
		writePreferenceSetting("Sousa/Connection/name", value);
   }
   static QString sousaConnectionName()
   {
		return readPreferenceSetting("Sousa/Connection/name");
   }
   static void setSousaConnectionIp(const QString& value)
   {
		writePreferenceSetting("Sousa/Connection/ip", value);
   }
   static QString sousaConnectionIp()
   {
		return readPreferenceSetting("Sousa/Connection/ip");
   }
   static void setSousaConnectionPort(const QString& value)
   {
		writePreferenceSetting("Sousa/Connection/port", value);
   }
   static QString sousaConnectionPort()
   {
		return readPreferenceSetting("Sousa/Connection/port");
   }
   static void setSousaConnectionPortType(const QString& value)
   {
		writePreferenceSetting("Sousa/Connection/portType", value);
   }
   static QString sousaConnectionPortType()
   {
		return readPreferenceSetting("Sousa/Connection/portType");
   }
   static std::deque<osg::ref_ptr<ossimPlanetTextureLayer> >& referenceImages()
   {
      return theReferenceImages;
   }
   
   static ossimFilename themePath()
   {
      return theThemePath.dirCat("default");
   }
   static QStringList wmsSettingsSubkeyList();
   static QString readWmsSettingsEntry(const QString& key);
   static void writeWmsSettingsEntry(const QString& key,
                                     const QString& value);
   static void removeWmsSettingsKey(const QString& key);
   
   static QStringList preferenceSettingsSubkeyList(const QString& key);
   static void removePreferenceSetting(const QString& key);
   static QString readPreferenceSetting(const QString& key);
   static void    writePreferenceSetting(const QString& key,
                                         const QString& value);
   
   static QString readSettingsEntry(const QString& entry);
   static QStringList settingsSubKeyList(const QString& key);
   static void writeSettingsValue(const QString& key,
                                  const QString& value);
   static void settingsRemoveKey(const QString& key);    
   static long initInputDevices();
   static void terminateInputDevices();
protected:
   static QSettings*                           theSettings;
   static QString                              theUserSupportDirectory;
   static QString                              theUserDirectory;
   static ReferenceImageListType               theReferenceImages;
   static std::vector<ossimFilename>           thePlugins;
   static ossimFilename                        theThemePath;
   static ossimFilename                        theCompassRing; 
  // static ossimFilename                        theCompassRose; 
};


#endif
