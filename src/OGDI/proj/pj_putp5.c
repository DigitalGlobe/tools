/*  Putnins P5 Projection */
#ifndef lint
static const char SCCSID[]="@(#)PJ_putp5.c	4.4	93/06/14	GIE	REL";
#endif
#define PJ_LIB__
#include	<projects.h>
#define FXC	1.01346
#define FYC	1.01346
#define CS	1.21585420370805325734
FORWARD(s_forward); /* spheroid */
	xy.x = FXC * lp.lam * (2. - sqrt(1. + CS * lp.phi * lp.phi));
	xy.y = FYC * lp.phi;
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	lp.phi = xy.y / FYC;
	lp.lam = xy.x / (FXC * (2. - sqrt(1. + CS * lp.phi * lp.phi)));
	return (lp);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_putp5) P->es = 0.; P->inv = s_inverse; P->fwd = s_forward; ENDENTRY(P)
