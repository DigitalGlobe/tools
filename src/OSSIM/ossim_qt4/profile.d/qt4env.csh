###
# $Id: qt4env.csh 10735 2007-04-18 14:57:22Z dburken $
# For csh / tcsh users do:  source .qt4env_csh
#
# Edit QTDIR to point to your qt.
# 
###
setenv QTDIR /usr/local/Trolltech/Qt-4.2.3

if ( "${path}" !~ *$QTDIR/bin* ) then
   set path = ( $QTDIR/bin $path )
endif

if ( $?LD_LIBRARY_PATH ) then
  if ( "${LD_LIBRARY_PATH}" !~ *$QTDIR/lib* ) then
    setenv LD_LIBRARY_PATH $QTDIR/lib:$LD_LIBRARY_PATH
  endif
else
  setenv LD_LIBRARY_PATH $QTDIR/lib
endif

setenv QTINC $QTDIR/include
setenv QTLIB $QTDIR/lib
