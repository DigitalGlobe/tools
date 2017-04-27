# *   Copyright (C) 1997-2004, International Business Machines
# *   Corporation and others.  All Rights Reserved.
# A list of txt's to build
# Note: 
#
#   If you are thinking of modifying this file, READ THIS. 
#
# Instead of changing this file [unless you want to check it back in],
# you should consider creating a 'reslocal.mk' file in this same directory.
# Then, you can have your local changes remain even if you upgrade or re-
# configure ICU.
#
# Example 'reslocal.mk' files:
#
#  * To add an additional locale to the list: 
#    _____________________________________________________
#    |  GENRB_SOURCE_LOCAL =   myLocale.txt ...
#
#  * To REPLACE the default list and only build with a few
#     locale:
#    _____________________________________________________
#    |  GENRB_SOURCE = ar.txt ar_AE.txt en.txt de.txt zh.txt
#
#


# This is the list of locales that are built, but not considered installed in ICU.
# These are usually aliased locales or the root locale.
COLLATION_ALIAS_SOURCE =

# Please try to keep this list in alphabetical order
COLLATION_SOURCE =
