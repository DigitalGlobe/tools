
/*************************************************************************
 *
 *N  Module VPFPROP - VPF Properties Module
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This module contains functions for returning various standard
 *    properties of VPF entities such as databases, libraries, coverages,
 *    and feature classes.
 *
 *    All of the functions take character strings as arguments to
 *    identify VPF tables and directories.  These character strings
 *    must not be passed NULL value parameters.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifndef INCL_XVTH
#include <xvt.h>
#endif

#ifndef __VPF_H__
#include "vpf.h"
#endif
#ifndef __VPFPROJ_H__
#include "vpfproj.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif
#ifndef __STRFUNC_H__
#include "strfunc.h"
#endif
#ifndef H_MUSEDIR
#include "musedir.h"
#endif
#ifndef __VPFPROP_H__
#include "vpfprop.h"
#endif

#ifndef MAXINT
#define MAXINT SHRT_MAX
#endif

/* Global Variables */
char *projection_names[] = {"Decimal Degrees",
             "Albers Equal Area",
             "Azimuthal Equal Area",
             "Azimuthal Equal Distance",
             "Gnomonic",
             "Lambert Conformal Conic",
             "Lambert Equal Area",
             "Mercator",
             "Oblique Mercator",
             "Orthographic",
             "Polar Stereographic",
             "Transverse Mercator",
             "UTM",
             "Plate-Carree"};
extern void *vpf_forward_projection;
extern void *vpf_inverse_projection;


/*
Check the directory path separators against the operating system separators.
*/
#ifdef PROTO
   void vpf_check_os_path( char *path )
#else
   void vpf_check_os_path (path)
      char *path;
#endif

{
   char *p, vpf_separator, os_separator;

   vpf_separator = VPF_SEPARATOR;
   os_separator = OS_SEPARATOR;

   if (vpf_separator == os_separator) return;
   p = &path[0];
   while (*p) {
      if (*p == VPF_SEPARATOR) *p = OS_SEPARATOR;
      p++;
   }
}

/*
Set the case of the filename for the current operating system.
*/
#ifdef PROTO
   char *os_case( char *filename )
#else
   char *os_case (filename)
      char *filename;
#endif

{
   char str[255];

   strcpy(str,filename);

#ifdef _MSDOS
   return strupr(str);
#else
   return strlwr(str);
#endif
}


/* The following function is a modification of file_exists using "access" */
/* rather that "stat" to make it more portable  DGM */
#ifdef PROTO
   int32 file_exists( char *filepath )
#else
   int32 file_exists (filepath)
      char *filepath;
#endif

{
   char *copy;
   int32 found;

   if (muse_access (filepath, 00) == 0) return 1;

   found = 0;
   /* filepath not found - try a '.' extension */
   copy = (char *)xvt_malloc(strlen(filepath)+2);
   if (!copy) {
      xvt_note ("memory allocation error in vpfprop::file_exists()\n");
      return 0;
   }
   strcpy(copy,filepath);
   strcat(copy,".");
   if (muse_access(copy, 00)==0) found = 1;
   xvt_free(copy);

   return found;
}

/*************************************************************************
 *
 *N  database_library_names
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns an array of the library names in a specified VPF database.
 *     The number of library names in the array is returned in nlibs.
 *     The array of strings is dynamically allocated and each array element
 *     as well as the array itself should be freed when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    dbpath  <input>==(char *) path the the VPF database.
 *    nlibs  <output>==(int *) the number of libraries in the database.
 *    return <output>==(char **) array of library names in the database.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char **database_library_names( char *dbpath, int32 *nlibs )
#else
   char **database_library_name (dbpath, nlibs)
      char *dbpath;
      int32 *nlibs;
#endif

{
   vpf_table_type table;
   row_type row;
   int32 LIB_, n, i;
   char **libname, path[255];

   *nlibs = 0;

   strcpy(path,dbpath);
   vpf_check_os_path(path); 
   strcat(path,OS_SEPARATOR_STRING);
   strcat(path,os_case("LAT"));
   if (!file_exists(path)) return (char **)NULL; 

   table = vpf_open_table( path, disk, "rb", NULL );
   if (!table.fp) return (char **)NULL;

   LIB_ = table_pos("LIBRARY_NAME",table);
   if (LIB_ < 0) {
      xvt_note ("vpfprop::database_library_names: ");
      xvt_note ("Invalid LAT (%s) - missing LIBRARY_NAME field\n",
         dbpath);
      vpf_close_table(&table);
      return (char **)NULL;
   }

   libname = (char**)xvt_zmalloc ((size_t)table.nrows * sizeof(char*));
   if (!libname) {
      vpf_close_table(&table);
      return (char **)NULL;
   }

   *nlibs = (int32)table.nrows;

   for (i=0;i<table.nrows;i++) {
      row = read_next_row(table);
      libname[i] = (char *)get_table_element(LIB_,row,table,NULL,&n);
      free_row(row,table);
   }

   vpf_close_table(&table);

   return libname;
}


/*************************************************************************
 *
 *N  database_producer
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the name of the VPF database producer.
 *     The character string is dynamically allocated and should be freed
 *     when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    dbpath  <input>==(char *) path the the VPF database.
 *    return <output>==(char *) database producer.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char *database_producer( char *dbpath )
#else
   char *database_producer (dbpath)
      char *dbpath;
#endif

{
  char path[255], *producer=(char *)NULL;
  int32 PROD_,n;
  row_type row;
  vpf_table_type table;

  strcpy(path,dbpath);
  vpf_check_os_path(path);
  rightjust(path);
  strcat(path,OS_SEPARATOR_STRING);
  strcat(path,os_case("DHT"));
  if (!file_exists(path)) {
    xvt_note ("vpfprop::database_producer: %s not found\n",path);
    return producer;
  }

  table = vpf_open_table(path,disk,"rb",NULL);
  if (!table.fp) {
    xvt_note ("vpfprop::database_producer: Error opening %s\n",path);
    return producer;
  }

  PROD_ = table_pos("ORIGINATOR",table);
  if (PROD_ < 0) {
    xvt_note ("vpfprop::database_producer: Invalid DHT (%s) - missing ORIGINATOR field\n",
       path);
    vpf_close_table(&table);
    return producer;
  }

  row = read_next_row(table);
  producer = (char *)get_table_element(PROD_,row,table,NULL,&n);
  free_row(row,table);
  vpf_close_table(&table);

  return producer;
}

/*************************************************************************
 *
 *N  library_description
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the text description of the specified VPF library in the
 *     specified database.
 *     The character string is dynamically allocated and should be freed
 *     when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    database_path  <input>==(char *) path the the VPF database.
 *    library        <input>==(char *) library name.
 *    return        <output>==(char *) description of the library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char *library_description( char *database_path, char *library )
#else
   char *library_description (database_path, library)
      char *database_path;
      char *library;
#endif

{
  char path[255],lib[16], *description=(char *)NULL;
  int32 DESC_,n;
  vpf_table_type table;
  row_type row;

  strcpy(path,database_path);
  vpf_check_os_path(path);
  rightjust(path);
  strcat(path,OS_SEPARATOR_STRING);
  strcpy(lib,library);
  rightjust(lib);
  strcat(path,os_case(lib));
  strcat(path,OS_SEPARATOR_STRING);
  strcat(path,os_case("LHT"));
  if (!file_exists(path)) {
    xvt_note ("vpfprop::library_description: %s not found\n",path);
    return description;
  }

  table = vpf_open_table(path,disk,"rb",NULL);
  if (!table.fp) {
    xvt_note ("vpfprop::library_description: Error opening %s\n",path);
    return description;
  }

  DESC_ = table_pos("DESCRIPTION",table);
  if (DESC_ < 0) {
    xvt_note ("vpfprop::library_description: Invalid LHT (%s) - missing DESCRIPTION field\n",
       path);
    vpf_close_table(&table);
    return description;
  }

  row = read_next_row(table);
  description = (char *)get_table_element(DESC_,row,table,NULL,&n);
  free_row(row,table);
  vpf_close_table(&table);

  return description;
}

/*************************************************************************
 *
 *N  library_extent
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the geographic extent of the specified VPF library in the
 *     coordinate system for the library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    database_path <input>==(char *) path the the VPF database.
 *    library       <input>==(char *) library name.
 *    return       <output>==(extent_type) geographic extent of the library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   extent_type library_extent( char *database_path, char *library )
#else
   extent_type library_extent (database_path, library)
      char *database_path, *library;
#endif

{
   static extent_type extent = {0.0,0.0,0.0,0.0};
   char path[255], *lib, found;
   int32 LIB_, XMIN_, YMIN_, XMAX_, YMAX_, i, n;
   float x1, y1, x2, y2;
   vpf_table_type table;
   row_type row;

   strcpy(path,database_path);
   vpf_check_os_path(path);
   rightjust(path);
   strcat(path,OS_SEPARATOR_STRING);
   strcat(path,os_case("LAT"));
   if (!file_exists(path)) {
      xvt_note ("vpfprop::library_extent: %s not found\n",path);
      return extent;
   }

   table = vpf_open_table(path,disk,"rb",NULL);
   if (!table.fp) {
      xvt_note ("vpfprop::library_extent: Error opening %s\n",path);
      return extent;
   }

   LIB_ = table_pos("LIBRARY_NAME",table);
   if (LIB_ < 0) {
      xvt_note ("vpfprop::library_extent: Invalid LAT (%s) - missing LIBRARY_NAME field\n",
              path);
      vpf_close_table(&table);
      return extent;
   }

   XMIN_ = table_pos("XMIN",table);
   if (XMIN_ < 0) {
      xvt_note ("vpfprop::library_extent: Invalid LAT (%s) - missing XMIN field\n",
         path);
      vpf_close_table(&table);
      return extent;
   }

   YMIN_ = table_pos("YMIN",table);
   if (YMIN_ < 0) {
      xvt_note ("vpfprop::library_extent: Invalid LAT (%s) - missing YMIN field\n",
         path);
      vpf_close_table(&table);
      return extent;
   }

   XMAX_ = table_pos("XMAX",table);
   if (XMAX_ < 0) {
      xvt_note ("vpfprop::library_extent: Invalid LAT (%s) - missing XMAX field\n",
         path);
      vpf_close_table(&table);
      return extent;
   }

   YMAX_ = table_pos("YMAX",table);
   if (YMAX_ < 0) {
      xvt_note ("vpfprop::library_extent: Invalid LAT (%s) - missing YMAX field\n",
         path);
      vpf_close_table(&table);
      return extent;
   }

   found = 0;
   for (i=0;i<table.nrows;i++) {
      row = read_next_row(table);
      lib = (char *)get_table_element(LIB_,row,table,NULL,&n);
      rightjust(lib);
      if (Mstrcmpi (lib,library)==0)
         {
         found = 1;
         /* get extent */
         get_table_element(XMIN_,row,table,&x1,&n);
         get_table_element(YMIN_,row,table,&y1,&n);
         get_table_element(XMAX_,row,table,&x2,&n);
         get_table_element(YMAX_,row,table,&y2,&n);
         extent.x1 = x1;
         extent.y1 = y1;
         extent.x2 = x2;
         extent.y2 = y2;
         }
      xvt_free(lib);
      free_row(row,table);
      if (found) break;
   }
   vpf_close_table(&table);

   if (!found) 
      xvt_note (
          "vpfprop::library_extent: Library %s not found for database %s\n",
          library, database_path);
            
   return extent;
}

