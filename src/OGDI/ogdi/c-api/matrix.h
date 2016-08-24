/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Double precision matrix inverse, allocation, multiplication,
 *          and print routines.
 * 
 ******************************************************************************
 */


/* linear algebra matrix routines */
int mat_inverse(double **matrix, int n);
    
void mat_mul_direct (double **matrix_a,int height_a,int width_a,
		    double **matrix_b,int height_b,int width_b,
		    double **matrix_result);

void mat_mul_transposed (double **matrix_a,int height_a,int width_a,
		    double **matrix_b,int height_b,int width_b,
		    double **matrix_result);

/* matrix memory allocation routine */

double **mat_malloc(int heigth, int width);

void mat_free(double **matrix, int height);
