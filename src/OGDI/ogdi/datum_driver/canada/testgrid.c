/*
 * NAD to NAD conversion test program.
 *
 * Tom Moore, March 1997
 *
 */

#include <stdio.h>
#include "nadconv.h"

int
main(argc, argv)
     int argc;
     char **argv;
{
  double lat, lon, orig_slat, orig_slon;
  int orig_dlat, orig_dlon, orig_mlat, orig_mlon;
  NAD_DATA *nadPtr;
  N_COORD *new_coord;

  sscanf(argv[1],"%d",&orig_dlat);
  sscanf(argv[2],"%d",&orig_mlat);
  sscanf(argv[3],"%lf",&orig_slat);
  sscanf(argv[4],"%d",&orig_dlon);
  sscanf(argv[5],"%d",&orig_mlon);
  sscanf(argv[6],"%lf",&orig_slon);

  lat = orig_dlat*3600 + orig_mlat*60 + orig_slat;
  lon = orig_dlon*3600 + orig_mlon*60 + orig_slon;
  
  if ((nadPtr=NAD_Init("d:/ntv2/gridshft/ntv2_0.gsb", "NAD27", "NAD83")) == NULL) {
    return 1;
  }

  if (NAD_Forward(nadPtr, &lat, &lon) != NAD_OK)
    return 2;
  printf("forward = %f %f\n", lat, lon);
  new_coord = sdms(lat, lon);
  printf("forward = %d %d %f  %d %d %f\n", new_coord->dlatn,new_coord->mlatn,new_coord->slatn,new_coord->dlonn,new_coord->mlonn,new_coord->slonn);

  if (NAD_Reverse(nadPtr, &lat, &lon) != NAD_OK)
    return 2;
  printf("reverse = %f %f\n", lat, lon);
  new_coord = sdms(lat, lon);
  printf("reverse = %d %d %f  %d %d %f\n", new_coord->dlatn,new_coord->mlatn,new_coord->slatn,new_coord->dlonn,new_coord->mlonn,new_coord->slonn);
  

  return 0;
}
