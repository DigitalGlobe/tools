/* VPFVIEW.H */

#ifndef __VPFVIEW_H__

#define __VPFVIEW_H__ 1


#ifndef __SET_H__
#include "set.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif
#ifndef __LINKLIST_H__
#include "linklist.h"
#endif
#ifndef __VPF_H__
#include "vpf.h"
#endif
#ifndef __COORGEOM_H__
#include "coorgeom.h"
#endif



/* VPF library internal structure */
typedef struct {
   char name[9];            /* Name of the library */
   boolean viewable;        /* Is this library accessible to the view? */
   char *path;              /* DOS path name to the library */
   int32 ntiles;         /* Number of tiles in the library */
   set_type tile_set;       /* Set of 'active' tiles in the library */
   vpf_projection_code projection; /* Projection of stored coord data */
   vpf_units_type units;    /* Units of the stored coordinate data */
} library_type;

/* VPF database internal structure */
typedef struct {
   char name[9];      /* Name of the VPF database */
   char *path;        /* UNIX path name to the database */
   library_type *library; /* Array of library structures for the database */
   int32  nlibraries;   /* Number of libraries in the database */
} database_type;

/* Each theme has a symbol structure associated with it.  Themes on */
/* simple feature classes just have relevant symbol information for */
/* one of the four primitive types, but complex feature themes may  */
/* have any or all of the primitive type symbols.                   */
typedef struct {
   int32 point_color;
   int32 point;
   int32 line_color;
   int32 line;
   int32 area_color;
   int32 area;
   int32 text_color;
   int32 text;
} theme_symbol_type;

/* A theme is a single entry for a view of the database.  It can be */
/* thought of as a stored query with a description and symbology.   */
/* Each theme is associated with a feature class.                   */
typedef struct {
   char *description;              /* Description of the theme */
   char *database;                 /* Source Database path */
   char *library;                  /* Source Library name */
   char *coverage;                 /* Source coverage name */
   char *fc;                       /* Feature class name for the theme */
   char *ftable;                   /* Feature table path for the fc */
   primitive_class_type primclass; /* Primitive class(es) of theme */
   char *expression;               /* Logical selection expression */
#if 0
   theme_symbol_type symbol;       /* Drawing symbol */
#endif
} theme_type;


/* View structure.  Each view is associated with a particular database */
/* and a particular library within that datbase.                       */
typedef struct {
   char name[9];             /* View name */
   database_type *database;  /* Array of Databases in the view */
   int32  ndb;		      /* Number of databases in the view */
   char *path;		     /* Directory path to the view */
   int32  nthemes;	      /* Number of themes in the view */
   theme_type *theme;        /* Array of themes */
   set_type selected;        /* Set of themes selected for display */
   set_type displayed;       /* Set of displayed themes */
   linked_list_type sellist; /* List of selected themes (ordered) */
   extent_type extent;       /* MBR of all library extents */
   double tileheight;        /* Min of all library tile heights */
   char sympath[255];        /* Symbol set path */
#if 0
   symbol_set_type sym;      /* Symbol set for the view */
#endif
} view_type;


/* Map environment information */
typedef struct {
   extent_type mapextent;           /* Current map extent */
   boolean     mapchanged;          /* Flag - has anything changed? */
   boolean     mapdisplayed;        /* Flag - has the map been displayed? */
   boolean     user_escape;         /* Flag - has the user hit escape? */
   boolean     study_area_selected; /* Flag - study area selected? */
   boolean     latlongrid;          /* Flag - lat-lon grid to be displayed?*/
   boolean     scale_bar;           /* Flag - scale bar to be displayed? */
   vpf_projection_type projection;  /* Current map display projection */
   coord_units_type distance_unit;  /* Units for distance display */
   coord_units_type scale_bar_unit; /* Units for scale bar display */
   vpf_units_type   locator_unit;   /* Units for map locate display */
} map_environment_type;

typedef struct {
  extent_type  	mapextent;	/* Current map extent */
  boolean	mapdisplayed;	/* Flag - has the map been displayed? */
  boolean	latlongrid;	/* Flag - lat-lon grid to be displayed? */
  boolean	tilegrid;	/* Flag - tile boundaries to be displayed? */
  boolean	points;		/* Flag - libref points to be displayed? */
  boolean	text;		/* Flag - libref text to be displayed? */
  vpf_units_type locator_unit;  /* Units for libref map locate display */
} libref_map_environment_type;

/* Window specifier flags */
#define COVERAGE_WINDOW 0
#define LIBREF_WINDOW 1 

/* Functions: */
/* main() */

#endif
