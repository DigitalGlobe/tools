#include <ossimPlanetQt/ossimPlanetQtWmsDialog.h>
#include <QtCore/QSettings>
#include <wms/wmsClient.h>
#include <QtGui/QButtonGroup>
#include <QtGui/QRadioButton>
#include <QtGui/QHeaderView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeWidgetItemIterator>
#include <QtGui/QFileDialog>
#include <ossimPlanetQt/ossimPlanetQtNewHttpConnection.h>
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossimPlanetQt/ossimPlanetQtApplication.h>

ossimPlanetQtWmsDialog::ossimPlanetQtWmsDialog(QWidget* parent)
   :QDialog(parent)
{
   setupUi(this);
   setupHeaderLabels();
   theImageFormatGroup  = new QButtonGroup;
   theImageFormatLayout = new QHBoxLayout;
   theGroupImageEncoding->setLayout(theImageFormatLayout);
   connect(theImageFormatGroup,
           SIGNAL(buttonClicked(QAbstractButton*)),
           this,
           SLOT(imageFormatButtonClicked(QAbstractButton*)));
           
   populateConnectionList();
}

void ossimPlanetQtWmsDialog::populateImageFormats()
{
   if(theCurrentCapabilities.valid())
   {
      clearFormatButtons();
      const ossimRefPtr<ossimWmsGetMap> getMap = theCurrentCapabilities->getRequestGetMap();
      if(getMap.valid())
      {
         const ossimWmsStringListType& formatTypes = getMap->getFormatTypes();
         ossim_uint32 idx = 0;
         for(idx = 0; idx < formatTypes.size(); ++idx)
         {
            ossimString tempType = formatTypes[idx];
            tempType = tempType.downcase();
            if(tempType.contains("image/"))
            {
               tempType = tempType.substitute("image/", "");
            }
            QRadioButton* btn = new QRadioButton (tempType.c_str());
            theImageFormatGroup->addButton(btn, idx);
            theImageFormatLayout->addWidget(btn);
            if(idx == 0)
            {
               theImageFormat = ("image/"+tempType);
               btn->setChecked(true);
            }
         }
      }
      else  // we need some format
      {
         QRadioButton* btn = new QRadioButton("jpeg");
         theImageFormatGroup->addButton(btn, 0);
         theImageFormatLayout->addWidget(btn);
         btn->setChecked(true);
         theImageFormat = "image/jpeg";
         btn = new QRadioButton("png");
         theImageFormatGroup->addButton(btn, 1);
         theImageFormatLayout->addWidget(btn);
      }
      theImageFormatLayout->addStretch();
   }
}
void ossimPlanetQtWmsDialog::populateConnectionList()
{
//   QSettings settings;
  
  QStringList keys = ossimPlanetQtApplication::wmsSettingsSubkeyList();//subkeyList(settings, "/ossim/connections-wms");
  
  QStringList::Iterator it = keys.begin();
  theServerConnections->clear();

  while(it!=keys.end())
  {
     theServerConnections->addItem(*it);
     ++it;
  }
  setConnectionListPosition();
  
  if(keys.begin()==keys.end())
  {
     // No connections - disable various buttons
     theConnectServerButton->setEnabled(false);
     theEditServerButton->setEnabled(false);
     theDeleteServerButton->setEnabled(false);
  }
  else
  {
     // Connections - enable various buttons
     theConnectServerButton->setEnabled(true);
     theEditServerButton->setEnabled(true);
     theDeleteServerButton->setEnabled(true);
  }
}

void ossimPlanetQtWmsDialog::setConnectionListPosition()
{
//   QSettings settings;
   QString toSelect = ossimPlanetQtApplication::readWmsSettingsEntry("selected");//"/ossim/connections-wms/selected");
   bool set = false;
   for (int i = 0; ((i < theServerConnections->count())&&!set); ++i)
   {
      if (theServerConnections->itemText(i) == toSelect)
      {
         theServerConnections->setCurrentIndex(i);
         set = true;
      }
   }
   // If we couldn't find the stored item, but there are some, 
   // default to the last item (this makes some sense when deleting
   // items as it allows the user to repeatidly click on delete to
   // remove a whole lot of items).
   if (!set && theServerConnections->count() > 0)
   {
      // If toSelect is null, then the selected connection wasn't found
      // by QSettings, which probably means that this is the first time
      // the user has used qgis with database connections, so default to
      // the first in the list of connetions. Otherwise default to the last.
      if (toSelect.isNull())
         theServerConnections->setCurrentIndex(0);
      else
         theServerConnections->setCurrentIndex(theServerConnections->count()-1);
   }
   populateCacheDir();
}