/*************************************************************************
 *
 *N  library_security
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the security classification of the VPF library at the
 *     specified directory path.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    return       <output>==(security_type) security classification
 *                            of the library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   security_type library_security( char *library_path )
#else
   security_type library_security (library_path)
      char *library_path;
#endif

{
  char path[255],sec;
  int32 SEC_,n;
  security_type security=UNKNOWN_SECURITY;
  vpf_table_type table;
  row_type row;

  if (!library_path) {
    xvt_note ("vpfprop::library_security: no path specified\n");
    return security;
  }

  strcpy(path,library_path);
  vpf_check_os_path(path);
  rightjust(path);
  strcat(path,OS_SEPARATOR_STRING);
  strcat(path,os_case("LHT"));
  if (!file_exists(path)) {
    xvt_note ("vpfprop::library_security: %s not found\n",path);
    return security;
  }

  table = vpf_open_table(path,disk,"rb",NULL);
  if (!table.fp) {
    xvt_note ("vpfprop::library_security: Error opening %s\n",path);
    return security;
  }

  SEC_ = table_pos("SECURITY_CLASS",table);
  if (SEC_ < 0) {
    xvt_note ("vpfprop::library_security: Invalid LHT (%s) - missing SECURITY_CLASS field\n",
       path);
    vpf_close_table(&table);
    return security;
  }

  row = read_next_row(table);
  get_table_element(SEC_,row,table,&sec,&n);
  free_row(row,table);
  vpf_close_table(&table);

  switch (sec) {
    case 'U':
      security = UNCLASSIFIED;
      break;
    case 'R':
      security = RESTRICTED;
      break;
    case 'C':
      security = CONFIDENTIAL;
      break;
    case 'S':
      security = SECRET;
      break;
    case 'T':
      security = TOP_SECRET;
      break;
  }

  return security;
}


/*************************************************************************
 *
 *N  library_coverage_names
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns an array of coverage names present in the specified library.
 *     The number of coverages is returned in ncov.  The array of strings
 *     is dynamically allocated and each array element as well as the
 *     array itself should be freed when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    ncov     <output>==(int32 *) number of coverages in the library.
 *    return       <output>==(char **) array of coverage names in the
 *                                     library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char **library_coverage_names( char *library_path, int32 *ncov )
#else
   char **library_coverage_names (library_path, ncov)
      char *library_path, *ncov;
#endif

{
   char **covname = (char **)NULL;
   char path[255];
   vpf_table_type table;
   row_type row;
   int32 i,n;
   int32 COV_;

   *ncov = 0;

   strcpy(path,library_path);
   rightjust(path);
   if (path[strlen(path)-1] != OS_SEPARATOR)
      strcat(path,OS_SEPARATOR_STRING);
   strcat(path,os_case("CAT"));
   if (!file_exists(path)) {
      xvt_note ("vpfprop::library_coverage_names: ");
      xvt_note ("Invalid VPF library (%s) - CAT missing\n",library_path);
      return covname;
   }

   table = vpf_open_table(path,disk,"rb",NULL);
   if (!table.fp) {
      xvt_note ("vpfprop::library_coverage_names: ");
      xvt_note ("Error opening %s\n",path);
      return covname;
   }

   COV_ = table_pos("COVERAGE_NAME",table);
   if (COV_ < 0) {
      xvt_note ("vpfprop::library_coverage_names: ");
      xvt_note ("%s - Invalid CAT - missing COVERAGE_NAME field\n",
              library_path);
      vpf_close_table(&table);
      return covname;
   }

   covname = (char **)xvt_malloc ((size_t)table.nrows * sizeof (char*));
   if (!covname) {
      xvt_note ("vpfprop::library_coverage_names: ");
      xvt_note ("Memory allocation error\n");
      vpf_close_table(&table);
      return covname;
   }

   for (i=0;i<table.nrows;i++) {
      row = read_next_row(table);
      covname[i] = (char *)get_table_element(COV_,row,table,NULL,&n);
      free_row(row,table);
   }

   *ncov = (int32)table.nrows;

   vpf_close_table(&table);

   return covname;
}

/*************************************************************************
 *
 *N  library_coverage_descriptions
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns an array of coverage descriptions present in the specified
 *     library.  The number of coverages is returned in ncov.  The array of
 *     strings is dynamically allocated and each array element as well
 *     as the array itself should be freed when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    ncov     <output>==(int32 *) number of coverages in the library.
 *    return       <output>==(char **) array of coverage descriptions in
 *                                     the library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char **library_coverage_descriptions( char *library_path, int32 *ncov )
#else
   char **library_coverage_descriptions (library_path, ncov)
      char *library_path, *ncov;
#endif

{
   char **covdesc = (char **)NULL;
   char path[255];
   vpf_table_type table;
   row_type row;
   int32 i,n;
   int32 DESC_;
 
   *ncov = 0;
 
   strcpy(path,library_path);
   rightjust(path);
   if (path[strlen(path)-1] != OS_SEPARATOR)
      strcat(path,OS_SEPARATOR_STRING);
   strcat(path,os_case("CAT"));
   if (!file_exists(path)) {
      xvt_note ("vpfprop::library_coverage_descriptions: ");
      xvt_note ("Invalid VPF library (%s) - CAT missing\n",
              library_path);
      return covdesc;
   }
 
   table = vpf_open_table(path,disk,"rb",NULL);
   if (!table.fp) {
      xvt_note ("vpfprop::library_coverage_descriptions: ");
      xvt_note ("Error opening %s\n",path);
      return covdesc;
   }

   DESC_ = table_pos("DESCRIPTION",table);
   if (DESC_ < 0) {
      xvt_note ("vpfprop::library_coverage_descriptions: ");
      xvt_note ("%s - Invalid CAT - missing DESCRIPTION field\n",
              library_path);
      vpf_close_table(&table);
      return covdesc;
   }
 
   covdesc = (char**)xvt_malloc ((size_t)table.nrows * sizeof(char*));
   if (!covdesc) {
      xvt_note ("vpfprop::library_coverage_descriptions: ");
      xvt_note ("Memory allocation error\n");
      vpf_close_table(&table);
      return covdesc;
   }
 
   for (i=0;i<table.nrows;i++) {
      row = read_next_row(table);
      covdesc[i] = (char *)get_table_element(DESC_,row,table,NULL,&n);
      free_row(row,table);
   }
 
   *ncov = (int32)table.nrows;

   vpf_close_table(&table);
 
   return covdesc;
}
 
 


/*************************************************************************
 *
 *N  library_feature_class_names
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns an array of all of the feature classes in an entire library.
 *     The strings will be of the form "<cov>\<fcname>" where <cov> is the
 *     name of the coverage and <fcname> is the name of the feature class.
 *     The number of feature classes is returned in nfc.  The array of
 *     strings is dynamically allocated and each array element as well
 *     as the array itself should be freed when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    nfc      <output>==(long *) number of feature classes in the
 *                                   library.
 *    return       <output>==(char **) array of coverage name\feature
 *                                     class name pairs in the library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char **library_feature_class_names( char *library_path, int32 *nfc )
#else
   char **library_feature_class_names (library_path, nfc)
      char *library_path;
      int32 *nfc;
#endif

{
  int32 ncov,i,j,k,n;
  char path[255];
  char **ptr, **coverages, **subset, **fcnames=(char **)NULL;

  *nfc=0;

  strcpy(path,library_path);
  vpf_check_os_path(path);
  rightjust(path);
  if (!file_exists(path)) {
    xvt_note ("vpfprop::library_feature_class_names: %s not found\n",
       path);
    return fcnames;
  }

  coverages = library_coverage_names( path, &ncov );
  if (ncov == 0) {
    xvt_note ("vpfprop::library_feature_class_names: No coverages in library %s\n",
       path);
    return fcnames;
  }

  for (i=0;i<ncov;i++) {
    rightjust(coverages[i]);
  }

  fcnames = (char **)xvt_malloc(sizeof(char *));
  if (!fcnames) {
    xvt_note ("vpfprop::library_feature_class_names: Memory allocation error\n");
    return fcnames;
  }

  for (i=0;i<ncov;i++) {
    subset = coverage_feature_class_names(path,coverages[i],&n);
    if (!subset) continue;
    for (j=0;j<n;j++) {
      rightjust(subset[j]);
    }
    *nfc += n;
    ptr = (char**)realloc (fcnames, (size_t)(sizeof (char*) * (*nfc)));
    if (!ptr) {
      xvt_note ("vpfprop::library_feature_class_names: ");
      xvt_note ("Memory allocation error\n");
      for (j=0;j<(*nfc)-n;j++) {
   xvt_free(fcnames[j]);
      }
      xvt_free ((char*)fcnames);
      *nfc = 0;
      for (j=0;j<n;j++) {
   xvt_free(subset[j]);
      }
      xvt_free ((char*)subset);
      return (char **)NULL;
    }
    fcnames = ptr;
    for (j=(*nfc)-n;j<(*nfc);j++) {
      fcnames[j] = (char *)xvt_malloc(sizeof(char)*(strlen(coverages[i])+
                  strlen(subset[j-((*nfc)-n)])+
                  2));
      if (!fcnames[j]) {
   for (k=0;k<j;k++) {
     xvt_free(fcnames[k]);
   }
   xvt_free ((char*)fcnames);
   for (k=0;k<ncov;k++) {
     xvt_free(coverages[k]);
   }
   xvt_free ((char*)coverages);
   for (k=0;k<n;k++) {
     xvt_free(subset[k]);
   }
   xvt_free ((char*)subset);
   xvt_note (
          "vpfprop::library_feature_class_names: Memory allocation error\n");
   return (char **)NULL;
      }
      sprintf (fcnames[j], "%s%c%s",
         coverages[i],VPF_SEPARATOR,subset[j-((*nfc)-n)]);
    }

    for (j=0;j<n;j++) {
      xvt_free(subset[j]);
    }
    xvt_free ((char*)subset);
  }

  for (i=0;i<ncov;i++) {
    xvt_free(coverages[i]);
  }
  xvt_free ((char*)coverages);

  return fcnames;
}

/*************************************************************************
 *
 *N  library_tile_height
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the minimum tile height in the specified library.
 *     The tile height is returned in the stored coordinate system
 *     for the library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    return       <output>==(double) minimum tile height in the library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   double library_tile_height( char *library_path )
#else
   double library_tile_height (library_path)
      char *library_path;
#endif

{
   double tileheight=0.0;
   vpf_table_type table;
   row_type row;
   char path[255], dbpath[255],*libname;
   int32 i, n;
   int32 YMIN_, YMAX_, XMIN_, XMAX_;
   double x1, y1, x2, y2;
   extent_type libextent;
   /* for dec degrees */
   vpf_projection_type libproj;
   void (*invproj)()=NULL;

   libproj = library_projection(library_path);
   set_vpf_forward_projection(libproj);
   set_vpf_inverse_projection(libproj);
