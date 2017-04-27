#ifndef omsSource_HEADER
#define omsSource_HEADER 1
#include <oms/Object.h>
#include <string>

class ossimSource;

namespace oms
{
	class OMSDLL Source : public Object
	{
	public:
		Source(ossimSource* nativePointer);
		virtual ~Source();
		virtual int addInput(Source* input);
		virtual void clearInputs();
		virtual void setProperty(const std::string& name,
										 const std::string& value);
		
		virtual ossimSource* getNativePointer();
		virtual const ossimSource* getNativePointer()const;
		virtual bool setNativePointer(ossimSource* src);
		
	private:
		class PrivateData;
		PrivateData* thePrivateData;
	};
}
#endif
