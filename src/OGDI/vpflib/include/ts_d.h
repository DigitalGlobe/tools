/******************************************************************************
 *++
 *           MAP DATA FORMATTING FACILITY
 *
 ******************************************************************************
 *
 * TITLE: m4_const.h
 *
 * DESCRIPTION: Constants defining the Model 4 Tesselated Spheriod projection.
 *
 * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE *
 *
 * Any modification made to this module MUST be made to M4_CONSTANTS.INC also.
 *
 * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE *
 *
 * SYNTAX:
 *
 * ARGUMENTS:
 *
 * REQUIRED SUBROUTINES:
 *
 * HISTORY:
 *	 Written: P.B.Wischow (NOARL) November 1990, Written to provide
 *			Model 4 TS projection constants in "C".
 *
 *	Modified: S.A.Myrick (NRL), 22 January 1992.  Added TS zone segment
 *                      boundaries and integer constants (for each TS zone)
 *
 *==
 ******************************************************************************
 */

#ifndef H_TS_D
#define H_TS_D 1
/* Number of zones in the Model 4 TS projection system */
#define NUM_ZONES  5

/* Zone constants in Model 4 TS projection system */

/*
 * static int SP_ZONE = 0; static int ST_ZONE = 1; static int EQ_ZONE = 2;
 * static int NT_ZONE = 3; static int NP_ZONE = 4;
 */


/* Number of tesselations in any latitude */
static int      nbr_tess_lat = 195;

/* Number of columns per zone at 1:2M scale, half hemisphere */
static int      nbr_tess[5] = {
190, 152, 190, 152, 190};

/* Number of columns per zone at 1:2M scale, for the entire globe */
/* static int nbr_tess2[5] = {380,304,380,304,380}; */

/* Latitude boundary between model 4 temperate and equator zone */
/* static double zborder_tempequ = 31.38461538; */

/* Latitude boundary between model 4 polar and temperate zone */
/* static double zborder_polartemp = 51.69230769; */

/****** NOTE ******************************************************************/
/* If point falls on a boundary, then the point is in the next northern zone. */
/****** NOTE ******************************************************************/



static double   bot_zone_lat_tbl[5] =	/* Bottom zone latitude table */
{
    -90.0,
    -51.69230769,
    -31.38461538,
    31.38461538,
51.69230769};

/***** The following are used as descriptive indeces for ZONE_SEG_BOUND *****
#define ZONE_TOP 1    /* Top of zone boundry */
#define ZONE_BOT 2		/* Bottom of zone boundry */

/***** The following are the Max/Min rows for each scale and  *****/
/***** non-polar zone.  Because the polar zone row/column     *****/
/***** boundaries are by definition undefined, they are       *****/
/***** zeroed here.  These bounds EXCLUDE the two segment     *****/
/***** overlap.                                               *****/

/* Note that array dimensions are: scale, zone, top/bottom rows.     */
/* Due to language differences, this C program uses array dimensions */
/* that differ from the FORTRAN version of this file!  Currently,    */
/* there are six scales and five zones.                              */

/*
 * static int  zone_seg_bound [6][5][2] = { 0,       0,	* Top, Bottom, SP
 * Scale = TLM * -1361,   -2240,	*              ST               *
 * 1359,   -1360,	*              EQ               * 2239,    1360,    *
 * NT               * 0,       0,	*              NP               * 0,
 * 0,	* Top, Bottom, SP  Scale = 100k * -681,   -1120,	*
 * ST               * 679,    -680,  	*              EQ               *
 * 1119,     680, 	*              NT               * 0,       0, 	*
 * NP               * 0,       0, 	* Top, Bottom  SP  Scale = JOG  *
 * -273,    -448,	*              ST               * 271,    -272,	*
 * EQ               * 447,     272,	*              NT               * 0,
 * 0,	*              NP               * 0,       0,	* Top, Bottom  SP
 * Scale = TPC  * -137,    -224,	*              ST               *
 * 135,    -136,    *              EQ               * 223,     136, 	*
 * NT               * 0,       0,	*              NP               * 0,
 * 0,	* Top, Bottom  SP  Scale = ONC  * -69,    -112,	*              ST               *
 * 67,     -68, 	*              EQ               * 111,      68,	*
 * NT               * 0,       0,	*              NP               * 0,
 * 0,	* Top, Bottom  SP  Scale = JNC  * -35,     -56, 	*
 * ST               * 33,     -34, 	*              EQ               * 55,
 * 34, 	*              NT               * 0,       0     *              NP               *
 * };
 */

#define TABLE_OFFSET 9

/* Model 4 TS zones */
/* static char *zone_table[5]={"SP","ST","EQ","NT","NP"}; */

/* Number of tesselations in each Model 4 zone */
/* where, MSCALE_Number * scale = 2 million    */
static int      mscale[TABLE_OFFSET] = {
40, 20, 8, 4, 2, 1, 0, 200, 25};

/*
 * static char *scale_table[TABLE_OFFSET]={ "50K ","100K","250K","500K","1M
 * ","2M  ", "5M  ","10K ","80K" }; static char *chart_series[TABLE_OFFSET]={
 * "TLM","XXX","JOG","TPC","ONC","JNC","GNC","CG ","H2 " }; static char
 * *scale_codes[18]={ "TL","XX","JA","TP","ON","JN","GN","CG","H2",
 * "XX","XX","JC","XX","XX","XX","XX","XX","XX" };
 */
#endif	/* H_TS_D */
