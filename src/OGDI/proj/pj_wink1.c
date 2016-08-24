/*  Winkel I */
#ifndef lint
static const char SCCSID[]="@(#)PJ_wink1.c	4.5	93/06/14	GIE	REL";
#endif
#define PROJ_PARMS__ \
	double	cosphi1;
#define PJ_LIB__
# include	<projects.h>
FORWARD(s_forward); /* spheroid */
	xy.x = .5 * lp.lam * (P->cosphi1 + cos(lp.phi));
	xy.y = lp.phi;
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	lp.phi = xy.y;
	lp.lam = 2. * xy.x / (P->cosphi1 + cos(lp.phi));
	return (lp);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_wink1)
	P->cosphi1 = cos(pj_param(P->params, "rlat_ts").f);
	P->es = 0.; P->inv = s_inverse; P->fwd = s_forward;
ENDENTRY(P)