#if 0
   if (libproj.code > DDS) {
      invproj = vpf_inverse_projection[libproj.code];
   }
#endif

   strcpy(path,library_path);
   vpf_check_os_path(path);
   rightjust(path);
   if (path[strlen(path)-1] != OS_SEPARATOR)
      strcat(path,OS_SEPARATOR_STRING);
   strcat(path,os_case("TILEREF"));
   strcat(path,OS_SEPARATOR_STRING);
   strcat(path,os_case("FBR"));
   if (!file_exists(path)) {
      strcpy(dbpath,library_path);
      rightjust(dbpath);
      if (dbpath[strlen(dbpath)-1] == DIR_SEPARATOR)
         dbpath[strlen(dbpath)-1] = '\0';
      i = strlen(dbpath)-1;
      while (dbpath[i] != DIR_SEPARATOR && i>= 0) i--;
      if (dbpath[i] == DIR_SEPARATOR) {
         libname = &dbpath[i+1];
         dbpath[i] = '\0';
      } else {
         libname = library_path;
         strcpy(dbpath,"");
      }
      libextent = library_extent(dbpath,libname);
      if (libproj.code > DDS) {
         invproj(&libextent.x1,&libextent.y1);
         invproj(&libextent.x2,&libextent.y2);
      }
      tileheight = (libextent.y2 - libextent.y1);
      return tileheight;
   }

   table = vpf_open_table(path,disk,"rb",NULL);
   if (!table.fp) {
      xvt_note ("vpfprop::library_tile_height - ");
      xvt_note ("Error opening %s\n",path);
      return tileheight;
   }

   YMIN_ = table_pos("YMIN",table);
   if (YMIN_ < 0) {
      xvt_note ("vpfprop:library_tile_height: ");
      xvt_note ("Invalid FBR (%s) - missing YMIN field\n",
         path);
      vpf_close_table(&table);
      return tileheight;
   }

   YMAX_ = table_pos("YMAX",table);
   if (YMAX_ < 0) {
      xvt_note ("vpfprop:library_tile_height: ");
      xvt_note ("Invalid FBR (%s) - missing YMAX field\n",
         path);
      vpf_close_table(&table);
      return tileheight;
   }

   XMIN_ = table_pos("XMIN",table);
   if (XMIN_ < 0) {
      xvt_note ("vpfprop:library_tile_height: ");
      xvt_note ("Invalid FBR (%s) - missing XMIN field\n",
         path);
      vpf_close_table(&table);
      return tileheight;
   }

   XMAX_ = table_pos("XMAX",table);
   if (XMAX_ < 0) {
      xvt_note ("vpfprop:library_tile_height: ");
      xvt_note ("Invalid FBR (%s) - missing XMAX field\n",
         path);
      vpf_close_table(&table);
      return tileheight;
   }

   tileheight = (float)MAXINT;
   for (i=1;i<=table.nrows;i++) {
      row = read_next_row(table);
      get_table_element(YMIN_,row,table,&y1,&n);
      get_table_element(YMAX_,row,table,&y2,&n);
      if (libproj.code > DDS) {
         get_table_element(XMIN_,row,table,&x1,&n);
         get_table_element(XMAX_,row,table,&x2,&n);
         libextent.x1 = x1;
         libextent.y1 = y1;
         libextent.x2 = x2;
         libextent.y2 = y2;
         invproj(&libextent.x1,&libextent.y1);
         invproj(&libextent.x2,&libextent.y2);
         x1 = libextent.x1;
         y1 = libextent.y1;
         x2 = libextent.x2;
         y2 = libextent.y2;
      }
      if ((y2-y1) < tileheight) tileheight = (y2-y1);
      free_row(row,table);
   }

   vpf_close_table(&table);

   return tileheight;
}

