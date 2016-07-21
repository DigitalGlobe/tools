#include <ossimPlanetQt/ossimPlanetQtNewHttpConnection.h>
#include <QtCore/QSettings>
#include <ossimPlanetQt/ossimPlanetQtApplication.h>

ossimPlanetQtNewHttpConnection::ossimPlanetQtNewHttpConnection(QWidget* parent,
                                                               const QString& connName,
                                                               Qt::WFlags fl)
   :QDialog(parent),
    theConnectionName(connName)
{
   setupUi(this);
   connect(theCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
   connect(theOkButton, SIGNAL(clicked()), this, SLOT(saveConnection()));
   
   if (!connName.isEmpty())
   {
      // populate the dialog with the information stored for the connection
      // populate the fields with the stored setting parameters
      
//       QSettings settings;
      
      QString key = connName;
      theName->setText     (connName);
      theUrl->setText      (ossimPlanetQtApplication::readWmsSettingsEntry(key+ "/url"));
      theProxyHost->setText(ossimPlanetQtApplication::readWmsSettingsEntry(key + "/proxyhost"));
      theProxyPort->setText(ossimPlanetQtApplication::readWmsSettingsEntry(key + "/proxyport"));
      theProxyUser->setText(ossimPlanetQtApplication::readWmsSettingsEntry(key + "/proxyuser"));
      theProxyPassword->setText(ossimPlanetQtApplication::readWmsSettingsEntry(key + "/proxypassword"));
   }
}

void ossimPlanetQtNewHttpConnection::saveConnection()
{
   QString key = theName->text();
   
   ossimPlanetQtApplication::writeWmsSettingsEntry(key + "/url", theUrl->text().trimmed());
   ossimPlanetQtApplication::writeWmsSettingsEntry(key + "/proxyhost", theProxyHost->text().trimmed());
   ossimPlanetQtApplication::writeWmsSettingsEntry(key + "/proxyport", theProxyPort->text().trimmed());
   ossimPlanetQtApplication::writeWmsSettingsEntry(key + "/proxyuser", theProxyUser->text().trimmed());
   ossimPlanetQtApplication::writeWmsSettingsEntry(key + "/proxypassword", theProxyPassword->text().trimmed());
  
   accept();
}
