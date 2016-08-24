#ifndef H_VEC_F
#define H_VEC_F

#ifndef INCL_XVTH
#include "xvt.h"
#endif
#ifndef H_VEC_D
#include "vec_d.h"
#endif
#ifndef H_MGM_D
#include "mgm_d.h"
#endif

/***************************************************************
@    area_feature_construct()
****************************************************************
Area feature constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API area_feature_construct(FILE *file, AREA_FEATURE ***area,
	int32 nr_features);
#else
ERRSTATUS MUSE_API area_feature_construct();
#endif
/*
Description:
If file is not NULL the area feature is written to the file.
If destruct is TRUE the area feature is released.
*/


/***************************************************************
@    line_feature_construct()
****************************************************************
Line feature constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API line_feature_construct(FILE *file,LINE_FEATURE ***lines,
	int32 nr_features);
#else
ERRSTATUS MUSE_API line_feature_construct();
#endif
/*
Description:
If file is not NULL the line feature is written to the file.
If destruct is TRUE the line feature is released.
*/

/***************************************************************
@    point_feature_construct()
****************************************************************
Point feature constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API point_feature_construct(FILE *file, POINT_FEATURE ***points,
	int32 nr_features);
#else
ERRSTATUS MUSE_API point_feature_construct();
#endif
/*
Description:
If file is not NULL the point feature is written to the file.
If destruct is TRUE the point feature is released.
*/

/***************************************************************
@    text_feature_construct()
****************************************************************
Text feature constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API text_feature_construct(FILE *file, TEXT_FEATURE ***text,
	int32 nr_features);
#else
ERRSTATUS MUSE_API text_feature_construct();
#endif
/*
Description:
If file is not NULL the text feature is written to the file.
If destruct is TRUE the text feature is released.
*/

/***************************************************************
@    decode_area_symb()
****************************************************************
Decode area symbology.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API decode_area_symb(unsigned char *record, AREA_SYMB *area_symb);
#else
ERRSTATUS MUSE_API decode_area_symb();
#endif
/*
Description:
Decodes data into the AREA_SYMB structure after read from file.
*/

/***************************************************************
@    area_symb_construct()
****************************************************************
Area symbology constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API area_symb_construct(FILE *file, AREA_SYMB **area_symb);
#else
ERRSTATUS MUSE_API area_symb_construct();
#endif
/*
Description:
If file is not NULL the area symbology is written to the file.
If destruct is TRUE the area symbology is released.
*/

/***************************************************************
@    decode_line_symb()
****************************************************************
Decode line symbology.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API decode_line_symb(unsigned char *record, LINE_SYMB *line_symb);
#else
ERRSTATUS MUSE_API decode_line_symb();
#endif
/*
Description:
Decodes data into the LINE_SYMB structure after read from file.
*/


/***************************************************************
@    line_symb_construct()
****************************************************************
Line symbology constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API line_symb_construct(FILE *file, LINE_SYMB **line_symb);
#else
ERRSTATUS MUSE_API line_symb_construct();
#endif
/*
Description:
If file is not NULL the line symbology is written to the file.
If destruct is TRUE the line symbology is released.
*/

/***************************************************************
@    decode_point_symb()
****************************************************************
Decode point symbology.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API decode_point_symb(unsigned char *record, POINT_SYMB *point_symb);
#else
ERRSTATUS MUSE_API decode_point_symb();
#endif
/*
Description:
Decodes data into the POINT_SYMB structure after read from file.
*/

/***************************************************************
@    point_symb_construct()
****************************************************************
Point symbology constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API point_symb_construct(FILE *file, POINT_SYMB **point_symb);
#else
ERRSTATUS MUSE_API point_symb_construct();
#endif
/*
Description:
If file is not NULL the point symbology is written to the file.
If destruct is TRUE the point symbology is released.
*/

