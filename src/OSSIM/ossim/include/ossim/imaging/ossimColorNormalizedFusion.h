//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Color normalized fusion
//
//*************************************************************************
// $Id: ossimColorNormalizedFusion.h 23664 2015-12-14 14:17:27Z dburken $
#ifndef ossimColorNormalizedFusion_HEADER
#define ossimColorNormalizedFusion_HEADER
#include <ossim/imaging/ossimFusionCombiner.h>

class ossimColorNormalizedFusion : public ossimFusionCombiner
{
public:

   ossimColorNormalizedFusion();
   ossimColorNormalizedFusion(ossimObject* owner);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

protected:
   virtual ~ossimColorNormalizedFusion();
TYPE_DATA
};

#endif

