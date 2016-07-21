#ifndef ossimPlanetQtPreferences_HEADER
#define ossimPlanetQtPreferences_HEADER
#include <ossimPlanetQt/ui_ossimPlanetQtPreferences.h>
#include <QtGui/QDialog>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class ossimPlanetQtPreferences : public QDialog, public Ui::ossimPlanetQtPreferences
{
   Q_OBJECT
public:
   ossimPlanetQtPreferences(QWidget* parent=0);

protected slots:
   void on_theOkButton_clicked(bool checked = false);
};
#endif
