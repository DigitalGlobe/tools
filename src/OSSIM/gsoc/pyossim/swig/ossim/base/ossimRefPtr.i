/*
*/

%module pyossim


#ifndef ossimRefPtr_HEADER
#define ossimRefPtr_HEADER
#include <ossim/base/ossimConstants.h>


template<class T> class ossimRefPtr
{
 public:
   typedef T element_type;
   
   ossimRefPtr() :m_ptr(0) {}
   ossimRefPtr(T* t):m_ptr(t)              { if (m_ptr) m_ptr->ref(); }
   ossimRefPtr(const ossimRefPtr& rp):m_ptr(rp.m_ptr)  { if (m_ptr) m_ptr->ref(); }
   ~ossimRefPtr()                           { if (m_ptr) m_ptr->unref(); m_ptr=0; }


   
   inline T& operator*()  { return *m_ptr; }
   
   inline const T& operator*() const { return *m_ptr; }
   
   inline T* operator->() { return m_ptr; }
   
   inline const T* operator->() const   { return m_ptr; }
   
   inline bool valid() const	{ return m_ptr!=0L; }
   
   inline T* get() { return m_ptr; }
   
   inline const T* get() const { return m_ptr; }
   

   inline T* take() { return release();}
   
   inline T* release() { T* tmp=m_ptr; if (m_ptr) m_ptr->unref_nodelete(); m_ptr=0; return tmp;}
   
 private:
   T* m_ptr;
};
#endif



