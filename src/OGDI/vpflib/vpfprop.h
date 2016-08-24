
#ifndef __VPFPROP_H__
#define __VPFPROP_H__ 1

#ifndef __COORGEOM_H__
#include "coorgeom.h"
#endif
#ifndef __VPF_H__
#include "vpf.h"
#endif

#ifdef PROTO
   char **database_library_names( char *dbpath, int32 *nlibs );
   char *database_producer( char *dbpath );
   char *library_description( char *database_path, char *library );
   extent_type library_extent( char *database_path, char *library );
   security_type library_security( char *library_path );
   char **library_coverage_names( char *library_path, int32 *ncov );
   char **library_coverage_descriptions( char *library_path, int32 *ncov );
   char **library_feature_class_names( char *library_path, int32 *nfc );
   double library_tile_height( char *library_path );
   vpf_projection_type library_projection( char *library_path );
   char *coverage_description( char *library_path, char *coverage );
   char **coverage_feature_class_names( char *library_path, char *coverage, 
				     int32 *nfc );
   int32 coverage_topology_level( char *library_path, char *coverage );
   char *feature_class_description( char *library_path, char *coverage,
			         char *feature_class );
   char *feature_class_table_description( char *fctable );
   char *feature_class_table( char *library_path, char *coverage,
			   char *feature_class );
   primitive_class_type feature_class_primitive_type( char *library_path,
						   char *coverage,
						   char *feature_class );
   int32 is_primitive( char *tablename );
   int32 is_simple_feature( char *tablename );
   int32 is_complex_feature( char *tablename );
   int32 is_feature( char *tablename );
   int32 is_join( char *tablename );
   int32 primitive_class( char *tablename );
   int32 feature_class_type( char *table );
#else
   char **database_library_names();
   char *database_producer();
   char *library_description();
   extent_type library_extent();
   security_type library_security();
   char **library_coverage_names();
   char **library_coverage_descriptions();
   char **library_feature_class_names();
   double library_tile_height();
   vpf_projection_type library_projection();
   char *coverage_description();
   char **coverage_feature_class_names();
   int32 coverage_topology_level();
   char *feature_class_description();
   char *feature_class_table_description();
   char *feature_class_table();
   primitive_class_type feature_class_primitive_type();
   int32 is_primitive();
   int32 is_simple_feature();
   int32 is_complex_feature();
   int32 is_feature();
   int32 is_join();
   int32 primitive_class();
   int32 feature_class_type();
#endif

#endif