void ossimPlanetQtWmsDialog::setupHeaderLabels()
{
   QStringList headerLabels;

   headerLabels.push_back("Id");
   headerLabels.push_back("Name");
   headerLabels.push_back("Title");
   headerLabels.push_back("Abstract");
   theLayersList->setColumnCount(4);
   theLayersList->setHeaderLabels(headerLabels);
}

void ossimPlanetQtWmsDialog::serverChanged()
{
  // Remember which server was selected.
//   QSettings settings;
//   settings.setValue("/ossim/connections-wms/selected", 
//                     theServerConnections->currentText());
   ossimPlanetQtApplication::writeWmsSettingsEntry("selected",
                                                   theServerConnections->currentText());
  populateCacheDir();
}

void ossimPlanetQtWmsDialog::on_theAddButton_clicked(bool /*checked*/)
{
   if(theCacheDir->text() == "")
   {
      QMessageBox::warning(this,
                           QString("Directory Cache Not Set"),
                           QString("Please specify a directory cache for this server\n"),
                           QMessageBox::Ok,
                           QMessageBox::NoButton);
      return;
   }
   
   emit addButtonHit(this, theCurrentCapabilities, theSelectedLayers);
}

void ossimPlanetQtWmsDialog::on_theCloseButton_clicked(bool /*checked*/)
{
   close();
}

void ossimPlanetQtWmsDialog::on_theAddDefaultServersButton_clicked(bool /*checked*/)
{
  QMap<QString, QString> exampleServers;
  exampleServers["NASA"]       = "http://neowms.sci.gsfc.nasa.gov/wms/wms?Version=1.1.1&Service=WMS&Request=GetCapabilities";
  exampleServers["NASA (JPL)"] = "http://wms.jpl.nasa.gov/wms.cgi?Version=1.1.1&Service=WMS&Request=GetCapabilities";
  exampleServers["BMNG"]       = "http://wms.telascience.org/cgi-bin/bmng_wms_t?Version=1.1.1&Service=WMS&Request=GetCapabilities";

//   QSettings settings;
  QString   servers;
//   QString basePath("/ossim/connections-wms/");
  QStringList keys = ossimPlanetQtApplication::wmsSettingsSubkeyList();//subkeyList(settings, basePath);
  
  for (QMap<QString, QString>::const_iterator i = exampleServers.constBegin();
       i != exampleServers.constEnd();
       ++i)
  {
     // Only do a server if it's name doesn't already exist.
//     QStringList keys = settings.subkeyList(basePath);
     
     if (!keys.contains(i.key()))
     {
        if(servers == "")
        {
           servers += i.key();
        }
        else
        {
           servers = servers + ", " + i.key();
        }
        ossimPlanetQtApplication::writeWmsSettingsEntry(i.key()+"/proxyhost", "");
        ossimPlanetQtApplication::writeWmsSettingsEntry(i.key()+"/proxyport", "");
        ossimPlanetQtApplication::writeWmsSettingsEntry(i.key()+"/proxyuser", "");
        ossimPlanetQtApplication::writeWmsSettingsEntry(i.key()+"/proxypassword", "");
        ossimPlanetQtApplication::writeWmsSettingsEntry(i.key()+"/url", i.value());
     }
  }
  populateConnectionList();
  if(servers != "")
  {
     QMessageBox::information(this, tr("WMS proxies"),
                              tr("<p>Several WMS servers ") + servers + tr(" have been added to the server list. Note that the proxy fields have been left blank and if you access the internet via a web proxy, you will need to individually set the proxy fields with appropriate values.</p>"));
  }
  else
  {
     QMessageBox::information(this, tr("WMS proxies"),
                              tr("<p>Default WMS servers already exist. The current list was not changed</p>"));
  }

}

