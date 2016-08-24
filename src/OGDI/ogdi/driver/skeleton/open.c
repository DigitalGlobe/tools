/*********************************************************************

  CSOURCE_INFORMATION
  
  NAME
     open.c

  DESCRIPTION
     Implementation of skeleton open, close and rewind functions
  END_DESCRIPTION

  END_CSOURCE_INFORMATION

  Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
  Permission to use, copy, modify and distribute this software and
  its documentation for any purpose and without fee is hereby granted,
  provided that the above copyright notice appear in all copies, that
  both the copyright notice and this permission notice appear in
  supporting documentation, and that the name of L.A.S. Inc not be used 
  in advertising or publicity pertaining to distribution of the software 
  without specific, written prior permission. L.A.S. Inc. makes no
  representations about the suitability of this software for any purpose.
  It is provided "as is" without express or implied warranty.
  
  ********************************************************************/

#include "skeleton.h"

extern int dbareaqty;
extern int dblineqty;
extern int dbpointqty;
extern int dbtextqty;

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _openAreaLayer

   DESCRIPTION
      Open and initialize an area vector layer. In our case, this
      function will do nothing except initializing the index to 0.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. TRUE if the operation is a success. 
            FALSE otherwise.
   END_FUNCTION_INFORMATION

   ********************************************************************
   */

int
_openAreaLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  l->index = 0;
  l->nbfeature = dbareaqty;
  return TRUE;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _closeAreaLayer

   DESCRIPTION
      Close a skeleton area vector layer. 
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_closeAreaLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
    (void) s;
    (void) l;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _rewindAreaLayer

   DESCRIPTION
      Reset the area layer selection.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_rewindAreaLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  l->index = 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _openLineLayer

   DESCRIPTION
      Open and initialize a line vector layer. In our case, this
      function will do nothing except setting the index to 0.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. TRUE if the operation is a success. 
            FALSE otherwise.
   END_FUNCTION_INFORMATION

   ********************************************************************
   */

int
_openLineLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  l->index = 0;
  l->nbfeature = dblineqty;
  return TRUE;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _closeLineLayer

   DESCRIPTION
      Close a skeleton line vector layer. 
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_closeLineLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  (void) l;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _rewindLineLayer

   DESCRIPTION
      Reset the line layer selection.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_rewindLineLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  l->index = 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _openPointLayer

   DESCRIPTION
      Open and initialize a point vector layer. In our case, this
      function will do nothing except setting the index to 0.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. TRUE if the operation is a success. 
            FALSE otherwise.
   END_FUNCTION_INFORMATION

   ********************************************************************
   */

int
_openPointLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  l->index = 0;
  l->nbfeature = dbpointqty;
  return TRUE;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _closePointLayer

   DESCRIPTION
      Close a skeleton point vector layer. 
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_closePointLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  (void) l;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _rewindPointLayer

   DESCRIPTION
      Reset the point layer selection.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_rewindPointLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  l->index = 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _openTextLayer

   DESCRIPTION
      Open and initialize a text vector layer. In our case, this
      function will do nothing except setting the index to 0.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. TRUE if the operation is a success. 
            FALSE otherwise.
   END_FUNCTION_INFORMATION

   ********************************************************************
   */

int
_openTextLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  l->index = 0;
  l->nbfeature = dbtextqty;
  return TRUE;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _closeTextLayer

   DESCRIPTION
      Close a skeleton text vector layer. 
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_closeTextLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  (void) l;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _rewindTextLayer

   DESCRIPTION
      Reset the text layer selection.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_rewindTextLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  l->index = 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _openMatrixLayer

   DESCRIPTION
      Open and initialize a matrix vector layer. This function
      will initialize the matrix region that will be used by
      the matrix and the matrix width and height.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. TRUE if the operation is a success. 
            FALSE otherwise.
   END_FUNCTION_INFORMATION

   ********************************************************************
   */

int
_openMatrixLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;

  (void) s;

  l->index = 0;
  lpriv->matrixregion.north = 4925000.0;
  lpriv->matrixregion.south = 4918000.0;
  lpriv->matrixregion.east = 597000.0;
  lpriv->matrixregion.west = 592000.0;
  lpriv->matrixwidth = 100;
  lpriv->matrixheight = 100;
  lpriv->matrixregion.ns_res = ((lpriv->matrixregion.north - 
				 lpriv->matrixregion.south)/
				((double) lpriv->matrixwidth));
  lpriv->matrixregion.ew_res = ((lpriv->matrixregion.east - 
				 lpriv->matrixregion.west)/
				((double) lpriv->matrixheight));

  return TRUE;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _closeMatrixLayer

   DESCRIPTION
      Close a skeleton matrix vector layer. 
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_closeMatrixLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  (void) l;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _rewindMatrixLayer

   DESCRIPTION
      Reset the matrix layer selection.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void
_rewindMatrixLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  (void) s;
  l->index = 0;
}