/*************************************************************************
 *
 *N  library_projection
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the projection information for the stored coordinates in the
 *     specified VPF library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    return       <output>==(vpf_projection_type) projection information
 *                           for the library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   vpf_projection_type library_projection( char *library_path )
#else
   vpf_projection_type library_projection (library_path)
      char *library_path;
#endif

{
   vpf_projection_type proj;
   char path[255], *buf;
   vpf_table_type table;
   row_type row;
   int32 n, col;
   float num;

   proj.code = 0;
   strcpy(proj.name,"");
   proj.parm1 = 0.0;
   proj.parm2 = 0.0;
   proj.parm3 = 0.0;
   proj.parm4 = 0.0;
   proj.units = 0;
   proj.false_easting = 0.0;
   proj.false_northing = 0.0;
   proj.forward_proj = NULL;
   proj.inverse_proj = NULL;

   strcpy(path,library_path);
   rightjust(path);
   vpf_check_os_path(path);
   if (path[strlen(path)-1] != OS_SEPARATOR)
      strcat(path,OS_SEPARATOR_STRING);
   strcat(path,os_case("GRT"));
   if (!file_exists(path)) {
      xvt_note ("vpfprop::library_projection: ");
      xvt_note ("%s not found\n",path);
      return proj;
   }

   table = vpf_open_table(path,disk,"rb",NULL);
   if (!table.fp) {
      xvt_note ("vpfprop::library_projection: Error opening %s\n",path);
      return proj;
   }

   row = read_next_row(table);

   col = table_pos("UNITS",table);
   if (col < 0) {
      xvt_note ("vpfprop::library_projection: ");
      xvt_note ("Invalid GRT (%s)- No UNITS field\n",path);
      proj.units = UNKNOWN_UNITS;
   } else {
      buf = (char *)get_table_element(col,row,table,NULL,&n);
      rightjust(buf);
      if (strcmp(buf,"000")==0) 
    proj.units = UNKNOWN_UNITS;
      else if (strcmp(buf,"001")==0)
    proj.units = METERS;
      else if (strcmp(buf,"014")==0)
    proj.units = FEET;
      else if (strcmp(buf,"019")==0)
    proj.units = DEC_DEGREES;
      else if (strcmp(buf,"021")==0)
    proj.units = INCHES;
      else if (strcmp(buf,"022")==0)
    proj.units = KILOMETERS;
      else if (strcmp(buf,"999")==0)
    proj.units = OTHER_UNITS;
      else {
    xvt_note ("vpfprop::library_projection: ");
    xvt_note ("%s -  Unknown UNITS code (%s)\n",path,buf);
    proj.units = UNKNOWN_UNITS;
      }
      xvt_free(buf);
   }

   col = table_pos("PROJECTION_CODE",table);
   if (col < 0) {
      proj.code = DDS;
      if (proj.units == UNKNOWN_UNITS) proj.units = DEC_DEGREES;
   } else {
      buf = (char *)get_table_element(col,row,table,NULL,&n);
      strupr(buf);
      if (n>2) buf[2] = '\0';
      if (strcmp(buf,"AC")==0) 
    proj.code = AC;
      else if (strcmp(buf,"AK")==0)
    proj.code = AK;
      else if (strcmp(buf,"AL")==0)
    proj.code = AL;
      else if (strcmp(buf,"GN")==0)
    proj.code = GN;
      else if (strcmp(buf,"LE")==0)
    proj.code = LE;
      else if (strcmp(buf,"LJ")==0)
    proj.code = LJ;
      else if (strcmp(buf,"MC")==0)
    proj.code = MC;
      else if (strcmp(buf,"OC")==0)
    proj.code = OC;
      else if (strcmp(buf,"OD")==0)
    proj.code = OD;
      else if (strcmp(buf,"PG")==0)
    proj.code = PG;
      else if (strcmp(buf,"TC")==0)
    proj.code = TC;
      else if (strcmp(buf,"UT")==0)
    proj.code = UT;
      else if (strcmp(buf,"  ")==0)
    proj.code = DDS;
      else {
    xvt_note ("vpfprop::library_projection: ");
    xvt_note ("%s - Unknown projection code (%s)\n",
       path, buf);
    proj.code = -1;
      }
      xvt_free(buf);
   }
   if (proj.code < 0)
      strcpy(proj.name,"Unknown");
   else
      strcpy ((char*) proj.name, (char*)projection_names[proj.code]);

#if 0
   proj.forward_proj = vpf_forward_projection[proj.code];
   proj.inverse_proj = vpf_inverse_projection[proj.code];
#endif

   if (proj.code == DDS) {
      free_row(row,table);
      vpf_close_table(&table);
      return proj;
   }

   /* parm1, parm2, parm3, parm4 */
   col = table_pos("PARAMETER1",table);
   if (col < 0) {
      xvt_note ("vpfprop::library_projection: ");
      xvt_note ("Invalid GRT (%s)- No PARAMETER1 field\n",path);
   } else {
      get_table_element(col,row,table,&num,&n);
      proj.parm1 = num;
   }
   col = table_pos("PARAMETER2",table);
   if (col < 0) {
      xvt_note ("vpfprop::library_projection: ");
      xvt_note ("Invalid GRT (%s)- No PARAMETER2 field\n",path);
   } else {
      get_table_element(col,row,table,&num,&n);
      proj.parm2 = num;
   }
   col = table_pos("PARAMETER3",table);
   if (col < 0) {
      xvt_note ("vpfprop::library_projection: ");
      xvt_note ("Invalid GRT (%s)- No PARAMETER3 field\n",path);
   } else {
      get_table_element(col,row,table,&num,&n);
      proj.parm3 = num;
   }
   col = table_pos("PARAMETER4",table);
   if (col < 0) {
      xvt_note ("vpfprop::library_projection: ");
      xvt_note ("Invalid GRT (%s)- No PARAMETER4 field\n",path);
   } else {
      get_table_element(col,row,table,&num,&n);
      proj.parm4 = num;
   }

   col = table_pos("FALSE_ORIGIN_X",table);
   if (col < 0) {
      xvt_note ("vpfprop::library_projection: ");
      xvt_note ("Invalid GRT (%s)- No FALSE_ORIGIN_X field\n",path);
   } else {
      get_table_element(col,row,table,&num,&n);
      proj.false_easting = num;
   }
   col = table_pos("FALSE_ORIGIN_Y",table);
   if (col < 0) {
      xvt_note ("vpfprop::library_projection: ");
      xvt_note ("Invalid GRT (%s)- No FALSE_ORIGIN_Y field\n",path);
   } else {
      get_table_element(col,row,table,&num,&n);
      proj.false_northing = num;
   }

   free_row(row,table);
   vpf_close_table(&table);

   return proj;
}

