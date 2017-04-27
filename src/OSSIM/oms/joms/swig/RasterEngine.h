#ifndef RasterEngine_HEADER
#define RasterEngine_HEADER

#include <oms/Constants.h>

#include <string>

class DataManager;

class OMSDLL RasterEngine
{
public:
	RasterEngine();
	~RasterEngine();

	void addFile( const std::string& filename );
	void getMap( const std::string& srs, double minX, double minY, double maxX, double maxY, int width, int height, ossim_int8* buffer );
	

protected:
	DataManager* theDataManager;
};

#endif
