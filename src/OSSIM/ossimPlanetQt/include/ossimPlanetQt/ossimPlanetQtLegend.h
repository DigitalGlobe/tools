#ifndef ossimPlanetQtLegend_HEADER
#define ossimPlanetQtLegend_HEADER
#include <QtGui/QTreeWidget>
#include <QtGui/QKeyEvent>
#include <QtGui/QMenu>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossimPlanet/ossimPlanetKmlLayerNode.h>
#include <ossimPlanet/ossimPlanetVideoLayerNode.h>
#include <ossimPlanet/ossimPlanetTextureLayer.h>
#include <ossim/base/ossimReferenced.h>
#include <osg/Node>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <osg/AnimationPath>
#include <ossimPlanetQt/ossimPlanetQtEvent.h>
#include <map>
class ossimPlanetQtMainWindow;
class ossimPlanetQtLegendTextureItem;
class ossimPlanetQtLegendKmlItem;
class ossimPlanetQtLegendItem;
class ossimPlanetQtLegendNodeItem;
class ossimPlanetQtLegendVideoItem;

class OSSIMPLANETQT_DLL ossimPlanetQtLegend : public QTreeWidget
{
   Q_OBJECT
public:
	typedef std::map<ossimPlanetNode*, ossimPlanetQtLegendNodeItem*> NodeIndexMapType;
	typedef std::map<ossimPlanetTextureLayer*, ossimPlanetQtLegendTextureItem*> TextureLayerIndexMapType;
   ossimPlanetQtLegend(QWidget * parent = 0);

   virtual ~ossimPlanetQtLegend();

   void setMainWindow(ossimPlanetQtMainWindow* mainWindow);
   
   void populate();
   void populateLegend(ossimRefPtr<ossimXmlNode> legendNode);
   ossimPlanetQtLegendTextureItem* referenceTextureItem();
   void addAnimationPath(osg::ref_ptr<osg::AnimationPath> path);
   void addAnimationPath(ossimRefPtr<ossimXmlNode> path);
	void addTextureLayerToTop(osg::ref_ptr<ossimPlanetTextureLayer> layer);
   //void addTextureLayersToTop(ossimRefPtr<ossimXmlNode> layers);
	void addNode(osg::ref_ptr<ossimPlanetNode> node);
	void addBookmark(osg::ref_ptr<ossimPlanetLookAt> lookAt, const QString& name=QString(""), const QString& description=QString("") );
   
	void removeNodeFromLegend(osg::ref_ptr<ossimPlanetNode> node, bool removeFromLayerFlag=false);
	
   ossimPlanetQtMainWindow* mainWindow();
   void mouseDoubleClickEvent ( QMouseEvent * event );

   /**
    * Will add a legend as a child to the passed in root node.
    */ 
   virtual ossimRefPtr<ossimXmlNode> saveXml()const;

   /**
    * The passed in node is a legend node and will traverse and populate the legend with the passed in information
    */ 
   virtual void loadXml(ossimRefPtr<ossimXmlNode> legendNode);
   virtual void addXml(ossimRefPtr<ossimXmlNode> legendNode);
   void updateTextureLayer(ossimRefPtr<ossimPlanetTextureLayer> layer);
   
   virtual void dropEvent ( QDropEvent * event );   
   virtual void dragEnterEvent(QDragEnterEvent *event);
   void editSelectedItem();
   void deleteSelectedItems();
   void setEnableFlagOnSelectedItems(bool flag);
   void clearDiskCacheOnSelectedItems();
   void refreshSelectedItems();
   void groupSelectedItems();
   void setLookAtForSelectedItems(const osg::ref_ptr<ossimPlanetLookAt> lookAt);
   void setLookAtToCurrentOrientationForSelectedItems(const osg::ref_ptr<ossimPlanetLookAt> lookAt);
   void setLookAtToCurrentRangeForSelectedItems(double range);
   void setHistogrmaStretchModeForSelectedItems(const QString& mode);
   void synchSelectedItems();
   void unselectItems();
   
