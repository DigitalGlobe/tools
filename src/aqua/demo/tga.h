/*  Emacs mode: -*- C++ -*-  */


#ifndef TGA_H
#define TGA_H


#include <stdio.h>  //  FILE


class TGASnapShot
{
public:

  TGASnapShot(int w, int h);
  ~TGASnapShot(void);

  void resize(int w, int h);
  void snap(void);


private:

  void writeshort(short n, FILE * f);

  unsigned char * buf;

  int num;
  int width,height;
};


#endif  /*  TGA_H  */