/*************************************************************************
 *
 *N  coverage_description
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the description of the specified coverage in the specified
 *     VPF library.  The description string is dynamically allocated and
 *     should be freed when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    coverage      <input>==(char *) coverage name.
 *    return       <output>==(char *) description of the coverage.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char *coverage_description( char *library_path, char *coverage )
#else
   char *coverage_description (library_path, coverage)
      char *library_path, *coverage;
#endif

{
  char path[255],*cov,*description=(char *)NULL;
  vpf_table_type table;
  row_type row;
  int32 COV_, DESC_, n;
  int32 found,i;

  strcpy(path,library_path);
  vpf_check_os_path(path);
  rightjust(path);
  strcat(path,OS_SEPARATOR_STRING);
  strcat(path,os_case("CAT"));
  if (!file_exists(path)) {
    xvt_note ("vpfprop::coverage_description: %s not found\n",path);
    return description;
  }

  table = vpf_open_table(path,disk,"rb",NULL);
  if (!table.fp) {
    xvt_note ("vpfprop::coverage_description: Error opening %s\n",path);
    return description;
  }

  COV_ = table_pos("COVERAGE_NAME",table);
  if (COV_ < 0) {
    xvt_note ("vpfprop::coverage_description: Invalid CAT (%s) - missing COVERAGE_NAME field\n",
       path);
    vpf_close_table(&table);
    return description;
  }

  DESC_ = table_pos("DESCRIPTION",table);
  if (DESC_ < 0) {
    xvt_note ("vpfprop::coverage_description: Invalid CAT (%s) - missing DESCRIPTION field\n",
       path);
    vpf_close_table(&table);
    return description;
  }

  found = 0;
  for (i=0;i<table.nrows;i++) {
    row = read_next_row(table);
    cov = (char *) get_table_element(COV_,row,table,NULL,&n);
    rightjust(cov);
 
    if (Mstrcmpi (cov, coverage) == 0)
       {
       found = 1;
       description = (char *)get_table_element(DESC_,row,table,NULL,&n);
       }

    xvt_free(cov);
    free_row(row,table);
    if (found) break;
  }

  vpf_close_table(&table);

  if (!found) {
    xvt_note (
       "vpfprop::coverage_description: Coverage %s not found for library %s\n",
       coverage, library_path);
  }
  return description;
}


/*************************************************************************
 *
 *N  coverage_feature_class_names
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns an array of the feature classes present in the specified
 *     coverage of the specified library.  The number of feature classes
 *     is returned in nfc.  The array of strings is dynamically allocated
 *     and each array element as well as the array itself should be freed
 *     when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    nfc      <output>==(int32 *) number of feature classes in the
 *                                   coverage.
 *    return       <output>==(char **) array of feature class names in
 *                                     the coverage.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char **coverage_feature_class_names( char *library_path, char *coverage,
                 int32 *nfc )
#else
   char **coverage_feature_class_names (library_path, coverage, nfc)
      char *library_path, coverage;
      int32 *nfc;
#endif

{
   char **fcnames = (char **)NULL, path[255], covpath[255];
   char *fc,**ptr;
   vpf_table_type table;
   row_type row;
   int32 FC_, i, j, k, n;
   int32 found;

   *nfc = 0;

   fcnames = (char **)xvt_malloc(sizeof(char *));
   if (!fcnames) {
     xvt_note (
        "vpfprop::coverage_feature_class_names: Memory allocation error\n");
     return (char **)NULL;
   }

   strcpy(covpath,library_path);
   rightjust(covpath);
   if (covpath[strlen(covpath)-1] != OS_SEPARATOR)
      strcat(covpath,OS_SEPARATOR_STRING);
   strcat(covpath,os_case(coverage));
   rightjust(covpath);
   strcat(covpath,OS_SEPARATOR_STRING);
   vpf_check_os_path(covpath);

   strcpy(path,covpath);
   strcat(path,os_case("FCS"));
   if (!file_exists(path)) {
      xvt_note ("vpfprop::coverage_feature_class_names: ");
      xvt_note ("Invalid VPF coverage (%s) - missing FCS\n",covpath);
      xvt_free ((char*)fcnames);
      return (char **)NULL;
   }
   table = vpf_open_table(path,disk,"rb",NULL);
   if (!table.fp) {
      xvt_note ("vpfprop::coverage_feature_class_names: Error opening %s\n",
         path);
      xvt_free ((char*)fcnames);
      return (char **)NULL;
   }
   FC_ = table_pos("FEATURE_CLASS",table);
   if (FC_ < 0) {
      xvt_note ("vpfprop::coverage_feature_class_names: ");
      xvt_note ("Invalid FCS (%s) - missing FEATURE_CLASS field\n",
              path);
      vpf_close_table(&table);
      xvt_free ((char*)fcnames);
      return (char **)NULL;
   }

   j = 0;
   row = read_next_row(table);
   fcnames[j] = (char *)get_table_element(FC_,row,table,NULL,&n);
   rightjust(fcnames[j]);
   free_row(row,table);

   for (i=2;i<=table.nrows;i++) {
      row = read_next_row(table);
      fc = (char *)get_table_element(FC_,row,table,NULL,&n);
      rightjust(fc);
      free_row(row,table);
      found = 0;
      for (k=j;k>=0;k--) {

    if (Mstrcmpi (fc, fcnames[k]) == 0)
       {
       found = 1;
       break;
       }
      }
      if (!found) {
    j++;
    ptr = (char**)realloc ((void*)fcnames, (size_t)(j+1) *
                            sizeof(char*));
    if (!ptr) {
       xvt_note ("vpfprop::coverage_feature_class_names: ");
       xvt_note ("Memory allocation error\n");
       for (k=(j-1);k>=0;k--) {
         xvt_free(fcnames[k]);
       }
       xvt_free ((char*)fcnames);
       vpf_close_table(&table);
       return (char **)NULL;
    }
    fcnames = ptr;
    fcnames[j] = (char *)xvt_malloc(strlen(fc)+1);
    if (!fcnames[j]) {
       xvt_note ("vpfprop::coverage_feature_class_names: ");
       xvt_note ("Memory allocation error\n");
       for (k=0;k<j;k++) xvt_free(fcnames[k]);
       xvt_free ((char*)fcnames);
       vpf_close_table(&table);
       return (char **)NULL;
    }
    strcpy(fcnames[j],fc);
      }
      xvt_free(fc);
   }
   vpf_close_table(&table);

   *nfc = (int32)j+1;

   return fcnames;
}

/*************************************************************************
 *
 *N  coverage_topology_level
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the topology level for the specified coverage in the
 *     specified VPF library.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    coverage      <input>==(char *) coverage name.
 *    return      <output>==(int32) topology level of the coverage.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   int32 coverage_topology_level( char *library_path, char *coverage )
#else
   int32 coverage_topology_level (library_path, coverage)
      char *library_path, *coverage;
#endif

{
  char path[255],*cov;
  int32 level=0, found,i;
  vpf_table_type table;
  row_type row;
  int32 COV_,LVL_,n;
 
  strcpy(path,library_path);
  vpf_check_os_path(path);
  rightjust(path);
  strcat(path,OS_SEPARATOR_STRING);
  strcat(path,os_case("CAT"));
  if (!file_exists(path)) {
    xvt_note ("vpfprop::coverage_topology_level: %s not found\n",path);
    return level;
  }

  table = vpf_open_table(path,disk,"rb",NULL);
  if (!table.fp) {
    xvt_note ("vpfprop::coverage_topology_level: Error opening %s\n",path);
    return level;
  }

  COV_ = table_pos("COVERAGE_NAME",table);
  if (COV_ < 0) {
    xvt_note ("vpfprop::coverage_topology_level: Invalid CAT (%s) - missing COVERAGE_NAME field\n",
       path);
    vpf_close_table(&table);
    return level;
  }

  LVL_ = table_pos("LEVEL",table);
  if (LVL_ < 0) {
    xvt_note ("vpfprop::coverage_topology_level: Invalid CAT (%s) - missing LEVEL field\n",
       path);
    vpf_close_table(&table);
    return level;
  }

  found = 0;
  for (i=0;i<table.nrows;i++) {
    row = read_next_row(table);
    cov = (char *) get_table_element(COV_,row,table,NULL,&n);
    rightjust(cov);

    if (Mstrcmpi (cov, coverage) == 0)
       {
       found = 1;
       get_table_element(LVL_,row,table,&level,&n);
       }

    xvt_free(cov);
    free_row(row,table);
    if (found) break;
  }

  vpf_close_table(&table);

  if (!found) {
    xvt_note (
       "vpfprop::coverage_topology_level: Coverage %s not found for library %s\n",
       coverage, library_path);
  }

  return level;
}



/*************************************************************************
 *
 *N  feature_class_description
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the text description for the specified VPF feature class.
 *     The description string is dynamically allocated and should be freed
 *     when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    coverage      <input>==(char *) coverage name.
 *    feature_class <input>==(char *) feature class name.
 *    return       <output>==(char *) feature class description string.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char *feature_class_description( char *library_path, char *coverage,
             char *feature_class )
#else
   char *feature_class_description (library_path, coverage, feature_class)
      char *library_path, *coverage, *feature_class;
#endif

{
   char *desc = (char *)NULL, path[255], *fctable, *fc;
   vpf_table_type table;
   row_type row;
   int32 FC_, DESC_, i, n;
   int32 found;

   /* First, see if an FCA table exists in the coverage */
   strcpy(path,library_path);
   rightjust(path);
   if (path[strlen(path)-1] != OS_SEPARATOR)
      strcat(path,OS_SEPARATOR_STRING);
   strcat(path,os_case(coverage));
   rightjust(path);
   strcat(path,OS_SEPARATOR_STRING);
   vpf_check_os_path(path);

   strcat(path,os_case("FCA"));
   if (file_exists(path)) {

      table = vpf_open_table(path,disk,"rb",NULL);
      if (!table.fp) {
    xvt_note ("vpfprop::feature_class_description: Error opening %s\n",
       path);
    return (char *)NULL;
      }

      FC_ = table_pos("FCLASS",table);
      if (FC_ < 0) {
    xvt_note ("vpfprop::feature_class_description: ");
    xvt_note ("Invalid FCA (%s) - missing FCLASS field\n",
       path);
    vpf_close_table(&table);
    return (char *)NULL;
      }

      DESC_ = table_pos("DESCRIPTION",table);
      if (DESC_ < 0) DESC_ = table_pos("DESCR",table);
      if (DESC_ < 0) {
    xvt_note ("vpfprop::feature_class_description: ");
    xvt_note ("Invalid FCA (%s) - missing DESCRIPTION field\n",
       path);
    vpf_close_table(&table);
    return (char *)NULL;
      }

      found = 0;
      for (i=1;i<=table.nrows;i++) {
    row = read_next_row(table);
    fc = (char *)get_table_element(FC_,row,table,NULL,&n);
    rightjust(fc);

    if (Mstrcmpi (fc, feature_class) == 0)
       {
       found = 1;
       desc = (char *)get_table_element(DESC_,row,table,NULL,&n);
       }
    xvt_free(fc);
    free_row(row,table);
    if (found) break;
      }

      vpf_close_table(&table);

      if (!found) {
    xvt_note ("vpfprop::feature_class_description: ");
    xvt_note ("Feature class (%s) not found in FCA (%s)\n",
       feature_class,path);
      }

   } else {

      /* Get description from feature class table */
      fctable = feature_class_table( library_path, coverage, feature_class );
      if (!fctable) {
    xvt_note ("vpfprop::feature_class_description: ");
    xvt_note ("Invalid feature class (%s) in coverage (%s %s)\n",
                 feature_class, library_path,coverage);
    return (char *)NULL;
      }
      if (!file_exists(fctable)) {
    xvt_note ("vpfprop::feature_class_description: ");
    xvt_note ("%s not found\n",fctable);
    xvt_free(fctable);
    return (char *)NULL;
      }

      table = vpf_open_table(fctable,disk,"rb",NULL);
      if (!table.fp) {
    xvt_note ("vpfprop::feature_class_description: ");
    xvt_note ("Error opening %s\n",fctable);
    xvt_free(fctable);
    return (char *)NULL;
      }

      xvt_free(fctable);

      desc = (char *)xvt_malloc(strlen(table.description)+1);
      if (!desc) {
    xvt_note ("vpfprop::feature_class_description: ");
    xvt_note ("Memory allocation error\n");
    return (char *)NULL;
      }

      strcpy(desc,table.description);

      vpf_close_table(&table);

   }

   return desc;
}