   void selectLayersContainingPosition(double lat, double lon, double hgt);
   virtual bool event(QEvent* e);
	
	void removeIndexMapping(osg::ref_ptr<ossimPlanetTextureLayer>,
									bool deleteGuiItem=false);
	void addIndexMapping(osg::ref_ptr<ossimPlanetTextureLayer>,
								ossimPlanetQtLegendTextureItem* item);
protected:
   void setHistogramStretchAndMode(ossimPlanetTextureLayer* group,
                                   bool enabledFlag,
                                   const std::string& mode);
   void setHistogramStretchAndMode(ossimPlanetTextureLayerGroup* group,
                                   bool enabledFlag,
                                   const std::string& mode);
#if 0
   ossimPlanetQtLegendTextureItem* addTextureLayerToTop(osg::ref_ptr<ossimPlanetTextureLayer> layer);
   ossimPlanetQtLegendTextureItem* addTextureLayerAfterItem(osg::ref_ptr<ossimPlanetTextureLayer> layer,
                                                            ossimPlanetQtLegendTextureItem* item);
#endif
   ossimPlanetQtLegendTextureItem* addTextureLayerToTop(osg::ref_ptr<ossimPlanetTextureLayer> layer,
                                                        ossimPlanetQtLegendTextureItem* item);
   ossimPlanetQtLegendTextureItem* addTextureLayerToBottom(osg::ref_ptr<ossimPlanetTextureLayer> layer,
																			  ossimPlanetQtLegendTextureItem* item);
   ossimPlanetQtLegendTextureItem* addTextureLayerBeforeItem(osg::ref_ptr<ossimPlanetTextureLayer> layer,
                                                             ossimPlanetQtLegendTextureItem* item);
   void addKmlNode(osg::ref_ptr<ossimPlanetKmlLayerNode> kml);
   void removeKmlNodeFromLegend(osg::ref_ptr<ossimPlanetKmlLayerNode> node);
   void addVideoNode(osg::ref_ptr<ossimPlanetVideoLayerNode> node);
   void removeVideoNodeFromLegend(osg::ref_ptr<ossimPlanetVideoLayerNode> node);
   void keyPressEvent ( QKeyEvent * e);
   void mousePressEvent(QMouseEvent *e);
   void mouseMoveEvent(QMouseEvent *e);
   void mouseReleaseEvent(QMouseEvent * e);
   
   void moveTextureItemFromTo(ossimPlanetQtLegendTextureItem* itemBeingMoved,
                              ossimPlanetQtLegendTextureItem* destination);

   void moveSelectedTexturesTo(ossimPlanetQtLegendTextureItem* destination);
   ossimPlanetQtLegendNodeItem* getParentNodeItem(ossimPlanetNode* node, ossimPlanetQtLegendNodeItem* defaultValue);
   ossimPlanetQtMainWindow* theMainWindow;
   bool theMouseDragFlag;
   bool theMousePressFlag;
   QTreeWidgetItem* theItemBeingMoved;
   QPoint theLastPressPos;
   QMenu* theItemPopup;
   ossimString theVersion;
   ossimPlanetQtLegendNodeItem* theLocationItem;
   ossimPlanetQtLegendNodeItem* theVideoItem;
   QTreeWidgetItem* theAnimationPathRootItem;
   std::vector<QAction*> thePopupMenuActions;
	
	OpenThreads::ReentrantMutex  theNodeIndexMapMutex;
	NodeIndexMapType    theNodeIndexMap;
	OpenThreads::ReentrantMutex  theTextureLayerIndexMapMutex;
	TextureLayerIndexMapType    theTextureLayerIndexMap;
	
   OpenThreads::ReentrantMutex thePopulateLegendMutex;
protected slots:
   void doubleClick(QTreeWidgetItem * item, int  columnNumber); 
   void changed(QTreeWidgetItem * item, int  columnNumber);
   void expanded(QTreeWidgetItem * item);
   void collapsed(QTreeWidgetItem * item);
   void popupMenuTriggered(QAction *);
};
#endif
