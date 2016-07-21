#include <oms/Projection.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimRefPtr.h>

class oms::Projection::PrivateData
{
public:
	PrivateData(ossimProjection* nativePointer=0)
	:theProjection(nativePointer)
	{
	}
	
	ossimRefPtr<ossimProjection> theProjection;
};

oms::Projection::Projection(ossimProjection* nativePointer)
:thePrivateData(new PrivateData(nativePointer))
{
}

oms::Projection::Projection(const Projection& src)
:thePrivateData(new PrivateData(src.thePrivateData->theProjection.valid()?(ossimProjection*)src.thePrivateData->theProjection->dup():0))
{
}

oms::Projection::~Projection()
{
	if(thePrivateData)
	{
		delete thePrivateData;
		thePrivateData = 0;
	}
}

bool oms::Projection::setNativePointer(ossimProjection* projection)
{
	thePrivateData->theProjection = projection;
	return true;
}

const ossimProjection* oms::Projection::getNativePointer()const
{
	return thePrivateData->theProjection.get();
}

ossimProjection* oms::Projection::getNativePointer()
{
	return thePrivateData->theProjection.get();
}