/*************************************************************************
 *
 *N  feature_class_table_description
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the text description for the specified feature class table.
 *     The description string is dynamically allocated and should be freed
 *     when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    fctable       <input>==(char *) path to the feature class table.
 *    return       <output>==(char *) feature class table description.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char *feature_class_table_description( char *fctable )
#else
   char *feature_class_table_description (fctable)
      char *fctable;
#endif

{
  char path[255],*description=(char *)NULL;
  vpf_table_type table;

  strcpy(path,fctable);
  vpf_check_os_path(path);
  rightjust(path);
  if (!file_exists(path)) {
    xvt_note ("vpfprop::feature_class_table_description: %s not found\n",
       path);
    return description;
  }

  if (!is_vpf_table(path)) {
    xvt_note (
            "vpfprop::feature_class_table_description: %s not a VPF table.\n",
       path);
    return description;
  }

  table = vpf_open_table(path,disk,"rb",NULL);
  if (!table.fp) {
    xvt_note ("vpfprop::feature_class_table_description: Error opening %s\n",
       path);
    return description;
  }

    description = (char*)xvt_malloc (strlen (table.description)+1);
    strcpy (description, table.description);

  vpf_close_table(&table);

  return description;
}

/*************************************************************************
 *
 *N  feature_class_table
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the feature table path for the specified VPF feature class.
 *     The description string is dynamically allocated and should be freed
 *     when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    coverage      <input>==(char *) coverage name.
 *    feature_class <input>==(char *) feature class name.
 *    return       <output>==(char *) feature class table path.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   char *feature_class_table( char *library_path, char *coverage,
            char *feature_class )
#else
   char *feature_class_table (library_path, coverage, feature_class)
      char *library_path, *coverage, *feature_class;
#endif

{
   char *fctable = (char *)NULL, path[255], covpath[255];
   char *fc, *table1;
   vpf_table_type table;
   row_type row;
   int32 TABLE1_, FC_, i,n;
   int32 fcfound, ftfound;

   fctable = (char *)xvt_malloc(255);
   if (!fctable) {
     xvt_note (
             "vpfprop::feature_class_table: Memory allocation error\n");
     return (char *)NULL;
   }

   strcpy(covpath,library_path);
   rightjust(covpath);
   if (covpath[strlen(covpath)-1] != OS_SEPARATOR)
      strcat(covpath,OS_SEPARATOR_STRING);
   strcat(covpath,os_case(coverage));
   rightjust(covpath);
   strcat(covpath,OS_SEPARATOR_STRING);
   vpf_check_os_path(covpath);

   strcpy(fctable,covpath);

   strcpy(path,covpath);
   strcat(path,os_case("FCS"));
   if (!file_exists(path)) {
      xvt_note ("vpfprop::feature_class_table: ");
      xvt_note ("Invalid VPF coverage (%s) - missing FCS\n",covpath);
      xvt_free(fctable);
      return (char *)NULL;
   }
   table = vpf_open_table(path,disk,"rb",NULL);
   if (!table.fp) {
      xvt_note ("vpfprop::feature_class_table: Error opening %s\n",
              path);
      xvt_free(fctable);
      return (char *)NULL;
   }
   FC_ = table_pos("FEATURE_CLASS",table);
   if (FC_ < 0) {
      xvt_note ("vpfprop::feature_class_table: ");
      xvt_note ("Invalid FCS (%s) - missing FEATURE_CLASS field\n",
              path);
      vpf_close_table(&table);
      xvt_free(fctable);
      return (char *)NULL;
   }
   TABLE1_ = table_pos("TABLE1",table);
   if (TABLE1_ < 0) {
      xvt_note ("vpfprop::feature_class_table: ");
      xvt_note ("Invalid FCS (%s) - missing TABLE1 field\n",
              path);
      vpf_close_table(&table);
      xvt_free(fctable);
      return (char *)NULL;
   }
   fcfound = 0;
   ftfound = 0;
   for (i=1;i<=table.nrows;i++) {
      row = read_next_row(table);
      fc = (char *)get_table_element(FC_,row,table,NULL,&n);
      rightjust(fc);
 
      if (Mstrcmpi (fc, feature_class) == 0)
         {
         fcfound = 1;
         table1 = (char*)get_table_element (TABLE1_, row, table, NULL, &n);
         rightjust (table1);
         if (is_feature (table1))
            {
            ftfound = 1;
            if (is_feature (fctable))
               {
               if (is_complex_feature (table1))
                  {
                  strcpy (fctable, covpath);
                  strcat (fctable, os_case (table1));
                  }
               }
            else
               {
               strcat (fctable, os_case (table1));
               }
            }
         xvt_free (table1);
         }
      free_row(row,table);
      xvt_free(fc);
   }
   vpf_close_table(&table);

   if (!fcfound) {
      xvt_note ("vpfprop::feature_class_table: ");
      xvt_note ("Feature class (%s) not found in FCS (%s)\n",
              feature_class,path);
      xvt_free(fctable);
      fctable = (char *)NULL;
   }

   if (!ftfound) {
      xvt_note ("vpfprop::feature_class_table: ");
      xvt_note ("(%s) - No feature table found for feature class %s\n",
              path,feature_class);
      xvt_free(fctable);
      fctable = (char *)NULL;
   }

   return fctable;
}



/*************************************************************************
 *
 *N  feature_class_primitive_type
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns the type(s) of primitive(s) associated with the specified
 *     VPF feature class.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library_path  <input>==(char *) path the the VPF library.
 *    coverage      <input>==(char *) coverage name.
 *    feature_class <input>==(char *) feature class name.
 *    return       <output>==(primitive_class_type) primitive class
 *                           structure for the feature class.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   primitive_class_type feature_class_primitive_type( char *library_path,
                     char *coverage,
                     char *feature_class )
#else
   primitive_class_type feature_class_primitive_type (library_path, coverage, feature_class)
      char *library_path, *coverage, *feature_class;
#endif

{
   static primitive_class_type pclass = {0,0,0,0,0};
   char path[255], covpath[255];
   char *fc, *table1, *table2;
   vpf_table_type table;
   row_type row;
   int32 TABLE1_, TABLE2_, FC_, i,n;
   int32 fcfound, pfound;

   strcpy(covpath,library_path);
   rightjust(covpath);
   if (covpath[strlen(covpath)-1] != OS_SEPARATOR)
      strcat(covpath,OS_SEPARATOR_STRING);
   strcat(covpath,os_case(coverage));
   rightjust(covpath);
   strcat(covpath,OS_SEPARATOR_STRING);
   vpf_check_os_path(covpath);

   strcpy(path,covpath);
   strcat(path,os_case("FCS"));
   if (!file_exists(path)) {
      xvt_note ("vpfprop::feature_class_primitive_type: ");
      xvt_note ("Invalid VPF coverage (%s) - missing FCS\n",covpath);
      return pclass;
   }
   table = vpf_open_table(path,disk,"rb",NULL);
   if (!table.fp) {
      xvt_note ("vpfprop::feature_class_primitive_type: ");
      xvt_note ("Error opening %s\n",
              path);
      return pclass;
   }
   FC_ = table_pos("FEATURE_CLASS",table);
   if (FC_ < 0) {
      xvt_note ("vpfprop::feature_class_primitive_type: ");
      xvt_note ("Invalid FCS (%s) - missing FEATURE_CLASS field\n",
              path);
      vpf_close_table(&table);
      return pclass;
   }  

   TABLE1_ = table_pos("TABLE1",table);
   if (TABLE1_ < 0) {
      xvt_note ("vpfprop::feature_class_primitive_type: ");
      xvt_note ("Invalid FCS (%s) - missing TABLE1 field\n",
              path);
      vpf_close_table(&table);
      return pclass;
   }  

   TABLE2_ = table_pos("TABLE2",table);
   if (TABLE2_ < 0) {
      xvt_note ("vpfprop::feature_class_primitive_type: ");
      xvt_note ("Invalid FCS (%s) - missing TABLE2 field\n",
              path);
      vpf_close_table(&table);
      return pclass;
   }  

   fcfound = 0;
   pfound = 0;
   for (i=1;i<=table.nrows;i++) {
      row = read_next_row(table);
      fc = (char *)get_table_element(FC_,row,table,NULL,&n);
      rightjust(fc);
  
      if (Mstrcmpi (fc, feature_class) == 0)
         {
         fcfound = 1;
         table1 = (char*)get_table_element (TABLE1_, row, table, NULL, &n);
         rightjust (table1);
         if (is_primitive (table1))
            {
            pfound = 1;
            switch (primitive_class (table1))
               {
               case EDGE:
                  pclass.edge = 1;
                  break;
               case FACE:
                  pclass.face = 1;
                  break;
               case TEXT:
                  pclass.text = 1;
                  break;
               case ENTITY_NODE:
                  pclass.entity_node = 1;
                  break;
               case CONNECTED_NODE:
                  pclass.connected_node = 1;
                  break;
               }
            }
         xvt_free (table1);
         table2 = (char*)get_table_element (TABLE2_, row, table, NULL, &n);
         rightjust (table2);
         if (is_primitive (table2))
            {
            pfound = 1;
            switch (primitive_class (table2))
               {
               case EDGE:
                  pclass.edge = 1;
                  break;
               case FACE:
                  pclass.face = 1;
                  break;
               case TEXT:
                  pclass.text = 1;
                  break;
               case ENTITY_NODE:
                  pclass.entity_node = 1;
                  break;
               case CONNECTED_NODE:
                  pclass.connected_node = 1;
                  break;
               }
            }
         xvt_free(table2);
         }
      free_row(row,table);
      xvt_free(fc);
   }
   vpf_close_table(&table);

   if (!fcfound) {
      xvt_note ("vpfprop::feature_class_primitive_type: ");
      xvt_note ("Feature class (%s) not found in FCS (%s)\n",
              feature_class,path);
   }

   if (!pfound) {
      xvt_note ("vpfprop::feature_class_primitive_type: ");
      xvt_note ("(%s) - No primitive table found for feature class %s\n",
              path,feature_class);
   }

   return pclass;
}


/*************************************************************************
 *
 *N  is_primitive
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Returns TRUE if the given table name is that of a valid VPF 
 *     primitive table; otherwise, returns FALSE.  Determination is
 *     based solely upon the file name.  The actual contents of the
 *     file are not checked for validity.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    tablename  <input>==(char *) name of the VPF table to check.
 *    return   <output>==(int32) TRUE if the table name specifies a VPF
 *                              primitive; otherwise, returns FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1992                      gcc
 *E
 *************************************************************************/
