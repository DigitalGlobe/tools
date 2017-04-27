/* Central Cylindrical projection */
#ifndef lint
static const char SCCSID[]="@(#)PJ_cc.c	4.4	93/06/13	GIE	REL";
#endif
#define PROJ_PARMS__ \
	double ap;
#define PJ_LIB__
#include	<projects.h>
#define EPS10 1.e-10
FORWARD(s_forward); /* spheroid */
	if (fabs(fabs(lp.phi) - HALFPI) <= EPS10) F_ERROR;
	xy.x = lp.lam;
	xy.y = tan(lp.phi);
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	lp.phi = atan(xy.y);
	lp.lam = xy.x;
	return (lp);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_cc) P->es = 0.; P->inv = s_inverse; P->fwd = s_forward; ENDENTRY(P)
