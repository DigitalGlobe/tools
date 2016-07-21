/*-----------------------------------------------------------------------------
 * Filename        : ossimConnectableObject.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimConnectableObject
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimId.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimListenerManager.h>
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/base/ossimRefPtr.h>
%}
%include "std_vector.i"
%template(ossimConnectableObjectList) std::vector<ossimRefPtr<ossimConnectableObject> >;
%template(ossimConnectableObjectPtr) ossimRefPtr<ossimConnectableObject>;
%rename(const_ossimConnectableObject_getInput) ossimConnectableObject::getInput() const;
%rename(const_const_ossimConnectableObject_getInput) ossimConnectableObject::getInput(ossim_uint32) const;
%rename(const_ossimConnectableObject_getOutput) ossimConnectableObject::getOutput() const;
%rename(const_const_ossimConnectableObject_getOutput) ossimConnectableObject::getOutput(ossim_uint32)const;
%rename(const_ossimConnectableObject_getInputList) ossimConnectableObject::getInputList()const;
%rename(const_ossimConnectableObject_getOutputList) ossimConnectableObject::getOutputList()const;

/* Wrapping class ossimConnectableObject */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimPropertyInterface.h"

%rename(const_ossimConnectableObject_canConnectMyInputTo) ossimConnectableObject::canConnectMyInputTo(ossim_int32,ossimConnectableObject const *)const;

%rename(__set__) ossimConnectableObject::operator=;

%include "ossim/base/ossimConnectableObject.h"
