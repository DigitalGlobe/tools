//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//----------------------------------------------------------------------------
// $Id: ossimQtEvent.h 2259 2011-03-11 17:24:01Z david.burken $
#ifndef ossimQtEvent_HEADER
#define ossimQtEvent_HEADER
#include <algorithm>
#include <QtCore/QEvent>
// #include <QtCore/QCustomEvent>
#include <QtGui/QMouseEvent>
#include <map>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include "ossimQtExport.h"

class ossimDataManager;
class ossimQtProject;

enum ossimQtEventIdType
{
   OSSIM_QT_EVENT_REFRESH_DISPLAY_ID = QEvent::User,
   OSSIM_QT_EVENT_FLUSH_DISPLAY_ID,
   OSSIM_QT_EVENT_DISPLAY_LIST_ID,
   OSSIM_QT_EVENT_DISPLAY_GEO_VIEW_ID,
   OSSIM_QT_EVENT_MOUSE_ID,
   OSSIM_QT_EVENT_TRACK_POSITION_ID,
   OSSIM_QT_EVENT_DISPLAY_CHAIN_ID,
   OSSIM_QT_EVENT_RESAMPLER_ID,
   OSSIM_QT_EVENT_GET_DATA_MANAGER,
   OSSIM_QT_EVENT_GET_PROJECT,
   OSSIM_QT_EVENT_STATE_EVENT,
   OSSIM_QT_EVENT_QUERY_OBJECT_ALLOCATED,
   OSSIM_QT_EVENT_ADD_IMAGE_FILE
};

class ossimQtCustomEvent : public QEvent
{
public:
  enum ossimQtCustomEventMode
    {
      ossimQtCustomEvent_NOPROPAGATE = 0,
      ossimQtCustomEvent_PROPAGATE
    };
  ossimQtCustomEvent(ossimQtCustomEvent::ossimQtCustomEventMode mode,
		     ossimQtEventIdType type,
		     QObject* eventObject=0);

  virtual bool isPropagating()const;

  virtual void setPropagating();

  virtual void setNoPropagating();

  virtual void setMode(ossimQtCustomEvent::ossimQtCustomEventMode mode);

  ossimQtCustomEvent::ossimQtCustomEventMode getMode()const;

  const QObject* getEventObject()const;

  void setEventObject(QObject* obj);

public:
  ossimQtCustomEvent::ossimQtCustomEventMode theMode;
  QObject*               theEventObject;
};

class ossimQtRefreshDisplayEvent : public ossimQtCustomEvent
{
public:
   ossimQtRefreshDisplayEvent(ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);
};

class ossimQtFlushDisplayEvent : public ossimQtCustomEvent
{
public:
   ossimQtFlushDisplayEvent(ossimQtCustomEvent::ossimQtCustomEventMode mode=ossimQtCustomEvent_NOPROPAGATE);
};

class ossimQtDisplayListEvent : public ossimQtCustomEvent
{
public:
   ossimQtDisplayListEvent(ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);

   void eraseDisplay(QObject* display);
   void addDisplay(QObject* display);
   QObject* firstDisplay();
   QObject* nextDisplay();
   
protected:
   std::map<QObject*, QObject*> theDisplayList;
   std::map<QObject*, QObject*>::iterator theDisplayListIterator;
};

class ossimQtDisplayGeoViewEvent : public ossimQtCustomEvent
{
public:
   ossimQtDisplayGeoViewEvent(const ossimObject* view=0,
			      const ossimGpt& center = ossimGpt(),
			      ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);

   virtual ~ossimQtDisplayGeoViewEvent();

   void setView(const ossimObject* view);

   void setCenter(const ossimGpt& center);

   const ossimObject* getView()const;

   const ossimGpt& getViewCenter()const;
   
protected:
   ossimRefPtr<ossimObject> theView;
   ossimGpt     theViewCenter;
};

class OSSIMQT_DLL ossimQtMouseEvent:public ossimQtCustomEvent
{
public:
   ossimQtMouseEvent(const ossimIpt& shiftToZeroZero,
                     QMouseEvent* evt,
                     ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);
   
   QMouseEvent* getEvent() const;

   void setButton(Qt::ButtonState button);

   ossimIpt getShiftToZeroZero()const;

   ossimIpt getPoint()const;

   ossimIpt getShiftPoint()const;

   Qt::ButtonState getButton()const;

   Qt::ButtonState getState()const;

   Qt::ButtonState getStateAfter()const;

