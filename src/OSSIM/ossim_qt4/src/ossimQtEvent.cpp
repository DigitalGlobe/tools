//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//----------------------------------------------------------------------------
// $Id: ossimQtEvent.cpp 19064 2011-03-12 16:01:23Z dburken $

#include <ossimQt/ossimQtEvent.h>

ossimQtCustomEvent::ossimQtCustomEvent(
   ossimQtCustomEvent::ossimQtCustomEventMode mode,
   ossimQtEventIdType type,
   QObject* eventObject)
   :QEvent((QEvent::Type)type),
    theMode(mode),
    theEventObject(eventObject)
{
}

bool ossimQtCustomEvent::isPropagating()const
{
   return (theMode == ossimQtCustomEvent_PROPAGATE);
}

void ossimQtCustomEvent::setPropagating()
{
   theMode = ossimQtCustomEvent_PROPAGATE;
}

void ossimQtCustomEvent::setNoPropagating()
{
   theMode =  ossimQtCustomEvent_NOPROPAGATE;
}

void ossimQtCustomEvent::setMode(
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
{
   theMode = mode;
}

ossimQtCustomEvent::ossimQtCustomEventMode ossimQtCustomEvent::getMode()const
{
   return theMode;
}

const QObject* ossimQtCustomEvent::getEventObject()const
{
   return theEventObject;
}

void ossimQtCustomEvent::setEventObject(QObject* obj)
{
   theEventObject = obj;
}

ossimQtRefreshDisplayEvent::ossimQtRefreshDisplayEvent(
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_REFRESH_DISPLAY_ID)
{}

ossimQtFlushDisplayEvent::ossimQtFlushDisplayEvent(
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_FLUSH_DISPLAY_ID)
{}

ossimQtDisplayListEvent::ossimQtDisplayListEvent(
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode, OSSIM_QT_EVENT_DISPLAY_LIST_ID)
{
   theDisplayListIterator = theDisplayList.end();
}

void ossimQtDisplayListEvent::addDisplay(QObject* display)
{
   if(display)
   {
      if(theDisplayList.find(display) == theDisplayList.end())
      {
         theDisplayList.insert(std::make_pair(display, display));
      }
   }
}

void ossimQtDisplayListEvent::eraseDisplay(QObject* display)
{
   std::map<QObject*, QObject*>::iterator iter = theDisplayList.find(display);

   if(iter != theDisplayList.end())
   {
      theDisplayList.erase(iter);
   }
}

QObject* ossimQtDisplayListEvent::firstDisplay()
{
   theDisplayListIterator = theDisplayList.begin();

   if(theDisplayList.size() > 0)
   {
      return (*theDisplayListIterator).first;
   }
   return 0;
}

QObject* ossimQtDisplayListEvent::nextDisplay()
{
   ++theDisplayListIterator;
   if(theDisplayListIterator != theDisplayList.end())
   {
      return (*theDisplayListIterator).first;
   }
   
   return 0;
}

ossimQtDisplayGeoViewEvent::ossimQtDisplayGeoViewEvent(
   const ossimObject* view,
   const ossimGpt& center,
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_DISPLAY_GEO_VIEW_ID)
{
   if(view)
   {
      theView = view->dup();
   }
   theViewCenter = center;
}

ossimQtDisplayGeoViewEvent::~ossimQtDisplayGeoViewEvent()
{
}

void ossimQtDisplayGeoViewEvent::setView(const ossimObject* view)
{
   theView = 0;
   if(view)
      theView = view->dup();
}

void ossimQtDisplayGeoViewEvent::setCenter(const ossimGpt& center)
{
   theViewCenter = center;
}

const ossimObject* ossimQtDisplayGeoViewEvent::getView()const
{
   return theView.get();
}

const ossimGpt& ossimQtDisplayGeoViewEvent::getViewCenter()const
{
   return theViewCenter;
}

ossimQtMouseEvent::ossimQtMouseEvent(
   const ossimIpt& shiftToZeroZero,
   QMouseEvent* evt,
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_MOUSE_ID),
    theEvent(evt), 
    theShiftToZeroZero(shiftToZeroZero),
    thePoint(evt->x(), evt->y()),
    theButton(evt->button()),
    theState(evt->state()),
    theStateAfter(evt->stateAfter()),
    theQtEventType(evt->type())
{
}

QMouseEvent* ossimQtMouseEvent::getEvent() const
{
   return theEvent;
}

void ossimQtMouseEvent::setButton(Qt::ButtonState button)
{
   theButton = button;
}

ossimIpt ossimQtMouseEvent::getShiftToZeroZero()const
{
   return theShiftToZeroZero;
}

ossimIpt ossimQtMouseEvent::getPoint()const
{
   return thePoint;
}

ossimIpt ossimQtMouseEvent::getShiftPoint()const
{
   return thePoint - theShiftToZeroZero;
}

Qt::ButtonState ossimQtMouseEvent::getButton()const
{
   return theButton;
}

Qt::ButtonState ossimQtMouseEvent::getState()const
{
   return theState;
}

Qt::ButtonState ossimQtMouseEvent::getStateAfter()const
{
   return theStateAfter;
}

QEvent::Type ossimQtMouseEvent::getQtEventType()const
{
   return theQtEventType;
}

void ossimQtMouseEvent::setState(Qt::ButtonState state)
{
   theState = state;
}

void ossimQtMouseEvent::setStateAfter(Qt::ButtonState stateAfter)
{
   theStateAfter = stateAfter;
}

void ossimQtMouseEvent::setQtEventType(QEvent::Type type)
{
   theQtEventType = type;
}

ossimQtDisplayTrackPositionEvent::ossimQtDisplayTrackPositionEvent(
   const ossimGpt& gpt,
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_TRACK_POSITION_ID),
    theGround(gpt)
{
}

ossimQtDisplayTrackPositionEvent::ossimQtDisplayTrackPositionEvent()
   :ossimQtCustomEvent(ossimQtCustomEvent_NOPROPAGATE,
                       OSSIM_QT_EVENT_TRACK_POSITION_ID)
{
   theGround.makeNan();
}

void ossimQtDisplayTrackPositionEvent::setGround(const ossimGpt& gpt)
{
   theGround = gpt;
}

const ossimGpt& ossimQtDisplayTrackPositionEvent::getGround()const
{
   return theGround;
}

void ossimQtDisplayTrackPositionEvent::setButton(Qt::ButtonState button)
{
   theButton = button;
}

Qt::ButtonState ossimQtDisplayTrackPositionEvent::getButton()const
{
   return theButton;
}

Qt::ButtonState ossimQtDisplayTrackPositionEvent::getState()const
{
   return theState;
}

Qt::ButtonState ossimQtDisplayTrackPositionEvent::getStateAfter()const
{
   return theStateAfter;
}

QEvent::Type ossimQtDisplayTrackPositionEvent::getQtEventType()const
{
   return theQtEventType;
}

void ossimQtDisplayTrackPositionEvent::setState(Qt::ButtonState state)
{
   theState = state;
}

void ossimQtDisplayTrackPositionEvent::setStateAfter(Qt::ButtonState stateAfter)
{
   theStateAfter = stateAfter;
}

void ossimQtDisplayTrackPositionEvent::setQtEventType(QEvent::Type type)
{
   theQtEventType = type;
}

ossimQtDisplayChainEvent::ossimQtDisplayChainEvent(
   ossim_int32 id,
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode, OSSIM_QT_EVENT_DISPLAY_CHAIN_ID),
    theId(id)
{}

ossim_int32 ossimQtDisplayChainEvent::getId()const
{
   return theId;
}

void ossimQtDisplayChainEvent::setId(ossim_int32 id)
{
   theId = id;
}

ossimQtResamplerEvent::ossimQtResamplerEvent(
   const ossimString& minify,
   const ossimString& magnify,
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_RESAMPLER_ID),
    theMinifyType(minify),
    theMagnifyType(magnify)
{
}

const ossimString& ossimQtResamplerEvent::getMinify()const
{
   return theMinifyType;
}

void ossimQtResamplerEvent::setMinify(const ossimString& type)
{
   theMinifyType = type;
}

const ossimString& ossimQtResamplerEvent::getMagnify()const
{
   return theMagnifyType;
}

void ossimQtResamplerEvent::setMagnify(const ossimString& type)
{
   theMagnifyType = type;
}

ossimQtGetDataManagerEvent::ossimQtGetDataManagerEvent(
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_GET_DATA_MANAGER),
    theDataManager(0)
{
}

void ossimQtGetDataManagerEvent::setDataManager(ossimDataManager* manager)
{
   theDataManager = manager;
}

ossimDataManager* ossimQtGetDataManagerEvent::getDataManager()
{
   return theDataManager;
}

ossimQtGetProjectEvent::ossimQtGetProjectEvent(
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_GET_PROJECT),
    theProject(0)
{
}

void ossimQtGetProjectEvent::setProject(ossimQtProject* project)
{
   theProject = project;
}

ossimQtProject* ossimQtGetProjectEvent::getProject()
{
   return theProject;
}

ossimQtStateEvent::ossimQtStateEvent(
   ossimQtStateEventType type,
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_STATE_EVENT),
    theStateType(type)
{
}

ossimString ossimQtStateEvent::getPrefix()const
{
   return thePrefix;
}

void ossimQtStateEvent::setPrefix(const ossimString& prefix)
{
   thePrefix = prefix;
}

ossimKeywordlist& ossimQtStateEvent::getKeywordlist()
{
   return theKwl;
}

void ossimQtStateEvent::setStateType(ossimQtStateEventType type)
{
   theStateType = type;
}

ossimQtStateEvent::ossimQtStateEventType ossimQtStateEvent::getStateType()const
{
   return theStateType;
}

void ossimQtStateEvent::setKeywordlist(const ossimKeywordlist& kwl)
{
   theKwl = kwl;
}

ossimQtQueryObjectAllocationEvent::ossimQtQueryObjectAllocationEvent(
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode,
                       OSSIM_QT_EVENT_QUERY_OBJECT_ALLOCATED),
    theObjectsAllocatedFlag(false)
{}
   
void ossimQtQueryObjectAllocationEvent::setObjectsAllocatedFlag(bool flag)
{
   theObjectsAllocatedFlag = flag;
}

bool ossimQtQueryObjectAllocationEvent::getObjectsAllocated()const
{
   return theObjectsAllocatedFlag;
}

void ossimQtQueryObjectAllocationEvent::setQueryList(
   std::vector<ossimString>& queryList)
{
   theClassNameList = queryList;
   std::sort(theClassNameList.begin(),
             theClassNameList.end());
}

void ossimQtQueryObjectAllocationEvent::getQueryList(
   std::vector<ossimString>& queryList)
{
   queryList = theClassNameList;
}

bool ossimQtQueryObjectAllocationEvent::find(const ossimString& className)const
{
   return std::binary_search(theClassNameList.begin(),
                             theClassNameList.end(),
                             className);
}

ossimQtAddImageFileEvent::ossimQtAddImageFileEvent(
   const ossimFilename& filename,
   ossimQtCustomEvent::ossimQtCustomEventMode mode)
   :ossimQtCustomEvent(mode, OSSIM_QT_EVENT_ADD_IMAGE_FILE),
    theFilename(filename)
{}

const ossimFilename& ossimQtAddImageFileEvent::getFilename()const
{
   return theFilename;
}

void ossimQtAddImageFileEvent::setFilename(const ossimFilename& filename)
{
   theFilename = filename;
}


