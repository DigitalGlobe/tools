//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Common code / functions.
//
//----------------------------------------------------------------------------
// $Id: Ipt.h 11216 2007-06-14 21:20:55Z dburken $

#include <pqe/Common.h>

// stores a floating point nan value
const pqe::IntFloatBitCoercion pqe::nanValue(~pqe_int64(0));
