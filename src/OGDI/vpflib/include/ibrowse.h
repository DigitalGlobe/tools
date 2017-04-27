/*
** iBrowse.H   Header for iBrowse functions.
*/

#define NUM_ITEMS       20                          /* number items on Windows menu */
#define M_WINDOWS       300                         /* Windows menu tag             */
#define PLACEHOLDER     (M_WINDOWS + NUM_ITEMS + 1) /* tag for dummy Win menu item  */

#ifndef STANDALONE
extern void    ibrowse_init       XVT_CC_ARGS((void));
extern void    ibrowse_destroy    XVT_CC_ARGS((void));
extern void    ibrowse_file       XVT_CC_ARGS((char *, FILE_SPEC *));
extern void    ibrowse_slist      XVT_CC_ARGS((char *, SLIST));
#else
static int32    task_eh           XVT_CC_ARGS((WINDOW, EVENT *));
#endif

#define MAGIC     25931   /* magic number                           */
#define GRANULE   100     /* allocation granule for lines array     */
#define MGN       2       /* margin (pixels)                        */
#define HINTERVAL 50      /* horizontal scrolling interval (pixels) */
#define VRANGE    10000   /* vertical scrolling range               */
