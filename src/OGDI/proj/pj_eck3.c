/*  Eckert III */
#ifndef lint
static const char SCCSID[]="@(#)PJ_eck3.c	4.4	93/06/13	GIE	REL";
#endif
#define PJ_LIB__
#include	<projects.h>
#define XF	.42223820031577120149
#define RXF	2.36833142821314873781
#define YF	.84447640063154240298
#define RYF	1.18416571410657436890
FORWARD(s_forward); /* spheroid */
	if (fabs(xy.x = lp.phi / HALFPI) >= 1.)
		xy.x = XF * lp.lam;
	else
		xy.x = XF * (1. + sqrt(1. - xy.x*xy.x)) * lp.lam;
	xy.y = YF * lp.phi;
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	double t;

	lp.lam = xy.x * RXF;
	if (fabs(t = (lp.phi = RYF * xy.y) / HALFPI) < 1.)
		lp.lam /= 1. + sqrt(1. - t*t);
	return (lp);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_eck3) P->es = 0.; P->inv = s_inverse; P->fwd = s_forward; ENDENTRY(P)
