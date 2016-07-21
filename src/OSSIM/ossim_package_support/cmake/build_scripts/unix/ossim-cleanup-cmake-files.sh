#!/bin/sh

###
# $Id: ossim-cleanup-cmake-files.sh 19754 2011-06-14 13:57:02Z dburken $
# 
# FILE:  ossim-cleanup-cmake-files.sh
#
# DESCRIPTION:
# 
# Convenience script to clean up all the files created by a cmake build.
#
# NOTE: Edit the RMCMD and FILES to do what you want.  Be careful!
###

###
# Command to delte things:
###
RMCMD="rm -rf"

###
# Files to delete:
###
# FILES="*~ bin cmake_install.cmake cmake_uninstall.cmake CMakeCache.txt CMakeFiles lib Makefile make.out oms ossim ossim_plugins packaging wms"
FILES="CMakeCache.txt CMakeFiles cmake_install.cmake cmake_uninstall.cmake Makefile"

echo -e "\nWARNING:\nThis will execute the following command:\n"
echo -e "$RMCMD $FILES\n"
echo -e "Are you sure[y/n]? \c"

# read command now stores a reply into the default build-in variable $REPLY
read

if [ $REPLY = "y" ]; then
  $RMCMD $FILES
else
  echo "Exiting on your request..."
fi
