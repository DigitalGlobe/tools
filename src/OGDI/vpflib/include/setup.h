/* SETUP_H */

#ifndef SETUP_H
#define SETUP_H

#include "vpf.h"

/* Output format types */
typedef enum {M_VEC=1, VPF, ASCII, DIG} format_type;

typedef struct
   {
   char 	           *db_path;
   char 	           *db_name;
   char 	           *lib_name;
   char 	           *cov_name;
   char 	           *fclass;
   char 	           *expression;
   vpf_feature_type feature_type;
   format_type      output_format;
   double	        x_min;
   double	        x_max;
   double	        y_min;
   double	        y_max;
   } SEL_THEME;



#endif	/* SETUP_H */
