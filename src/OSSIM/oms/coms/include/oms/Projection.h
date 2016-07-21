#ifndef omsProjection_HEADER
#define omsProjection_HEADER
#include <oms/Object.h>

class ossimProjection;
namespace oms
{
	class OMSDLL Projection : public Object
	{
	public:
		Projection(ossimProjection* nativePointer=0);
      Projection(const Projection& src);
		virtual ~Projection();
		virtual bool setNativePointer(ossimProjection* projection);
		virtual const ossimProjection* getNativePointer()const;
		virtual ossimProjection* getNativePointer();
		
	private:
		class PrivateData;
		PrivateData* thePrivateData;
	};
}
#endif
