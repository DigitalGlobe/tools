#include "RasterEngine.h"
#include "DataManager.h"

#include <iostream>

RasterEngine::RasterEngine()
	: theDataManager( new DataManager() )
{
}

RasterEngine::~RasterEngine()
{
	if ( theDataManager )
	{
		delete theDataManager;
		theDataManager = NULL;
	}
}

void RasterEngine::addFile( const std::string& filename )
{
	std::cout << "Adding " << filename << std::endl;
}

void RasterEngine::getMap( const std::string& srs, double minX, double minY, double maxX, double maxY, int width, int height, ossim_int8* buffer  )
{
	std::cout 
		<< "srs=" << srs << ", " 
		<< "bbox=(" << minX << ", " << minY << ", " << maxX << ", " << maxY << "), " 
		<< "size=(" << width << ", " << height << ")" 
		<< std::endl;
}


