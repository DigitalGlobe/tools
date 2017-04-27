/* VPF.H */

#ifndef __VPF_H__
#define __VPF_H__ 1

#ifndef _MACHINE_
#include "machine.h"
#endif

/* Define some standard VPF-related data types and structures */


/* Currently supported VPF versions */
typedef enum { VPF_0_7, VPF_0_8, VPF_0_9, VPF_1_0 } vpf_version_type;


/* VPF feature types */
typedef enum { LINE=1, AREA, ANNO, VPFPOINTS, VPFCOMPLEX=6 } vpf_feature_type;

/* VPF primitive types */
typedef enum { EDGE=1, FACE, TEXT, ENTITY_NODE, CONNECTED_NODE }
   vpf_primitive_type;

typedef struct {
   unsigned char edge;
   unsigned char face;
   unsigned char text;
   unsigned char entity_node;
   unsigned char connected_node;
} primitive_class_type;


typedef enum { UNKNOWN_SECURITY, UNCLASSIFIED, RESTRICTED, CONFIDENTIAL,
               SECRET, TOP_SECRET } security_type;


/* Units of measure */
typedef enum { UNKNOWN_UNITS, METERS, FEET, INCHES,
               KILOMETERS, OTHER_UNITS, DEC_DEGREES } vpf_units_type;


/* Map coordinate projection definitions */

typedef enum {
   DDS,   /* Decimal Degrees */
   AC,   /* Albers Equal Area */
   AK,   /* Azimuthal Equal Area */
   AL,   /* Azimuthal Equal Distance */
   GN,   /* Gnomonic */
   LE,   /* Lambert Conformal Conic */
   LJ,   /* Lambert (Cylindrical) Equal Area */
   MC,   /* Mercator */
   OC,   /* Oblique Mercator */
   OD,   /* Orthographic */
   PG,   /* Polar Stereographic */
   TC,   /* Transverse Mercator */
   UT,   /* UTM */
   PC    /* Plate-Carree */
} vpf_projection_code;


typedef struct
   {
   vpf_projection_code code;
   double parm1, parm2, parm3, parm4;
   vpf_units_type units;
   double false_easting, false_northing;
   int32 (*forward_proj)();
   int32 (*inverse_proj)();
   char name[21];
   } vpf_projection_type;

typedef unsigned char boolean;

#endif
