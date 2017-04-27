#include <ossimPlanetQt/ossimPlanetQtLegendItem.h>
#include <ossimPlanetQt/ossimPlanetQtLegend.h>
ossimPlanetQtLegendItem::ossimPlanetQtLegendItem(QTreeWidgetItem* item,
                                                 const QString& name)
   :QTreeWidgetItem(item)
{
   setText(0, name);
}

ossimPlanetQtLegendItem::ossimPlanetQtLegendItem(QTreeWidget* tWidget,
                                                 const QString& s)
   :QTreeWidgetItem(tWidget)
{
   setText(0, s);
}
 
ossimPlanetQtLegendItem::ossimPlanetQtLegendItem()
   :QTreeWidgetItem()
{
}

const ossimPlanetQtLegend* ossimPlanetQtLegendItem::legend() const
{
   return dynamic_cast<const ossimPlanetQtLegend*>(treeWidget());
}

ossimPlanetQtLegend* ossimPlanetQtLegendItem::legend() 
{
   return dynamic_cast<ossimPlanetQtLegend*>(treeWidget());
}

void ossimPlanetQtLegendItem::changed()
{
}

void ossimPlanetQtLegendItem::recursiveAdd(QTreeWidgetItem* parent,
                                           const ossimRefPtr<ossimXmlNode> node)
{
   if(!node.valid())
   {
      return;
   }
   const std::vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   if(childNodes.size() == 0)
   {
      QTreeWidgetItem* item = new QTreeWidgetItem(parent);
      item->setText(0, node->getTag().c_str());
      item->setText(1, node->getText().c_str());
   }
   else
   {
      ossim_uint32 idx = 0;
      ossim_uint32 idxMax = 0;
      idxMax = childNodes.size();
      QTreeWidgetItem* item = new QTreeWidgetItem(parent);
      item->setText(0, node->getTag().c_str());
      item->setText(1, node->getText().c_str());
      for(idx = 0; idx < idxMax; ++idx)
      {         
         recursiveAdd(item, childNodes[idx]);
      }
   }
}
