#include <ossimPlanetQt/ossimPlanetQtPreferences.h>

ossimPlanetQtPreferences::ossimPlanetQtPreferences(QWidget *parent)
   : QDialog(parent)
{
   setupUi(this);
}

void ossimPlanetQtPreferences::on_theOkButton_clicked(bool checked)
{
   close();
}