void ossimPlanetQtWmsDialog::on_theServerConnections_activated(int)
{
   serverChanged();
}

void ossimPlanetQtWmsDialog::on_theConnectServerButton_clicked(bool)
{
  // populate the table list
//   QSettings settings;

//   QString key = "/ossim/connections-wms/" + theServerConnections->currentText();
   
   QString connStringParts = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText() + "/url");//readEntry(settings, key + "/url");
   QString proxyHost = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText() + "/proxyhost");//readEntry(settings, key + "/proxyhost");
   QString proxyPort = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText() + "/proxyport");//readEntry(settings, key + "/proxyport");
   QString proxyUser = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText() + "/proxyuser");//readEntry(settings, key + "/proxyuser");
   QString proxyPassword = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText() + "/proxypassword");//readEntry(settings, key + "/proxypassword");
   theConnectionInfo = connStringParts;
   theConnectionName = theServerConnections->currentText();
   wmsClient client;
   client.setTimeout(static_cast<ossim_uint32>(ossimPlanetQtApplication::wmsNetworkTimeoutInSeconds()));
   client.setMaxNumberRetry(0);
   client.setProxyHost(proxyHost.toStdString());
   client.setProxyPort(proxyPort.toStdString());
   client.setProxyUser(proxyUser.toStdString());
   client.setProxyPassword(proxyPassword.toStdString());
   if(client.getCapabilities(theConnectionInfo.toStdString()))
   {
      ossimRefPtr<ossimWmsCapabilitiesDocument> cap = new ossimWmsCapabilitiesDocument;
      ossimString buf = client.getStream()->getBufferAsString();
      if(cap->read(buf))
      {
         theCurrentCapabilities = cap.get();
         populateImageFormats();
         populateLayerList();
      }
      else if(buf.size() > 0)
      {
         std::cout << "*****************Error: The Result is not a capabilitites string*******************\n" << buf << std::endl;
      }
   }
   else
   {
      QMessageBox::information(this,
                               "Get Capabilities",
                               "Unable to get capabilities from " +theConnectionInfo,
                               QMessageBox::Ok);
      
   }
}

void ossimPlanetQtWmsDialog::on_theNewServerButton_clicked(bool)
{
  ossimPlanetQtNewHttpConnection *nc = new ossimPlanetQtNewHttpConnection(this);
  nc->setWindowTitle("New WMS Connection");
  if(nc->exec())
  {
     populateConnectionList();
  }
}

void ossimPlanetQtWmsDialog::on_theEditServerButton_clicked(bool)
{
   ossimPlanetQtNewHttpConnection *nc = new ossimPlanetQtNewHttpConnection(this,
                                                                           theServerConnections->currentText());
   nc->theName->setReadOnly(true);
   nc->setWindowTitle("Edit WMS Connection");
   if(nc->exec())
   {
      nc->saveConnection();
   }
}

void ossimPlanetQtWmsDialog::on_theDeleteServerButton_clicked(bool)
{
//    QSettings settings;
//    QString key = "/ossim/connections-wms/" + theServerConnections->currentText();
   QString msg =(tr("Are you sure you want to remove the ") +
                 theServerConnections->currentText() +
                 tr(" connection and all associated settings?"));
  QMessageBox::StandardButton result = QMessageBox::information(this,
                                                                tr("Confirm Delete"),
                                                                msg,
                                                                QMessageBox::Ok | QMessageBox::Cancel);
  if(result == QMessageBox::Ok)
  {
     ossimPlanetQtApplication::removeWmsSettingsKey(theServerConnections->currentText());
     theServerConnections->removeItem(theServerConnections->currentIndex());
     setConnectionListPosition();
  }
 
}

void ossimPlanetQtWmsDialog::on_theLayersList_itemSelectionChanged()
{
   setSelectedItemsInfo();
}

