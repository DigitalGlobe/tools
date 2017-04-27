#ifndef ossimPlanetQtEvent_HEADER
#define ossimPlanetQtEvent_HEADER
#include <QtCore/QEvent>
#include <QtCore/QString>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossimPlanet/ossimPlanetNode.h>
#include <ossimPlanet/ossimPlanetKmlLayerNode.h>
#include <ossimPlanet/ossimPlanetVideoLayerNode.h>
#include <ossimPlanet/ossimPlanetTextureLayer.h>
#include <ossimPlanet/ossimPlanetTextureLayerGroup.h>
class ossimPlanetQtLegendItem;

namespace ossimPlanetQt
{
   enum ossimPlanetQtEventId
   {
      ANIMATION_EVENT_TYPE       = QEvent::User + 8000,
      WMS_SETTINGS_EVENT_TYPE    = ANIMATION_EVENT_TYPE + 1,
      LEGEND_XML_EVENT_TYPE      = WMS_SETTINGS_EVENT_TYPE + 1,
      KML_FILE_EVENT_TYPE        = LEGEND_XML_EVENT_TYPE + 1,
      NODE_CALLBACK_EVENT_TYPE   = KML_FILE_EVENT_TYPE + 1,
      NODE_PROPERTY_EVENT_TYPE        = NODE_CALLBACK_EVENT_TYPE + 1,
      IMAGE_LAYER_EVENT          = NODE_PROPERTY_EVENT_TYPE + 1,
      LEGEND_ITEM_PROPERTY_EVENT = IMAGE_LAYER_EVENT + 1,
		MESSAGE_EVENT_TYPE         = LEGEND_ITEM_PROPERTY_EVENT
   };
   
   class Event : public QEvent
   {
   public:
      Event(QEvent::Type type)
         :QEvent(type)
      {
      }
   };

   class AnimationPathEvent : public ossimPlanetQt::Event
   {
   public:
      AnimationPathEvent(const ossimString& animationPath="")
         :Event((QEvent::Type)ANIMATION_EVENT_TYPE)
         {
            if(!animationPath.empty())
            {
               theAnimationPathList.push_back(animationPath);
            }
         }
         ossim_uint32 animationPathCount()const
         {
            return theAnimationPathList.size();
         }
         void animationPath(ossimString& animationPath,
                            ossim_uint32 idx=0)const
         {
            animationPath = "";
            if(idx < theAnimationPathList.size())
            {
               animationPath = theAnimationPathList[idx];
            }
         }
         void setAnimationPaths(const std::vector<ossimString>& paths)
         {
            theAnimationPathList = paths;
         }
   protected:
         std::vector<ossimString> theAnimationPathList;
   };
   class LegendXmlEvent : public ossimPlanetQt::Event
   {
   public:
      LegendXmlEvent(ossimRefPtr<ossimXmlNode> xml)
         :Event((QEvent::Type)LEGEND_XML_EVENT_TYPE)
      {
         theXml = xml;
      }
         ossimRefPtr<ossimXmlNode> xmlNode()
         {
            return theXml;
         }
   protected:
      ossimRefPtr<ossimXmlNode> theXml;
      
   };
   class KmlFileEvent : public ossimPlanetQt::Event
   {
   public:
      KmlFileEvent(const ossimFilename& file)
         :Event((QEvent::Type)KML_FILE_EVENT_TYPE),
         theFilename(file)
      {
      }
      const ossimFilename& filename()
      {
         return theFilename;
      }
   protected:
         ossimFilename theFilename;      
   };
   class NodeCallbackEvent : public ossimPlanetQt::Event
	{
	public:
		enum NodeEventType
		{
			NodeEventType_ADD    = 1,
			NodeEventType_REMOVE,
			NodeEventType_DESTRUCTING,
		};
		NodeCallbackEvent(NodeEventType type)
		:Event((QEvent::Type)NODE_CALLBACK_EVENT_TYPE),
		theItem(0),
		theNode(0),
		theNodeEventType(type)
		{
		}
		NodeCallbackEvent(osg::ref_ptr<ossimPlanetNode> node,
								NodeEventType type)
		:Event((QEvent::Type)NODE_CALLBACK_EVENT_TYPE),
		theItem(0),
		theNode(node.get()),
		theNodeEventType(type)
		{
		}
		NodeCallbackEvent(ossimPlanetQtLegendItem* item,
								osg::ref_ptr<ossimPlanetNode> node,
								NodeEventType type)
		:Event((QEvent::Type)NODE_CALLBACK_EVENT_TYPE),
		theItem(item),
		theNode(node.get()),
		theNodeEventType(type)
		{
		}
		void setNodeEventType(NodeEventType t)
		{
			theNodeEventType = t;
		}
		NodeEventType nodeEventType()const
		{
			return theNodeEventType;
		}
		const ossimPlanetNode* node()const
		{
			return theNode.get();
		}
		ossimPlanetNode* node()
		{
			return theNode.get();
		}
		void setNode(ossimPlanetNode* n)
		{
			theNode = n;
		}
		void setItem(ossimPlanetQtLegendItem* value)
		{
			theItem = value;
		}
		ossimPlanetQtLegendItem* item()
		{
			return theItem;
		}
	protected:
		ossimPlanetQtLegendItem*      theItem;
		osg::ref_ptr<ossimPlanetNode> theNode;
		NodeEventType                 theNodeEventType;
	};
	
