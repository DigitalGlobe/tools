#include <oms/OutputSource.h>
#include <ossim/base/ossimOutputSource.h>

oms::OutputSource::OutputSource(ossimOutputSource* nativePointer)
:Source(nativePointer)
{
}

oms::OutputSource::~OutputSource()
{
}

bool oms::OutputSource::setNativePointer(ossimSource* nativePointer)
{
	if(dynamic_cast<ossimOutputSource*>(nativePointer))
	{
		return Source::setNativePointer(nativePointer);
	}
	
	return false;
}
