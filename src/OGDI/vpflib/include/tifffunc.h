/*
@Contents
TIFFFUNC.H includes definitions commonly
used in MUSE applications.
*/

#ifndef H_TIFFFUNC

/*
@#Includes
*/
#include<math.h>
#include<stdlib.h>
#ifndef H_MUSE
#include "muse.h"
#endif

#ifndef _TIFFIO_
#include "tiffio.h"
#endif

/*
@#Defines
Press the Next button to view.
	H_TIFFFUNC
H_TIFFFUNC is defined to indicate that
the TIFFFUNC.H header has been included.
*/
#define H_TIFFFUNC

/*
@Enumerated Data Types
Press the Next button to view.
@
*/

/*
@Simple Types
Push the Next button to view.
@
*/

/*
@Complex Data Types
Press the Next button to view.
*/
/*
@Functions
Press the Next button to view.
@	raster_to_tiff
*/

#if XVT_CC_PROTO
ERRSTATUS raster_to_tiff(RASTER *raster, FILE_SPEC file_spec);
#else
ERRSTATUS raster_to_tiff();
#endif

#endif /* end h_TIFFFUNC */


