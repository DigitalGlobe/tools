#ifndef H_VECTOR
#define H_VECTOR


#ifndef H_MUSE1
#include "muse1.h"
#endif
#ifndef H_PHIGS_D
#include "phigs_d.h"
#endif
#ifndef H_VEC_D
#include "vec_d.h"
#endif
#ifndef H_SQLLIB_D
#include "sqllib_d.h"
#endif

#ifndef _MACHINE_
#include "machine.h"
#endif

#ifndef MAGIC_VECTOR
#define MAGIC_VECTOR   110000000L
#endif

#ifndef MAGIC_VEC_DATA		/* SR 10/02 */
#define MAGIC_VEC_DATA 120000001L
#endif

/* SR 10/02 */
/***************************************************************
@    Vec_Data flags
****************************************************************
*/
#define VEC_DATA_VALID_MASK              0x0001
#define VEC_DATA_NEEDS_REDRAW_MASK       0x0002
#define VEC_DATA_USE_TIMER_MASK          0x0003

/*
 * Description:
 */


/* SR 09/16 */
/***************************************************************
@    VEC_DATA
****************************************************************
Defines a vector data structure
*/
typedef struct
{
    int32            magic;
    int32            bit_flags;
    BOOLEAN         contains_valid_data;
/*to compile    SEL_THEME       sel_theme;*/
    VEC            *vector;

    FILE_SPEC       file_spec;
    int32            vec_timer;
}               VEC_DATA;



/*
 * SR 09/25--list of functions for constructing map versions with different
 * vec_data structures
 */

#if XVT_CC_PROTO
ERRSTATUS       vec_con_vers0(FILE * file, VEC_DATA ** pointer, BYTE_ORDER * bo);
#else
ERRSTATUS       vec_con_vers0();
#endif

/*
 * Memory allocation and initialization for version 0 maps whose
 * product_pointers point to a VECTOR structure. Only called when opening an
 * old .map file: added overlays will be created with vec_data_construct for
 * the latest version of maps.
 */


#if XVT_CC_PROTO
ERRSTATUS       vec_con_vers1(FILE * file, VEC_DATA ** pointer, BYTE_ORDER * bo);
#else
ERRSTATUS       vec_con_vers1();
#endif

/*
 * Memory allocation and initialization of the VEC_DATA structure in version
 * 1 maps, which contains edge color and width, text and point color, and a
 * pointer to a VECTOR structure.
 */


/***************************************************************
@     (*vec_data_construct[])()
****************************************************************
list of functions to construct vec_data structure for each map version
*/

#if XVT_CC_PROTO
extern          ERRSTATUS(*vec_data_construct[2]) (FILE * file, VEC_DATA ** pointer, BYTE_ORDER * bo);
#else
extern          ERRSTATUS(*vec_data_construct[2]) ();
#endif


/***************************************************************
@     vec_data_destruct()
****************************************************************
function to destruct vec_data structure
*/

#if XVT_CC_PROTO
ERRSTATUS       vec_data_destruct(FILE * file, BOOLEAN destruct, VEC_DATA ** vec_data, BYTE_ORDER * bo);
#else
ERRSTATUS       vec_data_destruct();
#endif


/************************************************************************
@     decode_vec_data()
*************************************************************************
function to decode the vec_data structure
*/

#if XVT_CC_PROTO
ERRSTATUS       decode_vec_data(unsigned char *record, VEC_DATA * vec_data);
#else
ERRSTATUS       decode_vec_data();
#endif

/************************************************************************
@     encode_vec_data()
*************************************************************************
A function to encode the vec_data structure*/

#if XVT_CC_PROTO
ERRSTATUS       encode_vec_data(unsigned char *record, VEC_DATA * vec_data);
#else
ERRSTATUS       encode_vec_data();
#endif

#endif				/* H_VECTOR */
