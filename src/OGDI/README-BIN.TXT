	OGDI README for Binary (Executable) Distribution
	================================================

Source Version: @VERSION@
Platform: @PLATFORM@

Manifest
--------

This tree (ogdi-@PLATFORM@-bin.@VERSION@) contains a binary distribution
of OGDI @VERSION@ for the @PLATFORM@ system (note that win32 includes Windows
98, NT, 2000 and XP).  This distribution includes ready to run executables
along with shared libraries suitable for linking other applications against.
On Windows it includes DLLs and stub libraries for linking against.  As well,
all include files required for development against the client are included.

ogdi-@PLATFORM@-bin.@VERSION@/bin/*: 
    - shared libraries (unix) or DLLs (windows).
    - executable for gltpd (OGDI gltp server).
    - executables for portmapper daemon (windows only)
    - executables for sample client programs including:
      o ogdi_import: Translate OGDI vectors to shapefiles and rasters to
                     raw binary.
      o ogdi_info: Dump information about, and contents of OGDI datastore
                   in text format to the terminal window.

ogdi-@PLATFORM@-bin.@VERSION@/include/*:
    - all include files needed for client development

ogdi-@PLATFORM@-bin.@VERSION@/lib/*:
    - client stub libraries for dlls (windows only)

Setup on Unix
-------------

Generally speaking it is necessary to add the ogdi-@PLATFORM@-bin.@VERSION@/bin
directory to the executable path, and the shared library path.  This can
often be accomplished with commands like this (assuming the distribution
is unpacked under the users home directory):

C-Shell:
 % setenv PATH $PATH:$HOME/ogdi-@PLATFORM@-bin.@VERSION@/bin
 % setenv LD_LIBRARY_PATH $HOME/ogdi-@PLATFORM@-bin.@VERSION@/bin

Bourne Shell:
 % PATH=${PATH}:$HOME/ogdi-@PLATFORM@-bin.@VERSION@/bin
 % LD_LIBRARY_PATH=$HOME/ogdi-@PLATFORM@-bin.@VERSION@/bin
 % export LD_LIBRARY_PATH

If it is desirable to "install" the programs they can usually be copied
to a central shared directory something like this:

 % cp ogdi-@PLATFORM@-bin.@VERSION@/bin/*.so /usr/local/lib
 % cp ogdi-@PLATFORM@-bin.@VERSION@/bin/{gltpd,ogdi_import,ogdi_info} /usr/local/lib

To develop a client program it should be sufficient to add the 
include directory to the include path, and the bin directory to the link
path along with the main client library.

eg.
 % gcc -I$HOME/ogdi-@PLATFORM@-bin.@VERSION@/include  myclient.c  \
       -L$HOME/ogdi-@PLATFORM@-bin.@VERSION@/bin -logdi31 -ldl -lm


Setup on Windows
----------------

OGDI Client programs can be run directly from the ogdi-@PLATFORM@-bin.@VERSION@/bin
directory from a command window, or this directory can be added to the 
search path.  At the DOS command prompt the directory can be added to the
path using a statement like the following (assuming the distribution was
unpacked under C:\):

 C:\> SET PATH=%PATH%;C:\ogdi-@PLATFORM@-bin.@VERSION@\bin

Before running gltpd.exe (the OGDI server) it is necessary to run portmap.exe.
The portmap.exe program providers "port mapper" services necessary for the RPC
protocol used by gltpd. 

To develop code it is generally necessary to add the 
C:\ogdi-@PLATFORM@-bin.@VERSION@\include directory to the include path.  This
can be accomplished with the -IC:\ogdi-@PLATFORM@-bin.@VERSION@/include
directive in NMAKE files, or in the preprocessor tab of the Visual Studio
options.  

On windows applications should be linked against the 
C:\ogdi-@PLATFORM@-bin.@VERSION@\lib\ogdi31.lib file. 

More Information
----------------

For more information on OGDI, access to the OGDI source visit:

  http://ogdi.sourceforge.net/






