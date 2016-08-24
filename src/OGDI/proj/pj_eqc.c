/* Equidistant Cylindrical projection (Plate Caree) */
#ifndef lint
static const char SCCSID[]="@(#)PJ_eqc.c	4.5	93/06/13	GIE	REL";
#endif
#define PROJ_PARMS__ \
	double rc;
#define PJ_LIB__
# include	<projects.h>
FORWARD(s_forward); /* spheroid */
	xy.x = P->rc * lp.lam;
	xy.y = lp.phi;
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	lp.phi = xy.y;
	lp.lam = xy.x / P->rc;
	return (lp);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_eqc)
	if ((P->rc = cos(pj_param(P->params, "rlat_ts").f)) <= 0.) E_ERROR(-24);
	P->inv = s_inverse;
	P->fwd = s_forward;
	P->es = 0.;
ENDENTRY(P)
