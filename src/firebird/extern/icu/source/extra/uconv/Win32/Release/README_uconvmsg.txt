## README for "uconvmsg"'s static data (uconvmsg.LIB)
## created by pkgdata, ICU Version 3.0


To use this data in your application:

1. At the top of your source file, add the following lines:

     #include "unicode/utypes.h"
     #include "unicode/udata.h"
     U_CFUNC char uconvmsg_dat[];
2. *Early* in your application, call the following function:

     UErrorCode myError = U_ZERO_ERROR;
     udata_setAppData( "uconvmsg", (const void*) uconvmsg_dat, &myError);
     if(U_FAILURE(myError))
     {
          handle error condition ...
     }

3. Link your application against uconvmsg.LIB


4. Now, you may access this data with a 'path' of "uconvmsg" as in the following example:

     ... ures_open( "uconvmsg", "en_US", &err ); 
