#ifndef THIN_DIG_H
#define THIN_DIG_H

typedef struct
   {
   double x;
   double y;
   } COORDS;

/* Prototype Definitions */
#ifdef __STDC__
void Reduca (COORDS*, int32*, COORDS*, int32*, double*, int32*);
void Reduc1 (COORDS*, int32*, COORDS*, int32*, double*, int32*);
void Reduc2 (COORDS*, int32*, COORDS*, int32*, double*, int32*);
void Xcheck (int32*, double*, double*, int32*, double*, double*, int32*,
             int32*, double*, double*, int32*, int32*);
void Plnit (double*, double*, double*, double*, double*, double*,
             int32*, int32*);
#else
   void Reduca ();
   void Reduc1 ();
   void Reduc2 ();
   void Xcheck ();
   void Plnit ();
#endif

#endif /* THIN_DIG_H */
