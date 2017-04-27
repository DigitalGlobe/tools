/*  Wagner VII Projection */
#ifndef lint
static const char SCCSID[]="@(#)PJ_wag7.c	4.4	93/06/14	GIE	REL";
#endif
#define PJ_LIB__
#include	<projects.h>
#define THIRD	0.3333333333333333333
FORWARD(s_forward); /* spheroid */
	double s, c0, c1;

	s = 0.90631 * sin(lp.phi);
	c0 = sqrt(1. - s * s);
	c1 = sqrt(2./(1. + c0 * cos(lp.lam *= THIRD)));
	xy.x = 2.66723 * c0 * c1 * sin(lp.lam);
	xy.y = 1.24104 * s * c1;
	return (xy);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_wag7) P->es = 0.; P->fwd = s_forward; ENDENTRY(P)
