#ifndef omsOutputSource_HEADER
#define omsOutputSource_HEADER 1
#include <oms/Object.h>
#include <oms/Source.h>
class ossimOutputSource;
namespace oms
{
	class OMSDLL OutputSource : public Source
	{
	public:
		OutputSource(ossimOutputSource* nativePointer=0);
		virtual ~OutputSource();
		virtual bool setNativePointer(ossimSource* nativePointer);
		
	private:
		class PrivateData;
		PrivateData* thePrivateData;
	};
}

#endif
