/*
 * @Contents SUNRFUNC.H includes definitions commonly used in MUSE
 * applications.
 */

#ifndef H_SUNR_FUNC

/*
 * @#Defines Press the Next button to view. H_SUNR_FUNC H_SUNR_FUNC is
 * defined to indicate that the SUNRFUNC.H header has been included.
 */

#define H_SUNR_FUNC


/*
 * @#Includes
 */

#ifndef H_MUSE
#include "muse.h"
#endif

/*
 * @Functions
 */

/*
 * @	raster_to_sunraster
 */

#if XVT_CC_PROTO
ERRSTATUS       raster_to_sunraster(RASTER * raster, FILE_SPEC file_spec);
#else
ERRSTATUS       raster_to_sunraster();
#endif

/*
 * @	lan_to_raster
 */

#if 0

#if XVT_CC_PROTO
ERRSTATUS       sunraster_to_raster(FILE_SPEC file_spec, RASTER * raster);
#else
ERRSTATUS       sunraster_to_raster();
#endif

#endif

#endif				/* end H_SUNR_FUNC */
