/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Double precision matrix inverse, allocation, multiplication,
 *          and print routines.
 * 
 ******************************************************************************
 */
#include <math.h>
#include <stdlib.h>

#include "ecs.h"
#include "matrix.h"

#define EPSILON 1.0e-16

/*
 * initialize values within a matrix.
 */
void mat_init(double **matrix, int height, int width) 
{
  int i=0, j=0;
      for (i=0; i < height; i++) {
         for (j=0; j < width; j++) {
	    matrix[i][j]=0;
          } 
     }	
}

/**
 * mat_mult_direct - multiply directly two 2D matrixes
 *
 */
void mat_mul_direct (matrix_a, height_a, width_a, matrix_b, height_b, width_b, matrix_result)
    double **matrix_a; 
    int height_a; 
    int width_a;
    double **matrix_b; 
    int height_b; 
    int width_b;
    double **matrix_result; 
{

 /* initialize loop iterators */
 int i=0, j=0, k=0;

 /* initialise result matrix */
 mat_init( matrix_result, width_a, width_b );
  
 /* compute multiply of each elements into destination matrix  */
 for ( i = 0 ; i< height_a; i++ )
    for ( j = 0 ; j< width_b; j++ )
      for ( k = 0 ; k< width_a; k++ )
	matrix_result[i][j] += matrix_a[i][k]*matrix_b[k][j];
  return;
}

/**
 * mat_mult_traspose - multiply traspose of first 2D matrix 
 *                     transposed with second 2D matrix
 */
void mat_mul_transposed (matrix_a, height_a, width_a, matrix_b, height_b, width_b, matrix_result)
    double **matrix_a; 
    int height_a; 
    int width_a;
    double **matrix_b; 
    int height_b; 
    int width_b;
    double **matrix_result; 
{
 /* initialize loop iterators */
 int i=0, j=0, k=0;

 /* initialise result matrix */
 mat_init( matrix_result, width_a, width_b );

 /* compute multiply of each elements into destination matrix */
 for( i=0; i<width_a; ++i )
   for( j=0; j<width_b; ++j )
     for( k=0; k<height_a; ++k )
   	  matrix_result[i][j] += matrix_a[k][i] * matrix_b[k][j];
  return;    
}

/*
 * inverse: invert a square matrix (puts pivot elements on main diagonal).
 *          returns arg2 as the inverse of arg1.
 *
 *  This routine is based on a routine found in Andrei Rogers, "Matrix
 *  Methods in Urban and Regional Analysis", (1971), pp. 143-153.
 */
int mat_inverse (double **matrix,int n)
{
    int i, j, k, l, ir=0, ic=0 ;
    int* ipivot;
    int* itemp_0;
    int* itemp_1;
    double* pivot;
    double t;
    int ret = 1;

    ipivot = (int*) malloc( n * sizeof(int) );
    itemp_0 = (int*) malloc( n * sizeof(int) );
    itemp_1 = (int*) malloc( n * sizeof(int) );
    pivot = (double*) malloc( n * sizeof(double) );
    if( ipivot == NULL || itemp_0 == NULL || itemp_1 == NULL || pivot == NULL )
    {
        fprintf(stderr, "Memory allocation failure in mat_inverse(). \n");
        ret = -1;
        goto end;
    }

    /* initialization */
    for (i = 0; i < n; i++)
        ipivot[i] = 0;

    for (i = 0; i < n; i++)
    {
        t = 0.0;  /* search for pivot element */

        for (j = 0; j < n; j++)
        {
            if (ipivot[j] == 1) /* found pivot */
                continue;

            for (k = 0; k < n; k++)
                switch (ipivot[k]-1)
                {
                    case  0:
                        break;
                    case -1:
                        if (fabs (t) < fabs (matrix[j][k]))
                        {
                            ir = j;
                            ic = k;
                            t = matrix[j][k];
                        }
                        break;
                    case  1:
                        ret = -1;
                        goto end;
                        break;
                    default: /* shouldn't get here */
                        ret = -1;
                        goto end;
                        break;
                }
        }

        ipivot[ic] += 1;
        if (ipivot[ic] > 1) /* check for dependency */
        {
            ret = -1;
            goto end;
        }

        /* interchange rows to put pivot element on diagonal */
        if (ir != ic)
            for (l = 0; l < n; l++)
            {
                t = matrix[ir][l];
                matrix[ir][l] = matrix[ic][l];
                matrix[ic][l] = t;
            }

        itemp_0[i] = ir;
        itemp_1[i] = ic;
        pivot[i] = matrix[ic][ic];

        /* check for zero pivot */
        if (fabs (pivot[i]) < EPSILON)
        {
            ret = -1;
            goto end;
        }

        /* divide pivot row by pivot element */
        matrix[ic][ic] = 1.0;

        for (j = 0; j < n; j++)
            matrix[ic][j] /= pivot[i];

        /* reduce nonpivot rows */
        for (k = 0; k < n; k++)
            if (k != ic)
            {
                t = matrix[k][ic];
                matrix[k][ic] = 0.0;

                for (l = 0; l < n; l++)
                    matrix[k][l] -= (matrix[ic][l] * t);
            }
    }

    /* interchange columns */
    for (i = 0; i < n; i++)
    {
        l = n - i - 1;
        if (itemp_0[l] == itemp_1[l])
            continue;

        ir = itemp_0[l];
        ic = itemp_1[l];

        for (k = 0; k < n; k++)
        {
            t = matrix[k][ir];
            matrix[k][ir] = matrix[k][ic];
            matrix[k][ic] = t;
        }
    }

end:
    free(ipivot);
    free(itemp_0);
    free(itemp_1);
    free(pivot);
    return ret;
}

/*
 * allocate memory for a 2D matrix with type of double elements.
 * returns NULL on failure.
 */
double **mat_malloc(int height,int width)
{
  int i=0;
  double **matrix=NULL;

    matrix = (double **)malloc(height*sizeof(double *));
	  if(matrix == NULL) {
	        printf("Allocating memory for matrix computation pointers failed. \n");
		free(matrix);
		exit(1);
		}

    for (i=0;i < height;i++) {
        matrix[i] = (double *)malloc(width*sizeof(double));
	  if(matrix == NULL) {
	        printf("Allocating memory for matrix data failed. \n");
		free(matrix);
		exit(1);
	                     }
                    }
  /* initialize values in matrix */
  mat_init(matrix,height,width);

  /* matrix created, return it */
  return matrix;
}

/*
 * unallocate memory for a 2D matrix.
 */
void mat_free(double **matrix, int height) 
{
  int i=0;
  for (i=0; i<height; i++)
    free(matrix[i]);
  /* free root on witdh > 1 type matrix only.*/
  if (matrix)  
       free(matrix);
}
