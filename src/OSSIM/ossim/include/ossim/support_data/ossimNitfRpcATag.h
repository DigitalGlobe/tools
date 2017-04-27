//*******************************************************************
// Copyright (C) 2000 Intelligence Data Systems. 
//
// LICENSE: MIT
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
//
// Description: Nitf support class for RPC00A -
// Rational Polynomial Coefficient extension.
//
//********************************************************************
// $Id: ossimNitfRpcATag.h 23666 2015-12-14 20:01:22Z rashadkm $
#ifndef ossimNitfRpcATag_HEADER
#define ossimNitfRpcATag_HEADER 1

#include <ossim/support_data/ossimNitfRpcBase.h>

/**
 * The layout of RPC00B is the same as RPC00A.  The difference is how the
 * coefficients are used; hence, all the code is in the ossimNitfRpcBase class.
 */
class OSSIM_DLL ossimNitfRpcATag : public ossimNitfRpcBase
{
public:
   
   ossimNitfRpcATag();
   
protected:
   
TYPE_DATA   
};

#endif
