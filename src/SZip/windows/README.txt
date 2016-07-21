To build szip for Windows, just use the enclosed Visual Studio projects.  They
were created with VS2008 but should work fine with later versions.  We have not
included a top-level solution since one will be created automatically for you
by Visual Studio.

There are separate projects for the dynamic library (dll) and the static
library (lib).  The libraries will appear in a subdirectory named 'lib' in the
solution directory.  The intermediate object and build files will be in the
tmp directory in the project directory.

Like most standard C programs, the dynamic library will have a dependency on 
the C runtime linked to the library.  For VS2008, this is MSVCR90 or MSVCR90D
depending on whether the release or debug versions were built.