#ifdef PROTO
   int32 is_primitive( char *tablename )
#else
   int32 is_primitive (tablename)
      char *tablename;
#endif

{
   char *locname,*end;
   int32 len, retval=0;

   locname = (char*) xvt_zmalloc (strlen (tablename) * sizeof(char)+1);
   if (locname == (char*)NULL)
      {
      xvt_note ("vpfprop::is_primitive:  Memory allocation error");
      return FALSE;
      }

   strcpy(locname,tablename);
  
   vpf_check_os_path(locname);

   /* if OS_SEPARATOR in string, delete through last OS_SEPARATOR */
 
   end = strrchr(locname,OS_SEPARATOR);

   if (end != NULL) {                    
     end += sizeof(char);
     strcpy(locname,end);
   }
 
   /* if trailing '.' remove */
 
   len = strlen(locname);
 
   if (locname[len-1]=='.') {
     locname[len-1]='\0';
     len--;
   }
 
   strupr(locname);
 
   if (strcmp(locname,"END") == 0) retval = TRUE;
   if (strcmp(locname,"CND") == 0) retval = TRUE;
   if (strcmp(locname,"EDG") == 0) retval = TRUE;
   if (strcmp(locname,"FAC") == 0) retval = TRUE;
   if (strcmp(locname,"TXT") == 0) retval = TRUE;
 
   xvt_free(locname);

   return retval;
}


/*************************************************************************
 * 
 *N  is_simple_feature
 * 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * 
 *   Purpose: 
 *P 
 *     Returns TRUE if the given table name is that of a valid VPF 
 *     simple feature table; otherwise, returns FALSE.  Determination is
 *     based solely upon the file name.  The actual contents of the 
 *     file are not checked for validity. 
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   Parameters:
 *A 
 *    tablename  <input>==(char *) name of the VPF table to check.
 *    return   <output>==(int32) TRUE if the table name specifies a VPF
 *                              simple feature; otherwise, returns FALSE.
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   History: 
 *H 
 *    Barry Michaels    Nov 1992                      gcc 
 *E 
 *************************************************************************/
#ifdef PROTO
   int32 is_simple_feature( char *tablename )
#else
   int32 is_simple_feature (tablename)
      char *tablename;
#endif

{
   char *locname, *end;
   int32 retval=0;
 
   locname = (char*) xvt_zmalloc (strlen (tablename) * sizeof (char)+1);
   if (locname == (char*)NULL)
      {
      xvt_note ("vpfprop:is_simple_feature: Memory allocation error");
      return FALSE;
      }
 
   strcpy(locname,tablename);
   rightjust(locname);
 
   /* if '.' in string, delete through last . */
 
   end = strrchr(locname,'.');

   if (end != NULL) {
     strcpy(locname,end);
   }
 
   strupr(locname);
 
   if (strcmp(locname,".PFT")==0) retval = TRUE;
   if (strcmp(locname,".LFT")==0) retval = TRUE;
   if (strcmp(locname,".AFT")==0) retval = TRUE;
   if (strcmp(locname,".TFT")==0) retval = TRUE;
 
   xvt_free(locname);

   return retval;
}


