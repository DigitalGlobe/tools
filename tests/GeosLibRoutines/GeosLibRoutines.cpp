#include "GeosLibRoutines.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>

#include "C:\DigitalGlobeTemp\tools\src\GEOS\capi\geos_c.h"

#define MAXWKTLEN 1047551


using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::operation::polygonize;
using namespace geos::operation::linemerge;
using geos::util::GEOSException;
using geos::util::IllegalArgumentException;


void notice(const char *fmt, ...) {
	va_list ap;

	fprintf(stdout, "NOTICE: ");

	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
	fprintf(stdout, "\n");
}

void log_and_exit(const char *fmt, ...) {
	va_list ap;

	fprintf(stdout, "ERROR: ");

	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
	fprintf(stdout, "\n");
	exit(1);
}


int GeosLibRoutines::createPoint(double x, double y)
{
	GEOSCoordSequence* cs;
	static char wkt[1024];
	char tempX[10], tempY[10];

	initGEOS(notice, log_and_exit);

	double a, b;

	_ltoa(x, tempX, 10);
	_ltoa(y, tempY, 10);
	
	strcpy(wkt, "POINT(");
	strcat(wkt, tempX);
	strcat(wkt, " ");
	strcat(wkt, tempY);
	strcat(wkt, ")");

	std::cout << wkt << std::endl;
	
	GEOSGeometry* g1 = GEOSGeomFromWKT(wkt);
	GEOSGeometry* g2;

	cs = GEOSCoordSeq_clone(GEOSGeom_getCoordSeq(g1));
	g2 = GEOSGeom_createPoint(cs);

	int xVal = GEOSGeomGetX(g2, &a);
	int yVal = GEOSGeomGetX(g2, &b);

	std::cout << "Point coordinates are: xVal = " << xVal << " and yVal = " << yVal << std::endl;
	
	finishGEOS();
	
	return 0;
}

int GeosLibRoutines::createRectangle(double lX, double lY, double width, double height)
{
	static char wkt[1024];
	char tempX[10], tempY[10], tempWidth[10], tempHeight[10];

	initGEOS(notice, log_and_exit);

	_ltoa(lX, tempX, 10);
	_ltoa(lY, tempY, 10);
	_ltoa(width, tempWidth, 10);
	_ltoa(height, tempHeight, 10);

	//create the data string
	strcpy(wkt, "POLYGON((");
	strcat(wkt, tempX);
	strcat(wkt, " ");
	strcat(wkt, tempHeight);
	strcat(wkt, ", ");
	strcat(wkt, tempY);
	strcat(wkt, " ");
	strcat(wkt, tempHeight);
	strcat(wkt, ", ");
	strcat(wkt, tempY);
	strcat(wkt, " ");
	strcat(wkt, tempWidth);
	strcat(wkt, ", ");
	strcat(wkt, tempX);
	strcat(wkt, " ");
	strcat(wkt, tempHeight);
	strcat(wkt, "))");

	std::cout << wkt << std::endl;

	GEOSGeometry *inputGeom = GEOSGeomFromWKT(wkt);

	const GEOSGeometry *linearRing;
	const GEOSCoordSequence *coordSeq;

	int numGeom = GEOSGetNumGeometries(inputGeom);

	int n;
	for (n = 0; n < numGeom; n++) {
		linearRing = GEOSGetExteriorRing(GEOSGetGeometryN(inputGeom, n));

		unsigned int numPoints, p;
		numPoints = GEOSGeomGetNumPoints(linearRing);

		for (p = 0; p < numPoints; p++) {
			double xCoord, yCoord;
			coordSeq = GEOSGeom_getCoordSeq(linearRing);
			GEOSCoordSeq_getX(coordSeq, p, &xCoord);
			GEOSCoordSeq_getY(coordSeq, p, &yCoord);

			std::cout << "Point coordinates are: xVal = " << xCoord << " and yVal = " << yCoord << std::endl;
		}
	}

	finishGEOS();

	return 0;
}