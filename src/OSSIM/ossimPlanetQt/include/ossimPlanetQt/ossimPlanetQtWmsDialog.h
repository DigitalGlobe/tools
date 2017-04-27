#ifndef ossimPlanetQtWmsDialog_HEADER
#define ossimPlanetQtWmsDialog_HEADER
#include <ossimPlanetQt/ui_ossimPlanetQtWmsDialog.h>
#include <QtGui/QDialog>
#include <ossimPlanet/ossimPlanetWmsImageLayer.h>
#include <wms/wmsMemoryStream.h>
#include <wms/wmsCapabilitiesRoot.h>
#include <wms/wmsCapabilitiesState.h>
#include <QtCore/QSettings>
#include <ossim/base/ossimWms.h>
#include <map>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class OSSIMPLANETQT_DLL ossimPlanetQtWmsDialog : public QDialog, public Ui::ossimPlanetQtWmsDialog
{
   Q_OBJECT
public:
   struct WmsLayerSelectionInfoType
   {
      ossimString                theUrl;
      ossimRefPtr<ossimWmsLayer> theLayer;
      ossimString                theStyle;
      ossimString                theImageFormat;
      ossimFilename              theRootCacheDir;
      ossimString                theDescription;
      ossimString                theProxyHost;
      ossimString                theProxyPort;
      ossimString                theProxyUser;
      ossimString                theProxyPassword;
   };
   typedef std::map<QString, QString> ServerSelectionMapType;
   typedef std::map<unsigned int, ossimRefPtr<ossimWmsLayer> > NamedLayerMapType;
   typedef QMap<QString, QPair<QString, int> > ImageEncodingFormatType;
   typedef std::vector<WmsLayerSelectionInfoType> SelectedLayerInfoListType;
   
   ossimPlanetQtWmsDialog(QWidget* parent);
   void populateConnectionList();
   void setConnectionListPosition();
   void populateLayerList();
   void populateImageFormats();
   void populateCacheDir();
/*    bool populateLayerList */
   
/*    static QStringList subkeyList(QSettings& settings, const QString& key); */
/*    static QString readEntry(QSettings& settings, const QString& key); */

   void setSelectedItemsInfo();
   
protected slots:
   void on_theAddButton_clicked(bool checked = false);
   void on_theCloseButton_clicked(bool checked = false);
   void on_theAddDefaultServersButton_clicked(bool checked = false);
   void on_theServerConnections_activated(int);
   void on_theConnectServerButton_clicked(bool checked = false);
   void on_theNewServerButton_clicked(bool checked = false);
   void on_theEditServerButton_clicked(bool checked = false);
   void on_theDeleteServerButton_clicked(bool checked = false);
   void on_theLayersList_itemSelectionChanged();
   void on_theCacheDirButton_clicked(bool checked = false);
   void on_theCacheDir_textChanged(const QString& value);
   void imageFormatButtonClicked(QAbstractButton* button);
/*    void on_theCacheDirButton_clicked(bool checked = false); */
/*    void capabilitiesReturnPressed(); */
/*    void cacheDirReturnPressed(); */
/*    void on_theLayerList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous ); */
/*    void on_theLayerList_itemActivated ( QListWidgetItem * item ); */
signals:
   void addButtonHit(ossimPlanetQtWmsDialog*,
                     ossimRefPtr<ossimWmsCapabilitiesDocument> capabilities,
                     ossimPlanetQtWmsDialog::SelectedLayerInfoListType& layers);

protected:
   void clearFormatButtons();
   void setupHeaderLabels();
   void serverChanged();

   QString theConnectionName;
   QString theConnectionInfo;
   ossimRefPtr<ossimWmsCapabilitiesDocument> theCurrentCapabilities;
   ossimWmsLayerListType                     theNamedLayers;
   QButtonGroup*           theImageFormatGroup;
   QHBoxLayout*            theImageFormatLayout;
   SelectedLayerInfoListType theSelectedLayers;
   NamedLayerMapType         theNamedLayersMap;
   ossimString               theImageFormat; 
};

#endif
