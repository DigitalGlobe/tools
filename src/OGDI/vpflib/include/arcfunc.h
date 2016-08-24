#ifndef H_ARC_FUNC
#define H_ARC_FUNC

/*
 * Prototypes for functions in arc.c
 */

#if XVT_CC_PROTO
int             arc(int, double *, double *, int32 *, int32 *, ARC_PARMS *, TILE_INFO *);
void            arc_to_lat(int32, int32, double, double, double, double, int, double *, double *);
double          dms2dec(char *);
void            get_ab(int, double *, double *);
int             get_arc_zone(double);
int             get_overlap_coords(ARC_PARMS * parms, int32 *north_row, int32 *south_row);
void            lat_to_arc(double, double, double, double, int, double, double, int32 *, int32 *);
int             read_arc_parms(ARC_PARMS *);
void            scale_adjust(double *a, double *b, int32 scale);
char           *strip_str(char *, char);
#else
int             arc();
void            arc_to_lat();
double          dms2dec();
void            get_ab();
int             get_arc_zone();
int             get_overlap_coords();
void            lat_to_arc();
int             read_arc_parms();
void            scale_adjust();
char           *strip_str();
#endif

#endif				/* H_ARC_FUNC */