/***************************************************************
@    decode_symbology()
****************************************************************
Decode symbology.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API decode_symbology(unsigned char *record, SYMBOLOGY *symb);
#else
ERRSTATUS MUSE_API decode_symbology();
#endif
/*
Description:
Decodes data into the SYMBOLOGY structure after read from file.
*/

/***************************************************************
@    symbology_construct()
****************************************************************
Symbology constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API symbology_construct(FILE *file, SYMBOLOGY **symb);
#else
ERRSTATUS MUSE_API symbology_construct();
#endif
/*
Description:
If file is not NULL the symbology is written to the file.
If destruct is TRUE the symbology is released.
*/


/***************************************************************
@    decode_text_symb()
****************************************************************
Decode text symbology
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API decode_text_symb(unsigned char *record, TEXT_SYMB *text_symb);
#else
ERRSTATUS MUSE_API decode_text_symb();
#endif
/*
Description:
Decodes data into the TEXT_SYMB structure after read from file.
*/


/***************************************************************
@    text_symb_construct()
****************************************************************
Text symbology constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API text_symb_construct(FILE *file, TEXT_SYMB **text_symb);
#else
ERRSTATUS MUSE_API text_symb_construct();
#endif
/*
Description:
If file is not NULL the text symbology is written to the file.
If destruct is TRUE the text symbology is released.
*/

/***************************************************************
@    decode_vec_extents()
****************************************************************
Decode vector extents.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API decode_vec_extents(unsigned char *record, VEC_EXTENT *extents);
#else
ERRSTATUS MUSE_API decode_vec_extents();
#endif
/*
Description:
Decodes data into the VEC_EXTENT structure after read from file.
*/

/***************************************************************
@    vec_extent_construct()
****************************************************************
Vector extent constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API vec_extent_construct(FILE *file, VEC_EXTENT **extents);
#else
ERRSTATUS MUSE_API vec_extent_construct();
#endif
/*
Description:
If file is not NULL the vector extent is written to the file.
If destruct is TRUE the vector extent is released.
*/

/***************************************************************
@    vec_info_construct()
****************************************************************
Vector information constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API vec_info_construct(FILE *file, VEC_INFO **info);
#else
ERRSTATUS MUSE_API vec_info_construct();
#endif
/*
Description:
If file is not NULL the vector information is written to the file.
If destruct is TRUE the vector information is released.
*/

/***************************************************************
@    decode_vec()
****************************************************************
Decode Vector.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API decode_vec(unsigned char *record, VEC *vec);
#else
ERRSTATUS MUSE_API decode_vec();
#endif
/*
Description:
Decodes data into the VECTOR structure after read from file.
*/


/***************************************************************
@    vec_construct()
****************************************************************
Vector constructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API vec_construct(FILE *file, VEC **vec);
#else
ERRSTATUS MUSE_API vec_construct();
#endif
/*
Description:
If file is not NULL the vector is written to the file.
If destruct is TRUE the vector is released.
*/

/***************************************************************
@    is_vec_ok()
****************************************************************
Test Vector Magic number for validity.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API is_vec_ok(FILE *file);
#else
ERRSTATUS MUSE_API is_vec_ok();
#endif
/*
Description:
Checks a newly opened vector file to see
is it has a correct magic number.
*/



/***************************************************************
@    area_feature_destruct()
****************************************************************
Area feature destructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API area_feature_destruct(FILE *file, BOOLEAN destruct, AREA_FEATURE ***area,
	int32 nr_areas);
#else
ERRSTATUS MUSE_API area_feature_destruct();
#endif
/*
Description:
Constructs a area feature object. If file is not NULL the area
feature is read in from the file.
*/

/***************************************************************
@    line_feature_destruct()
****************************************************************
Line feature destructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API line_feature_destruct(FILE *file, BOOLEAN destruct, LINE_FEATURE ***lines,
	int32 nr_lines);
#else
ERRSTATUS MUSE_API line_feature_destruct();
#endif
/*
Description:
Constructs a line feature object. If file is not NULL the line
feature is read in from the file.
*/

