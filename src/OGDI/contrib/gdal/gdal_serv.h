/**********************************************************************
 * $Id: gdal_serv.h,v 1.1 2001/05/04 03:13:35 warmerda Exp $
 *
 * Project:  GDAL OGDI Server
 * Purpose:  Declarations.
 * Author:   Frank Warmerda, warmerda@home.com
 *
 **********************************************************************
 * Copyright (c) 2000, Frank Warmerdam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************
 * 
 * $Log: gdal_serv.h,v $
 * Revision 1.1  2001/05/04 03:13:35  warmerda
 * New
 *
 * Revision 1.2  2000/08/28 20:21:47  warmerda
 * minimally operational
 *
 * Revision 1.1  2000/08/23 19:40:17  warmerda
 * New
 *
 */

#ifndef GDAL_SERV_H
#define GDAL_SERV_H

#include "ecs.h"
#include "gdalbridge.h"

typedef struct {
    int             nBand;
    GDALRasterBandH hBand;

    int		    nOGDIImageType;	/* for Image type only */
    int		    nGDALImageType;	/* for Image type only */

    double 	    dfMatrixOffset;	
    double          dfMatrixScale;
} LayerPrivateData;

typedef struct {
    GDALDatasetH 	hDS;
    double     		adfGeoTransform[6];
    char                *pszProjection;
} ServerPrivateData;

#endif