void ossimPlanetQtWmsDialog::on_theCacheDirButton_clicked(bool /*checked*/)
{
   QString directory = QFileDialog::getExistingDirectory(this,
                                                         "Choose wms cache location",
                                                         theCacheDir->text());
   if(directory!="")
   {
      theCacheDir->setText(directory);
   }
}

void ossimPlanetQtWmsDialog::on_theCacheDir_textChanged(const QString& value)
{
//    QSettings settings;
//    QString key = "/ossim/connections-wms/" + theServerConnections->currentText()+"/cache";
//    settings.setValue(key,
//                      value);
   ossimPlanetQtApplication::writeWmsSettingsEntry(theServerConnections->currentText()+"/cache",
                                                   value);
}

void ossimPlanetQtWmsDialog::imageFormatButtonClicked(QAbstractButton* button)
{
   if(button)
   {
      theImageFormat = ("image/" + button->text()).toStdString();
      theImageFormat = theImageFormat.downcase();
      setSelectedItemsInfo();
   }
}

void ossimPlanetQtWmsDialog::populateCacheDir()
{
   theCacheDir->blockSignals(true);
//    QSettings settings;
//    QString key = "/ossim/connections-wms/" + theServerConnections->currentText()+"/cache";
   QString entry = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText()+"/cache");//readEntry(settings, key);
   entry = entry.simplified();
   if(entry=="")
   {
      ossimFilename file = ossimEnvironmentUtility::instance()->getUserOssimSupportDir();
      file = file.dirCat("planet");
      file = file.dirCat("wms");
      file = file.dirCat("cache");
      ossimPlanetQtApplication::writeWmsSettingsEntry(theServerConnections->currentText()+"/cache", file.c_str());
//       QSettings settings;
//       settings.setValue("/ossim/connections-wms/"+ theServerConnections->currentText()+"/cache", 
//                         file.c_str());
      entry = file.c_str();

   }
   theCacheDir->setText(entry);
   theCacheDir->blockSignals(false);
}

void ossimPlanetQtWmsDialog::setSelectedItemsInfo()
{
   theSelectedLayers.clear();
   
   if(!theCurrentCapabilities.valid())
   {
      return;
   }
   QTreeWidgetItemIterator iter(theLayersList, QTreeWidgetItemIterator::Selected);
   theSelectedLayers.clear();
   ossimString urlGetMap;
   ossimRefPtr<ossimWmsGetMap> getMap = theCurrentCapabilities->getRequestGetMap();
   if(getMap.valid())
   {
      urlGetMap = getMap->getUrl();
   }
   if(urlGetMap.empty())
   {
      urlGetMap = theConnectionInfo.toStdString();
      // we will try the capabilities url for the get map
   }
   WmsLayerSelectionInfoType info;
   info.theUrl   = urlGetMap;
   info.theImageFormat = theImageFormat;
   bool validToAdd = false;
//    QSettings settings;
//    QString key(("/ossim/connections-wms/" + theServerConnections->currentText().toStdString()).c_str());
   QString proxyHost = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText()+"/proxyhost");//readEntry(settings, key + "/proxyhost");
   QString proxyPort = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText()+"/proxyport");//readEntry(settings, key + "/proxyport");
   QString proxyUser = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText()+"/proxyuser");//readEntry(settings, key + "/proxyuser");
   QString proxyPassword = ossimPlanetQtApplication::readWmsSettingsEntry(theServerConnections->currentText()+"/proxypassword");//readEntry(settings, key + "/proxypassword");
   while(*iter)
   {
      validToAdd = true;
      QString layerId;
      info.theLayer = 0;
      info.theStyle = "";
      if((*iter)->parent()) // this is a style item
      {
         layerId = (*iter)->parent()->text(0);
         info.theStyle = (*iter)->text(1).toStdString();
         // use the parent to get the layer id
      }
      else if(!(*iter)->isExpanded()) // use the first child style if the current selected layer is not expanded
      {
         layerId = (*iter)->text(0);
         ossimString style;
         if((*iter)->childCount()>0)
         {
            if((*iter)->child(0))
            {
               info.theStyle = (*iter)->child(0)->text(1).toStdString();
            }
         }
      }
      else
      {
         validToAdd = false;
      }
      
      if(validToAdd)
      {
         NamedLayerMapType::iterator layerIter = theNamedLayersMap.find(layerId.toUInt());
         if(layerIter!=theNamedLayersMap.end())
         {
            ossimFilename dirCache;
            wmsUrl url(info.theUrl.string());
            ossimString urlString = url.server();
            dirCache = ossimFilename(theCacheDir->text().toStdString());
            info.theRootCacheDir = dirCache;
            info.theLayer = layerIter->second;
            if(info.theStyle.empty())
            {
               info.theDescription = (theServerConnections->currentText().toStdString()+
                                      ": " +
                                      info.theLayer->getName());
            }
            else
            {
               info.theDescription = (theServerConnections->currentText().toStdString()+
                                      ": " +
                                      info.theLayer->getName() + " " + info.theStyle);
            }
            info.theProxyHost = proxyHost.toStdString();
            info.theProxyPort = proxyPort.toStdString();
            info.theProxyUser = proxyUser.toStdString();
            info.theProxyPassword = proxyPassword.toStdString();
            theSelectedLayers.push_back(info);
         }
      }
      
      ++iter;
   }
}