/***************************************************************
@    point_feature_destruct()
****************************************************************
Point feature destructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API point_feature_destruct(FILE *file, BOOLEAN destruct, POINT_FEATURE ***points,
	int32 nr_points);
#else
ERRSTATUS MUSE_API point_feature_destruct();
#endif
/*
Description:
Constructs a point feature object. If file is not NULL the point
feature is read in from the file.
*/

/***************************************************************
@    text_feature_destruct()
****************************************************************
Text feature destructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API text_feature_destruct(FILE *file, BOOLEAN destruct, TEXT_FEATURE ***text,
	int32 nr_text);
#else
ERRSTATUS MUSE_API text_feature_destruct();
#endif
/*
Description:
Constructs a text feature object. If file is not NULL the text
feature is read in from the file.
*/

/***************************************************************
@    encode_area_symb()
****************************************************************
Encode area symbology.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API encode_area_symb(unsigned char *record, AREA_SYMB *area_symb);
#else
ERRSTATUS MUSE_API encode_area_symb();
#endif
/*
Description:
Encodes the AREA_SYMB structure so that it can be written
portably.
*/

/***************************************************************
@    area_symb_destruct()
****************************************************************
Area symbology destructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API area_symb_destruct(FILE *file, BOOLEAN destruct, AREA_SYMB **area_symb);
#else
ERRSTATUS MUSE_API area_symb_destruct();
#endif
/*
Description:
Constructs a area symbology object. If file is not NULL the area
symbology is read in from the file.
*/

/***************************************************************
@    encode_line_symb()
****************************************************************
Encode line symbology.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API encode_line_symb(unsigned char *record, LINE_SYMB *line_symb);
#else
ERRSTATUS MUSE_API encode_line_symb();
#endif
/*
Description:
Encodes the LINE_SYMB structure so that it can be written
portably.
*/


/***************************************************************
@    line_symb_destruct()
****************************************************************
Line symbology destructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API line_symb_destruct(FILE *file, BOOLEAN destruct, LINE_SYMB **line_symb);
#else
ERRSTATUS MUSE_API line_symb_destruct();
#endif
/*
Description:
Constructs a vector line symbology. If file is not NULL the line
symbology is read in from the file.
*/

/***************************************************************
@    encode_point_symb()
****************************************************************
Encode point symbology.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API encode_point_symb(unsigned char *record, POINT_SYMB *point_symb);
#else
ERRSTATUS MUSE_API encode_point_symb();
#endif
/*
Description:
Encodes the POINT_SYMB structure so that it can be written
portably.
*/

/***************************************************************
@    point_symb_destruct()
****************************************************************
Point symbology destructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API point_symb_destruct(FILE *file, BOOLEAN destruct, POINT_SYMB **point_symb);
#else
ERRSTATUS MUSE_API point_symb_destruct();
#endif
/*
Description:
Constructs a point symbology object. If file is not NULL the point
symbology is read in from the file.
*/

/***************************************************************
@    encode_symbology()
****************************************************************
Encode symbology.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API encode_symbology(unsigned char *record, SYMBOLOGY *symb);
#else
ERRSTATUS MUSE_API encode_symbology();
#endif
/*
Description:
Encodes the SYMBOLOGY structure so that it can be written
portably.
*/

/***************************************************************
@    symbology_destruct()
****************************************************************
Symbology destructor
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API symbology_destruct(FILE *file, BOOLEAN destruct, SYMBOLOGY **symb);
#else
ERRSTATUS MUSE_API symbology_destruct();
#endif
/*
Description:
Constructs a symbology object. If file is not NULL the symbology
is read in from the file.
*/