   QEvent::Type getQtEventType()const;

   void setState(Qt::ButtonState state);

   void setStateAfter(Qt::ButtonState stateAfter);

   void setQtEventType(QEvent::Type type);
  
protected:
  QMouseEvent* theEvent;
  ossimIpt     theShiftToZeroZero;
  ossimIpt     thePoint;
  Qt::ButtonState  theButton;
  Qt::ButtonState  theState;
  Qt::ButtonState  theStateAfter;
  QEvent::Type theQtEventType;
};

class ossimQtDisplayTrackPositionEvent : public ossimQtCustomEvent
{
public:
  ossimQtDisplayTrackPositionEvent(const ossimGpt& gpt,
				   ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);

  ossimQtDisplayTrackPositionEvent();
  
  void setGround(const ossimGpt& gpt);
  
  const ossimGpt& getGround()const;

  void setButton(Qt::ButtonState button);

  Qt::ButtonState getButton()const;

  Qt::ButtonState getState()const;

  Qt::ButtonState getStateAfter()const;

  QEvent::Type getQtEventType()const;

  void setState(Qt::ButtonState state);

  void setStateAfter(Qt::ButtonState stateAfter);

  void setQtEventType(QEvent::Type type);

protected:
  ossimGpt     theGround;
  Qt::ButtonState  theButton;
  Qt::ButtonState  theState;
  Qt::ButtonState  theStateAfter;
  QEvent::Type theQtEventType;
};

class ossimQtDisplayChainEvent : public ossimQtCustomEvent
{
public:
  ossimQtDisplayChainEvent(ossim_int32 id,
			   ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);

  ossim_int32 getId()const;

  void setId(ossim_int32 id);

protected:
  ossim_int32 theId;
};

class ossimQtResamplerEvent : public ossimQtCustomEvent
{
public:
   ossimQtResamplerEvent(const ossimString& minify,
			 const ossimString& magnify,
			 ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);
   
   const ossimString& getMinify()const;

   void setMinify(const ossimString& type);
   
   const ossimString& getMagnify()const;

   void setMagnify(const ossimString& type);
   
protected:
   ossimString theMinifyType;
   ossimString theMagnifyType;
};

class ossimQtGetDataManagerEvent : public ossimQtCustomEvent
{
public:
   ossimQtGetDataManagerEvent(ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);

   void setDataManager(ossimDataManager* manager);

   ossimDataManager* getDataManager();

protected:
   ossimDataManager* theDataManager;
};

class ossimQtGetProjectEvent : public ossimQtCustomEvent
{
public:
   ossimQtGetProjectEvent(ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);

   void setProject(ossimQtProject* project);

   ossimQtProject* getProject();

protected:
   ossimQtProject* theProject;
};

class ossimQtStateEvent : public ossimQtCustomEvent
{
public:
   enum ossimQtStateEventType
   {
      ossimQtStateEventType_LOAD = 0,
      ossimQtStateEventType_SAVE
   };
   ossimQtStateEvent(ossimQtStateEventType type=ossimQtStateEventType_SAVE,
                     ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_PROPAGATE);

   ossimString getPrefix()const;

   void setPrefix(const ossimString& prefix);

   ossimKeywordlist& getKeywordlist();

   void setStateType(ossimQtStateEventType type);

   ossimQtStateEventType getStateType()const;

   void setKeywordlist(const ossimKeywordlist& kwl);

protected:
   ossimKeywordlist      theKwl;
   ossimString           thePrefix;
   ossimQtStateEventType theStateType;
};

class ossimQtQueryObjectAllocationEvent : public ossimQtCustomEvent
{
public:
   ossimQtQueryObjectAllocationEvent(ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);
   
   void setObjectsAllocatedFlag(bool flag = true);

   bool getObjectsAllocated()const;

   void setQueryList(std::vector<ossimString>& queryList);

   void getQueryList(std::vector<ossimString>& queryList);

   bool find(const ossimString& className)const;

protected:
   bool theObjectsAllocatedFlag;
   
   std::vector<ossimString> theClassNameList;
};

class ossimQtAddImageFileEvent : public ossimQtCustomEvent
{
public:
  ossimQtAddImageFileEvent(const ossimFilename& filename,
			   ossimQtCustomEvent::ossimQtCustomEventMode mode = ossimQtCustomEvent_NOPROPAGATE);

  const ossimFilename& getFilename()const;

  void setFilename(const ossimFilename& filename);

protected:
  ossimFilename theFilename;
};

#endif
