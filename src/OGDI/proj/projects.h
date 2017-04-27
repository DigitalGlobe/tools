/* General projections header file */
#ifndef PROJECTS_H
#define PROJECTS_H

#ifndef lint
static const char PROJECTS_H_ID[] = "@(#)projects.h	4.7	93/08/20	GIE	REL";
#endif
    /* standard inclusions */
#include <math.h>
#include <stdlib.h>
	/* prototype hypot for systems where absent */
#ifndef _WINDOWS
extern double hypot(double, double);
#endif
	/* some useful constants */
#define HALFPI		1.5707963267948966
#define FORTPI		0.78539816339744833
#ifndef PI
#define PI		3.14159265358979323846
#endif
#define TWOPI		6.2831853071795864769
#define RAD_TO_DEG	57.29577951308232
#define DEG_TO_RAD	.0174532925199432958

/* environment parameter name */
#ifndef PROJ_LIB
#define PROJ_LIB "PROJ_LIB"
#endif
/* maximum tag id length for +init and default files */
#ifndef ID_TAG_MAX
#define ID_TAG_MAX 50
#endif

/* directory delimiter for DOS support */
#ifdef DOS
#define DIR_CHAR '\\'
#else
#define DIR_CHAR '/'
#endif

typedef struct { double u, v; }	UV;
typedef struct { double r, i; }	COMPLEX;

#ifndef PJ_LIB__
#define XY UV
#define LP UV
#else
typedef struct { double x, y; }     XY;
typedef struct { double lam, phi; } LP;
#endif

	extern int		/* global error return code */
pj_errno;

typedef union { double  f; int  i; char *s; } PROJVALUE;

struct PJ_LIST {
	char	*id;		/* projection keyword */
	void	*(*proj)();	/* projection entry point */
	char	*name;		/* basic projection full name */
};
struct PJ_ELLPS {
	char	*id;	/* ellipse keyword name */
	char	*major;	/* a= value */
	char	*ell;	/* elliptical parameter */
	char	*name;	/* comments */
};
struct PJ_UNITS {
	char	*id;	/* units keyword */
	char	*to_meter;	/* multiply by value to get meters */
	char	*name;	/* comments */
};
struct FACTORS {
	struct DERIVS {
		double x_l, x_p; /* derivatives of x for lambda-phi */
		double y_l, y_p; /* derivatives of y for lambda-phi */
	} der;
	double h, k;	/* meridinal, parallel scales */
	double omega, thetap;	/* angular distortion, theta prime */
	double conv;	/* convergence */
	double s;		/* areal scale factor */
	double a, b;	/* max-min scale error */
	int code;		/* info as to analytics, see following */
};
#define IS_ANAL_XL_YL 01	/* derivatives of lon analytic */
#define IS_ANAL_XP_YP 02	/* derivatives of lat analytic */
#define IS_ANAL_HK	04		/* h and k analytic */
#define IS_ANAL_CONV 010	/* convergence analytic */
    /* parameter list struct */
typedef struct ARG_list {
	struct ARG_list *next;
	char used;
	char param[1]; } paralist;
	/* base projection data structure */
typedef struct PJconsts {
	XY  (*fwd)(LP, struct PJconsts *);
	LP  (*inv)(XY, struct PJconsts *);
	void (*spc)(LP, struct PJconsts *, struct FACTORS *);
	void (*pfree)(struct PJconsts *);
	paralist *params;   /* parameter list */
	int over;   /* over-range flag */
	int geoc;   /* geocentric latitude flag */
	double
		a,  /* major axis or radius if es==0 */
		e,  /* eccentricity */
		es, /* e ^ 2 */
		ra, /* 1/A */
		one_es, /* 1 - e^2 */
		rone_es, /* 1/one_es */
		lam0, phi0, /* central longitude, latitude */
		x0, y0, /* easting and northing */
		k0,	/* general scaling factor */
		to_meter, fr_meter; /* cartesian scaling */
#ifdef PROJ_PARMS__
PROJ_PARMS__
#endif /* end of optional extensions */
} PJ;

#ifndef PJ_LIST__
extern struct PJ_LIST pj_list[];
#endif

#ifndef PJ_ELLPS__
extern struct PJ_ELLPS pj_ellps[];
#endif