/***************************************************************
@    encode_text_symb()
****************************************************************
Encode text symbology.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API encode_text_symb(unsigned char *record, TEXT_SYMB *text_symb);
#else
ERRSTATUS MUSE_API encode_text_symb();
#endif
/*
Description:
Encodes the TEXT_SYMB structure so that it can be written
portably.
*/

/***************************************************************
@    text_symb_destruct()
****************************************************************
Text symbology destructor. 
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API text_symb_destruct(FILE *file, BOOLEAN destruct, TEXT_SYMB **text_symb);
#else
ERRSTATUS MUSE_API text_symb_destruct();
#endif
/*
Description:
Constructs a text symbology object. If file is not NULL the text
symbology is read in from the file.
*/


/***************************************************************
@    encode_vec_extents()
****************************************************************
Encode vector extents.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API encode_vec_extents(unsigned char *record, VEC_EXTENT *extents);
#else
ERRSTATUS MUSE_API encode_vec_extents();
#endif
/*
Description:
Encodes the VEC_EXTENT structure so that it can be written
portably.
*/

/***************************************************************
@    vec_extent_destruct()
****************************************************************
Vector extent destructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API vec_extent_destruct(FILE *file, BOOLEAN destruct, VEC_EXTENT **extents);
#else
ERRSTATUS MUSE_API vec_extent_destruct();
#endif
/*
Description:
Constructs a vector extent object. If file is not NULL the vector
extent is read in from the file.
*/

/***************************************************************
@    vec_info_destruct()
****************************************************************
Vector information destructor
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API vec_info_destruct(FILE *file, BOOLEAN destruct, VEC_INFO **info);
#else
ERRSTATUS MUSE_API vec_info_destruct();
#endif
/*
Description:
Constructs a vector information object. If file is not NULL the
vector information is read in from the file.
*/

/***************************************************************
@    encode_vec()
****************************************************************
Encode vector.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API encode_vec(unsigned char *record, VEC *vec);
#else
ERRSTATUS MUSE_API encode_vec();
#endif
/*
Description:
Encodes the VECTOR structure so that it can be written
portably.
*/

/***************************************************************
@    vec_destruct()
****************************************************************
Vector destructor.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API vec_destruct(FILE *file, BOOLEAN destruct, VEC **vec);
#else
ERRSTATUS MUSE_API vec_destruct();
#endif
/*
Description:
Constructs a vector object. If file is not NULL the vector
is read in from the file.
*/

/***************************************************************
@    draw_vec()
****************************************************************
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API draw_vec(
    MGM *mgm,
    VEC *vec,
    WINDOW map_win,
    BOOLEAN *no_process_vector);
#else
ERRSTATUS MUSE_API draw_vec();
#endif
/*
Description:
*/

/***************************************************************
@    draw_vec_areas()
****************************************************************
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API draw_vec_areas(
    MGM * mgm,
    VEC * vec,
    WINDOW map_win,
    BOOLEAN * npv);
#else
ERRSTATUS MUSE_API draw_vec_areas();
#endif
/*
Description:
*/

/***************************************************************
@    draw_vec_lines()
****************************************************************
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API draw_vec_lines(
    MGM * mgm,
    VEC * vec,
    WINDOW map_win,
    BOOLEAN * npv);
#else
ERRSTATUS MUSE_API draw_vec_lines();
#endif
/*
Description:
*/

/***************************************************************
@    draw_vec_points()
****************************************************************
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API draw_vec_points(
    MGM * mgm,
    VEC * vec,
    WINDOW map_win,
    BOOLEAN * npv);
#else
ERRSTATUS MUSE_API draw_vec_points();
#endif
/*
Description:
*/

/***************************************************************
@    draw_vec_text()
****************************************************************
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API draw_vec_text(
    MGM * mgm,
    VEC * vec,
    WINDOW map_win,
    BOOLEAN * npv);
#else
ERRSTATUS MUSE_API draw_vec_text();
#endif
/*
Description:
*/

#endif /* H_VEC_F */
