#ifndef ossimQtGeometryBuilderDialogContrioller_HEADER
#define ossimQtGeometryBuilderDialogContrioller_HEADER
#include <QtCore/QObject>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>

class ossimQtGeometryBuilderDialog;
class ossimQtGeometryBuilderDialogController : public QObject
{
  Q_OBJECT
public:
  ossimQtGeometryBuilderDialogController(ossimQtGeometryBuilderDialog* dialog);
  void convertFromWkt();
  void makeAdjustable();
  void saveOssimGeometry();
  void loadWktStringFromFile();
  void generateDefaultOssimGeometry(const char* className=0);

signals:
  void saveOssimGeometryOverride(bool& overrideFlag,
				 const ossimKeywordlist& geomKwl);

protected:
  void generateProjections();

  ossimQtGeometryBuilderDialog* theDialog;
  ossimFilename                 theFilename;
  ossimFilename                 theWktStringFilename;
};

#endif
