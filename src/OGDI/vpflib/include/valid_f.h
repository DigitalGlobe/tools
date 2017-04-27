#ifndef H_VALIDFUN
#define H_VALIDFUN

/***************************************************************
@   check_integer()
****************************************************************
Check that edit field contains a valid integer with no more
than the given number of digits
*/
#if XVT_CC_PROTO
BOOLEAN check_integer(short length, char *string, BOOLEAN negs_ok,
                      int *caret_pos);
#else
BOOLEAN check_integer();
#endif
/*
Description:  Checks each character in a string and
returns TRUE if a non-digit or too many digits are found.  This
method is used for dialog box fields because E_CHAR events are not
sent or are not portable.
*/


/***************************************************************
@   check_real()
****************************************************************
Check that edit field contains a valid real number with no
more than the given total number of digits and the given number
of digits to the right of the decimal place
*/
#if XVT_CC_PROTO
BOOLEAN check_real(short width, short precision, char *string,
                   int *caret_pos);
#else
BOOLEAN check_real();
#endif
/*
Description: checks each character in a string and
returns TRUE if a non-digit or too many digits are found.  This
method is used for dialog box fields because E_CHAR events are not
sent or are not portable.
*/


#if XVT_CC_PROTO
BOOLEAN
check_for_ltr_x_error(char *mgrs);
#else
BOOLEAN
check_for_ltr_x_error();
#endif


#if XVT_CC_PROTO
BOOLEAN
check_for_non_mgrs_ellips(char *mgrs, char *isph, int input_type_code);
#else
BOOLEAN
check_for_non_mgrs_ellips();
#endif


#if XVT_CC_PROTO
BOOLEAN
check_for_ups(double sphi, int32 *izone, double *y, double *x, char *mgrs,
               int32 *iarea, double degrad);
#else
BOOLEAN
check_for_ups();
#endif


#endif