void ossimPlanetQtWmsDialog::populateLayerList()
{
   theLayersList->clear();
   setupHeaderLabels();
   if(!theCurrentCapabilities.valid()) return;
   if(!theCurrentCapabilities->getCapability().valid()) return;
   ossim_uint32 idx = 0;
   theNamedLayers.clear();
   theNamedLayersMap.clear();
   theCurrentCapabilities->getCapability()->getNamedLayers(theNamedLayers);

   ossim_uint32 idIdx = 0;
   for(idx =0; idx < theNamedLayers.size();++idx)
   {
      QTreeWidgetItem* item =  new QTreeWidgetItem(theLayersList);
      item->setText(0, QString::number(idIdx));
      item->setText(1, QString(theNamedLayers[idx]->getName().c_str()).simplified());
      item->setText(2, QString(theNamedLayers[idx]->getTitle().c_str()).simplified());
      item->setText(3, QString(theNamedLayers[idx]->getAbstract().c_str()).simplified());
      ossim_uint32 styleIdx = 0;
      const ossimWmsStyleListType& styles = theNamedLayers[idx]->getStyles();
      theNamedLayersMap.insert(std::make_pair(idIdx, theNamedLayers[idx]));
      ++idIdx;
      for(styleIdx = 0; styleIdx < styles.size(); ++styleIdx)
      {
         QTreeWidgetItem* styleItem =  new QTreeWidgetItem(item);
         styleItem->setText(0, QString::number(idIdx));
         styleItem->setText(1, QString(styles[styleIdx]->getName().c_str()).simplified());
         styleItem->setText(2, QString(styles[styleIdx]->getTitle().c_str()).simplified());
         styleItem->setText(3, QString(styles[styleIdx]->getAbstract().c_str()).simplified());
         ++idIdx;
      }
   }
   theLayersList->resizeColumnToContents(0);
   theLayersList->resizeColumnToContents(1);
   theLayersList->resizeColumnToContents(2);
   theLayersList->resizeColumnToContents(3);
}


// QStringList ossimPlanetQtWmsDialog::subkeyList(QSettings& settings, const QString& key)
// {
//    settings.beginGroup(key);
//    QStringList result = settings.childGroups();
//    settings.endGroup();

//    return result;
// }

// QString ossimPlanetQtWmsDialog::readEntry(QSettings& settings, const QString& key)
// { 
//    return settings.value(key, "").toString();  
// }

void ossimPlanetQtWmsDialog::clearFormatButtons()
{
   QList<QAbstractButton *> buttons = theImageFormatGroup->buttons();
   for(QList<QAbstractButton *>::iterator iter = buttons.begin();
       iter != buttons.end();++iter)
   {
      theImageFormatGroup->removeButton(*iter);
      delete *iter;
   }
   QLayoutItem *child;
   while ((child = theImageFormatLayout->takeAt(0)) != 0)
   {  
      delete child;
   }
}
