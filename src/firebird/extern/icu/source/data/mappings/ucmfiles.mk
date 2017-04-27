# Copyright (c) 1999-2004, International Business Machines Corporation and
# others. All Rights Reserved.
# A list of UCM's to build
# Note: 
#
#   If you are thinking of modifying this file, READ THIS. 
#
# Instead of changing this file [unless you want to check it back in],
# you should consider creating a 'ucmlocal.mk' file in this same directory.
# Then, you can have your local changes remain even if you upgrade or re
# configure the ICU.
#
# Example 'ucmlocal.mk' files:
#
#  * To add an additional converter to the list: 
#    _____________________________________________________
#    |  UCM_SOURCE_LOCAL =  myconverter.ucm ...
#
#  * To REPLACE the default list and only build with a few
#     converters:
#    _____________________________________________________
#    |  UCM_SOURCE = ibm-913.ucm ibm-949.ucm ibm-37.ucm
#
# If you are planning to exclude EBCDIC mappings in you data then please delete
# ucmebcdic.mk from the <icu>/source/data directory
#

UCM_SOURCE_FILES = icu-internal-25546.ucm\
ibm-942_P12A-1999.ucm\
ibm-943_P130-1999.ucm\
windows-874-2000.ucm\
windows-936-2000.ucm