/*************************************************************************
 * 
 *N  is_complex_feature
 * 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * 
 *   Purpose: 
 *P 
 *     Returns TRUE if the given table name is that of a valid VPF 
 *     complex feature table; otherwise, returns FALSE.  Determination is
 *     based solely upon the file name.  The actual contents of the 
 *     file are not checked for validity. 
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   Parameters:
 *A 
 *    tablename  <input>==(char *) name of the VPF table to check.
 *    return   <output>==(int32) TRUE if the table name specifies a VPF
 *                              complex feature; otherwise, returns FALSE.
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   History: 
 *H 
 *    Barry Michaels    Nov 1992                      gcc 
 *E 
 *************************************************************************/
#ifdef PROTO
   int32 is_complex_feature( char *tablename )
#else
   int32 is_complex_feature (tablename)
      char *tablename;
#endif

{
   char *locname, *end;
   int32 retval=0;
 
   locname = (char*)xvt_zmalloc (strlen (tablename) * sizeof(char)+1);
   if (locname == (char*)NULL)
      {
      xvt_note ("vpfprop:is_complex_feature: Memory allocation error");
      return FALSE;
      }
 
   strcpy(locname,tablename);
   rightjust(locname);
 
   /* if '.' in string, delete through last . */
 
   end = strrchr(locname,'.');

   if (end != NULL) {
     strcpy(locname,end);
   }
 
   strupr(locname);
 
   if (strcmp(locname,".CFT")==0) retval = TRUE;

   xvt_free(locname);

   return retval;
}


/*************************************************************************
 * 
 *N  is_feature
 * 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * 
 *   Purpose: 
 *P 
 *     Returns TRUE if the given table name is that of a valid VPF 
 *     feature table; otherwise, returns FALSE.  Determination is
 *     based solely upon the file name.  The actual contents of the 
 *     file are not checked for validity. 
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   Parameters:
 *A 
 *    tablename  <input>==(char *) name of the VPF table to check.
 *    return   <output>==(int32) TRUE if the table name specifies a VPF
 *                              feature; otherwise, returns FALSE.
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   History: 
 *H 
 *    Barry Michaels    Nov 1992                      gcc 
 *E 
 *************************************************************************/
#ifdef PROTO
   int32 is_feature( char *tablename )
#else
   int32 is_feature (tablename)
      char *tablename;
#endif

{
   if (is_simple_feature(tablename)) return TRUE;
   if (is_complex_feature(tablename)) return TRUE;
   return FALSE;
}


/*************************************************************************
 * 
 *N  is_join
 * 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * 
 *   Purpose: 
 *P 
 *     Returns TRUE if the given table name is that of a valid VPF 
 *     join table; otherwise, returns FALSE.  Determination is
 *     based solely upon the file name.  The actual contents of the 
 *     file are not checked for validity. 
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   Parameters:
 *A 
 *    tablename  <input>==(char *) name of the VPF table to check.
 *    return   <output>==(int32) TRUE if the table name specifies a VPF
 *                              join table; otherwise, returns FALSE.
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   History: 
 *H 
 *    Barry Michaels    Nov 1992                      gcc 
 *E 
 *************************************************************************/
#ifdef PROTO
   int32 is_join( char *tablename )
#else
   int32 is_join (tablename)
      char *tablename;
#endif

{
   char *locname, *end;
   int32 retval=0;
 
   locname = (char*)xvt_zmalloc (strlen (tablename) * sizeof(char)+1);
   if (locname == (char*)NULL)
      {
      xvt_note ("vpfprop:is_join: Memory allocation error");
      return FALSE;
      }
 
   /* if '.' in string, delete through last . */
 
   end = strrchr(tablename,'.');

   if (end != NULL) {
     strcpy(locname,end);
   }
   else {
     strcpy(locname,tablename);
   }
   rightjust(locname);
 
   strupr(locname);
 
   if (strcmp(locname,".CJT")==0) retval = TRUE;
   if (strcmp(locname,".PJT")==0) retval = TRUE;
   if (strcmp(locname,".LJT")==0) retval = TRUE;
   if (strcmp(locname,".AJT")==0) retval = TRUE;
   if (strcmp(locname,".TJT")==0) retval = TRUE;

   xvt_free(locname);

   return retval;
}


/*************************************************************************
 * 
 *N  primitive_class
 * 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * 
 *   Purpose: 
 *P 
 *     Returns the primitive class of the given VPF primitive table name.
 *     If the table name is not that of a valid VPF primitive table, 0 is 
 *     returned.  Determination is based solely upon the file name.  The 
 *     actual contents of the file are not checked for validity. 
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   Parameters:
 *A 
 *    tablename  <input>==(char *) name of the VPF table to check.
 *    return   <output>==(int32) primitive class of the table (EDGE, FACE,
 *                              ENTITY_NODE, CONNECTED_NODE, TEXT, or 0
 *                              (if tablename not a primitive)).
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   History: 
 *H 
 *    Barry Michaels    Nov 1992                      gcc 
 *E 
 *************************************************************************/
#ifdef PROTO
   int32 primitive_class( char *tablename )
#else
   int32 primitive_class (tablename)
      char *tablename;
#endif

{
   char *locname,*end;
   int32 len, retval=0;

   locname = (char*)xvt_zmalloc (strlen (tablename) * sizeof(char)+1);
   if (locname == (char*)NULL)
      {
      xvt_note ("vpfprop:primitive_class:  Memory allocation error");
      return FALSE;
      }

   strcpy(locname,tablename);
  
   vpf_check_os_path(locname);

   /* if OS_SEPARATOR in string, delete through last OS_SEPARATOR */
 
   end = strrchr(locname,OS_SEPARATOR);

   if (end != NULL) {                    
     end += sizeof(char);
     strcpy(locname,end);
   }
 
   /* if trailing '.' remove */
 
   len = strlen(locname);
 
   if (locname[len-1]=='.') {
     locname[len-1]='\0';
     len--;
   }
 
   strupr(locname);
 
   if (strcmp(locname,"END") == 0) retval = ENTITY_NODE;
   if (strcmp(locname,"CND") == 0) retval = CONNECTED_NODE;
   if (strcmp(locname,"EDG") == 0) retval = EDGE;
   if (strcmp(locname,"FAC") == 0) retval = FACE;
   if (strcmp(locname,"TXT") == 0) retval = TEXT;
 
   xvt_free(locname);

   return retval;
}


/*************************************************************************
 * 
 *N  feature_class_type
 * 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * 
 *   Purpose: 
 *P 
 *     Returns the VPF feature type for the given feature table name.
 *     If the table name is not that of a valid VPF feature table, 0 is
 *     returned.  Determination is based solely upon the file name.  
 *     The actual contents of the file are not checked for validity. 
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   Parameters:
 *A 
 *    table      <input>==(char *) name of the VPF table to check.
 *    return   <output>==(int32) VPF feature type of the table (LINE,
 *                              AREA, ANNO, POINT, COMPLEX, or 0
 *                              (if table is none of the above)).
 *E 
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  * 
 *   History: 
 *H 
 *    Barry Michaels    Nov 1992                      gcc 
 *E 
 *************************************************************************/
#ifdef PROTO
   int32 feature_class_type( char *table )
#else
   int32 feature_class_type (table)
      char *table;
#endif

{
   char *locname, *end;
   int32 retval=0;
 
   locname = (char*)xvt_zmalloc (strlen (table) * sizeof (char)+1);
   if (locname == (char*)NULL)
      {
      xvt_note ("vpfprop:feature_class_type: Memory allocation error");
      return 0;
      }
 
   strcpy(locname,table);
   rightjust(locname);
 
   /* if '.' in string, delete through last . */
 
   end = strrchr(locname,'.');

   if (end != NULL) {
     strcpy(locname,end);
   }
 
   strupr(locname);
 
   if (strcmp(locname,".PFT")==0) retval = VPFPOINTS;
   if (strcmp(locname,".LFT")==0) retval = LINE;
   if (strcmp(locname,".AFT")==0) retval = AREA;
   if (strcmp(locname,".TFT")==0) retval = ANNO;
   if (strcmp(locname,".CFT")==0) retval = VPFCOMPLEX;
 
   xvt_free(locname);

   return retval;
}