   class WmsSettingsEvent : public ossimPlanetQt::Event
   {
   public:
      class WmsSettingsProperties
      {
      public:
         WmsSettingsProperties(const ossimString& name,
                               const ossimString& url="",
                               const ossimString& cache="",
                               const ossimString& proxyHost="",
                               const ossimString& proxyPort="",
                               const ossimString& proxyUser="",
                               const ossimString& proxyPassword="")
            :theName(name),
            theUrl(url),
            theCache(cache),
            theProxyHost(proxyHost),
            theProxyPort(proxyPort),
            theProxyUser(proxyUser),
            theProxyPassword(proxyPassword)
            {
            }
         ossimString theName;
         ossimString theUrl;
         ossimString theCache;
         ossimString theProxyHost;
         ossimString theProxyPort;
         ossimString theProxyUser;
         ossimString theProxyPassword;
      };
      enum WmsSettiingsType
      {
         NONE = 0,
         ADD  = 1,
         DELETE = 2
      };
      WmsSettingsEvent(const WmsSettingsProperties& settings,
                       WmsSettiingsType settingsType=ADD)
         :Event((QEvent::Type)WMS_SETTINGS_EVENT_TYPE),
         theSettingsType(settingsType)
      {
         theSettingsPropertyList.push_back(settings);
      }
      
      WmsSettingsEvent(const std::vector<WmsSettingsProperties>& settings,
                       WmsSettiingsType settingsType=ADD)
         :Event((QEvent::Type)WMS_SETTINGS_EVENT_TYPE),
         theSettingsPropertyList(settings),
         theSettingsType(settingsType)
      {
         
      }
         const std::vector<WmsSettingsProperties>& settingsPropertyList()const
         {
            return theSettingsPropertyList;
         }
   protected:
      std::vector<WmsSettingsProperties> theSettingsPropertyList;
      WmsSettiingsType theSettingsType;
   };
   class NodePropertyEvent : public ossimPlanetQt::Event
   {
   public:
      NodePropertyEvent (ossimPlanetNode* node,
                    const ossimString& name)
      :Event((QEvent::Type)NODE_PROPERTY_EVENT_TYPE),
      theNode(node),
      theName(name)
      {
         
      }
      const ossimString& name()const
      {
         return theName;
      }
      void setName(const ossimString& name)
      {
         theName = name;
      }
      
		ossimPlanetNode* node()
		{
			return theNode.get();
		}
      const ossimPlanetNode* node()const
      {
         return theNode.get();
      }
      void setNode(ossimPlanetNode* node)
      {
         theNode = node;
      }
   protected:
      osg::ref_ptr<ossimPlanetNode> theNode;
      ossimString theName;
   };
   
   class ImageLayerEvent : public ossimPlanetQt::Event
   {
   public:
      enum ActionType
      {
         ADD = 0, // this is used to add a layer to the legend
			ADDED, // this says a layer was added.
         REMOVED // this says a layer was removed.
      };
      ImageLayerEvent(ossimPlanetTextureLayer* layer,
                      ActionType actionType=ImageLayerEvent::ADD)
      :Event((QEvent::Type)IMAGE_LAYER_EVENT),
      theLayer(layer),
      theActionType(actionType)
      {
			if(layer)
			{
				osg::ref_ptr<ossimPlanetTextureLayerGroup> group = layer->parent(0);
				if(group.valid())
				{
					theIndex = group->findLayerIndex(layer);
				}
			}
      }
      osg::ref_ptr<ossimPlanetTextureLayer> layer()
      {
         return theLayer;
      }
      const osg::ref_ptr<ossimPlanetTextureLayer> layer()const
      {
         return theLayer;
      }
      void setLayer(ossimPlanetTextureLayer* layer)
      {
         theLayer = layer;
      }
      ActionType actionType()const
      {
         return theActionType;
      }
      void setActionType(ActionType type)
      {
         theActionType = type;
      }
		ossim_int32 layerIndex()const
		{
			return theIndex;
		}
   protected:
      osg::ref_ptr<ossimPlanetTextureLayer> theLayer;
		ossim_int32 theIndex;
      ActionType theActionType;
   };

   class LegendItemProperty : public ossimPlanetQt::Event
   {
   public:
      LegendItemProperty(const ossimString& name,
                      ossimPlanetQtLegendItem* item)
      :Event((QEvent::Type)LEGEND_ITEM_PROPERTY_EVENT),
      theName(name),
      theItem(item)
      {
      }
      void setItem(ossimPlanetQtLegendItem* value)
      {
         theItem = value;
      }
      ossimPlanetQtLegendItem* item()
      {
         return theItem;
      }
      void setName(const ossimString& name)
      {
         theName = name;
      }
      const ossimString& name()const
      {
         return theName;
      }
   protected:
      ossimString theName;
      ossimPlanetQtLegendItem* theItem;
   };
   class MessageEvent : public ossimPlanetQt::Event
   {
   public:
		enum MessageType
		{
			ERROR_MESSAGE_TYPE = 0,
			WARNING_MESSAGE_TYPE,
			DEBUG_MESSAGE_TYPE,
			INFO_MESSAGE_TYPE
		};
      MessageEvent(const QString& msg,
		  MessageType msgType)
      :Event((QEvent::Type)MESSAGE_EVENT_TYPE),
		theMessage(msg),
		theMessageType(msgType)
		{
			
		}
		const QString& message()const
		{
			return theMessage;
		}
		void setMessage(const QString& msg)
		{
			theMessage = msg;
		}
		MessageType messageType()const
		{
			return theMessageType;
		}
		void setMessageType(MessageType msgType)
		{
			theMessageType = msgType;
		}
	protected:
		QString theMessage;
		MessageEvent::MessageType theMessageType;
	};
		
}    

#endif
