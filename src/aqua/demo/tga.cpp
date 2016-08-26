#include "tga.h"

#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>


TGASnapShot::TGASnapShot(int w, int h)
{
  buf = new unsigned char[3 * w * h];
  width = w;
  height = h;
  num = 0;
}


TGASnapShot::~TGASnapShot(void)
{
  delete [] buf;
}


void
TGASnapShot::resize(int w, int h)
{
  delete [] buf;
  buf = new unsigned char[3 * w * h];

  width = w;
  height = h;

  //num = 0;
}


void
TGASnapShot::snap(void)
{
  FILE * f;
  char NomSnapShot[80];
  unsigned char * ptr;
  int i;

  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buf);
  //sprintf(NomSnapShot, "./video/rgr%04d.tga", num);
  sprintf(NomSnapShot, "/mnt/hole/tmp/rgr%04d.tga", num);
  //printf("%s\n", NomSnapShot);
  f = fopen(NomSnapShot, "wb");
  if (f == NULL)
    {
      puts("EnregistreTGA : Erreur ecriture fichier");
      exit(1);
    }
  //printf("isfgisig\n");
  fputc(0, f); /* Taille commentaire */
  fputc(0, f);  /* Type fichier */
  fputc(2, f);  /* Type image */
  writeshort(0, f);  /* Origine */
  writeshort(0, f);  /* longueur */
  fputc(0, f);  /* depth */
  writeshort(0, f);  /* Origine X */
  writeshort(0, f);  /* Origine Y */
  writeshort(width, f);  /* Taille X */
  writeshort(height, f);  /* Taille Y */
  fputc(24, f);  /* Taille pixel en bits */
  fputc(0, f);  /* Descripteur */
  ptr = buf;
  for(i = 0; i < height * width; i++)
    {
      unsigned char temp;
      temp = ptr[0];
      ptr[0] = ptr[2];
      ptr[2] = temp;
      ptr += 3;
    }
  fwrite(buf, 3, width * height, f);

  fclose(f);
//   printf("Image %d enregistree\r", num);
//   fflush(stdout);
  num++;
}


/****************************************************************/


void
TGASnapShot::writeshort(short n, FILE * f)
{
  fputc(n & 255, f);
  fputc(n >> 8, f);
}
