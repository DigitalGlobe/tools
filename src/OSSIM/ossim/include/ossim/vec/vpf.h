//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: vpf.h 23664 2015-12-14 14:17:27Z dburken $
#ifndef VPF_HEADER
#define VPF_HEADER

#include <ossim/ossimConfig.h>

#if __OSSIM_CARBON__
#define MAXINT INT_MAX
#endif

extern "C"
{
#include <ossim/vpfutil/vpfapi.h>
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/vpfselec.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfprim.h>
#include <ossim/vpfutil/vpfmisc.h>
#include <ossim/vpfutil/vpfdisp.h>
}

#endif

