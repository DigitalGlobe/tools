#---
# Open Source Software Image Map(OSSIM) initialization script (sh).
#---
if [ -z "$OSSIM_INSTALL_PREFIX" ]; then
   export OSSIM_INSTALL_PREFIX=/usr
fi

if [ -z "$OSSIM_PREFS_FILE" ]; then
   export OSSIM_PREFS_FILE=$OSSIM_INSTALL_PREFIX/share/ossim/ossim-site-preferences
fi

#---
# Stub to set path on standard install:
#---
# ossim_bin=/usr/local/ossim/bin
# if ! echo ${PATH} | /bin/grep -q $ossim_bin ; then
#    PATH=$ossim_bin:${PATH}
# fi
# export PATH
