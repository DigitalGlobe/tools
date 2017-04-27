#ifndef H_MGM_FUNC
#define H_MGM_FUNC

#include "math.h"
/* JLL#include "tx.h" */

#ifndef H_ARC
/* JLL#include "arc.h" */
#endif

/*
 * The only reason for including XVT.H is to define XVT_CC_PROTO properly for
 * the compiler in use.  XVT_CC_PROTO should be defined if the compiler uses
 * ANSII prototypes.
 */

#ifndef INCL_XVTH
#include "xvt.h"
#endif
#ifndef H_UNIT_FUNC
#include "unit_f.h"
#endif


#ifndef ERRSTATUS
#define ERRSTATUS short
#endif

/***************************************************************
@    gptlam()
****************************************************************
GP to Lambert Conformal Conic map projection 2 standard parallels
*/

#if XVT_CC_PROTO
void 
gptlam(
       double a,
       double recf,
       double ophi,
       double olam,
       double fn,
       double fe,
       double ok,
       double sphi,
       double slam,
       double *y,
       double *x);
#else
void            gptlam();
#endif

/*
 * Description:
 * 
 */



/***************************************************************
@    gptotm()
****************************************************************
GP to transverse Mercator map projection
*/

#if XVT_CC_PROTO
void 
gptotm(
       double a,
       double recf,
       double ophi,
       double olam,
       double fn,
       double fe,
       double ok,
       double sphi,
       double slam,
       double *y,
       double *x);
#else
void            gptotm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    gptups()
****************************************************************
GP to UPS map projection
*/

#if XVT_CC_PROTO
void 
gptups(
       double a,
       double recf,
       double sphi,
       double slam,
       double *y,
       double *x);
#else
void            gptups();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    lamb()
****************************************************************
Lambert Conformal Conic map projection 2 standard parallels
*/

#if XVT_CC_PROTO
void 
lamb(
     double *p,
     double *l,
     int32 dir,
     double *a,
     double f,
     double op,
     double ol,
     double *fn,
     double *fe,
     double *ok,
     double *y,
     double *x,
     int32 *ierr);
#else
void            lamb();
#endif

/*
 * Description:
 * 
 */


/***************************************************************
@    merc()
****************************************************************
Mercator map projection
*/

#if XVT_CC_PROTO
void 
merc(
     double *p,
     double *l,
     int32 dir,
     double *a,
     double f,
     double op,
     double ol,
     double *fn,
     double *fe,
     double *ok,
     double *y,
     double *x,
     int32 *ierr);
#else
void            merc();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    mertgp()
****************************************************************
Mercator map projection to geog posn
*

#if XVT_CC_PROTO
void 
mertgp(
       double a,
       double recf,
       double ophi,
       double olam,
       double fn,
       double fe,
       double ok,
       double *sphi,
       double *slam,
       double y,
       double x);
#else
void            mertgp();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    tmtogp()
****************************************************************
Transverse Mercator map projection to geog posn
*/

#if XVT_CC_PROTO
void 
tmtogp(
       double a,
       double recf,
       double ophi,
       double olam,
       double fn,
       double fe,
       double ok,
       double *sphi,
       double *slam,
       double y,
       double x);
#else
void            tmtogp();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    upstgp()
****************************************************************
UPS map projection to geog posn
*/

#if XVT_CC_PROTO
void 
upstgp(
       double a,
       double recf,
       double *sphi,
       double *slam,
       double *y,
       double x);
#else
void            mertgp();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    upolar()
****************************************************************
Universal Polar Stereographic map projection
*/

#if XVT_CC_PROTO
void 
upolar(
       double *p,
       double *l,
       int32 dir,
       double *a,
       double f,
       double *y,
       double *x,
       int32 *ierr);
#else
void            upolar();
#endif

/*
 * Description:
 * 
 */
/***************************************************************
@    utm()
****************************************************************
Universal Transverse Mercator map projection
*/

#if XVT_CC_PROTO
void 
utm(
    double *p,
    double *l,
    int32 dir,
    int32 *izone,
    double *a,
    double f,
    double *ok,
    double *y,
    double *x,
    int32 zflag,
    int32 *ierr);
#else
void            utm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    unflow()
****************************************************************
Universal Transverse Mercator map projection
*/

#if XVT_CC_PROTO
void 
unflow(
       double *value,
       int32 kode);
#else
void            unflow();
#endif

/*
 * Description:
 * 
 */
#endif