#ifndef PJ_UNITS__
extern struct PJ_UNITS pj_units[];
#endif

#ifdef PJ_LIB__
    /* repeatative projection code */
#define ENTRYA(name) PJ *name(PJ *P) { if (!P) { \
	if (P = pj_malloc(sizeof(PJ))) { \
	P->pfree = freeup; P->fwd = 0; P->inv = 0; P->spc = 0;
#define ENTRYX } return P; } else {
#define ENTRY0(name) ENTRYA(name) ENTRYX
#define ENTRY1(name, a) ENTRYA(name) P->a = 0; ENTRYX
#define ENTRY2(name, a, b) ENTRYA(name) P->a = 0; P->b = 0; ENTRYX
#define ENDENTRY(p) } return (p); }
#define E_ERROR(err) { pj_errno = err; freeup(P); return(0); }
#define E_ERROR_0 { freeup(P); return(0); }
#define F_ERROR { pj_errno = -20; return(xy); }
#define I_ERROR { pj_errno = -20; return(lp); }
#define FORWARD(name) static XY name(LP lp, PJ *P) { XY xy
#define INVERSE(name) static LP name(XY xy, PJ *P) { LP lp
#define FREEUP static void freeup(PJ *P) {
#define SPECIAL(name) static void name(LP lp, PJ *P, struct FACTORS *fac)
#endif
#define MAX_TAB_ID 80
typedef struct { float lam, phi; } FLP;
typedef struct { int lam, phi; } ILP;
struct CTABLE {
	char id[MAX_TAB_ID]; /* ascii info */
	LP ll;      /* lower left corner coordinates */
	LP del;     /* size of cells */
	ILP lim;    /* limits of conversion matrix */
	FLP *cvs;   /* conversion matrix */
};
	/* procedure prototypes */
double  dmstor(const char *, char **);
void set_rtodms(int, int);
char *rtodms(char *, double, int, int);
double adjlon(double);
double aacos(double), aasin(double), asqrt(double);
PROJVALUE pj_param(paralist *, char *);
paralist *pj_mkparam(char *);
int pj_ell_set(paralist *, double *, double *);
double *pj_enfn(double);
double pj_mlfn(double, double, double, double *);
double pj_inv_mlfn(double, double, double *);
double pj_qsfn(double, double, double);
double pj_tsfn(double, double, double);
double pj_msfn(double, double, double);
double pj_phi2(double, double);
double pj_qsfn_(double, PJ *);
double *pj_authset(double);
double pj_authlat(double, double *);
COMPLEX pj_zpoly1(COMPLEX, COMPLEX *, int);
COMPLEX pj_zpolyd1(COMPLEX, COMPLEX *, int, COMPLEX *);
int pj_deriv(LP, double, PJ *, struct DERIVS *);
int pj_factors(LP, PJ *, double, struct FACTORS *);
XY pj_fwd(LP, PJ *);
LP pj_inv(XY, PJ *);
void pj_pr_list(PJ *);
void pj_free(PJ *);
PJ *pj_init(int, char **);
void *pj_malloc(size_t);
void pj_dalloc(void *);
char *pj_strerrno(int);
/* Approximation structures and procedures */
typedef struct {	/* Chebyshev or Power series structure */
	UV a, b;		/* power series range for evaluation */
					/* or Chebyshev argument shift/scaling */
	struct PW_COEF {/* row coefficient structure */
		int m;		/* number of c coefficients (=0 for none) */
		double *c;	/* power coefficients */
	} *cu, *cv;
	int mu, mv;		/* maximum cu and cv index (+1 for count) */
	int power;		/* != 0 if power series, else Chebyshev */
} Tseries;
Tseries *mk_cheby(UV, UV, double, UV *, UV (*)(UV), int, int, int);
UV bpseval(UV, Tseries *);
UV bcheval(UV, Tseries *);
UV biveval(UV, Tseries *);
void *vector1(int, int);
void **vector2(int, int, int);
int bchgen(UV, UV, int, int, UV **, UV(*)(UV));
int bch2bps(UV a, UV b, UV **, int, int);
/* nadcon related protos */
LP nad_interp(LP, struct CTABLE *);
LP nad_cvt(LP, int, struct CTABLE *);
struct CTABLE *nad_init(char *);
void nad_free(struct CTABLE *);

#endif /* end of basic projections header */
