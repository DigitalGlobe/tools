#include <oms/Source.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimSource.h>

class oms::Source::PrivateData
{
public:
	PrivateData(ossimSource* source)
	:theSource(source)
	{
	}

	ossimRefPtr<ossimSource> theSource;
};

oms::Source::Source(ossimSource* nativePointer)
:thePrivateData(new PrivateData(nativePointer))
{
}

oms::Source::~Source()
{
	if(thePrivateData)
	{
		delete thePrivateData;
		thePrivateData = 0;
	}
}

int oms::Source::addInput(Source* inputSource)
{
	ossim_int32 connectionIdx = -1;
	if(inputSource&&inputSource->getNativePointer()&&getNativePointer())
	{
		connectionIdx = getNativePointer()->connectMyInputTo(inputSource->getNativePointer());
	}
	
	return (connectionIdx >= 0);
}

bool oms::Source::setNativePointer(ossimSource* source)
{
	thePrivateData->theSource = source;
	return true;
}

ossimSource* oms::Source::getNativePointer()
{
	return thePrivateData->theSource.get();
}

const ossimSource* oms::Source::getNativePointer()const
{
	return thePrivateData->theSource.get();
}

void oms::Source::clearInputs()
{
	thePrivateData->theSource->disconnectAllInputs();
}

void oms::Source::setProperty(const std::string& name,
										const std::string& value)
{
	ossimPropertyInterface* prop = thePrivateData->theSource.get();
	if(prop)
	{
		prop->setProperty(name, value);
	}
}