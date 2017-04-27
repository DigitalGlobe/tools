#---
# Open Source Software Image Map(OSSIM) initialization script (csh).
#---
if ( ! $?OSSIM_INSTALL_PREFIX ) then
   setenv OSSIM_INSTALL_PREFIX "/usr"
endif

if ( ( ! $?OSSIM_PREFS_FILE ) then
   setenv OSSIM_PREFS_FILE $OSSIM_INSTALL_PREFIX/share/ossim/ossim-site-preferences
endif

#---
# Stub to set path on standard install:
#---
# set ossim_bin=/usr/local/ossim/bin
# if ( "${path}" !~ *$ossim_bin* ) then
#   set path = ( $ossim_bin $path )
# endif
