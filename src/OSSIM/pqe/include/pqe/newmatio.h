//$$ newmatio.h           definition file for matrix package input/output

// Copyright (C) 1991,2,3,4: R B Davies

#ifndef pqe_newmatio_HEADER
#define pqe_newmatio_HEADER 1

#ifndef WANT_STREAM
#define WANT_STREAM
#endif
#include <pqe/Constants.h>
#include <pqe/newmat.h>
#include <iostream>

namespace pqe
{
   /**************************** input/output *****************************/

   PQEDLL std::ostream& operator<<(std::ostream&, const BaseMatrix&);
   
   PQEDLL std::ostream& operator<<(std::ostream&, const GeneralMatrix&);

}

#endif
