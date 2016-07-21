#!/usr/bin/env python
import os
import sys
try:
    ossimlib=os.environ['PYOSSIM_DIR']
    print ossimlib
except KeyError:
    print 'PyOSSIM python bindings not installed or PYOSSIM_DIR not set.'
    print 'Contact PlanetSasha developers'
    print 'https://github.com/epifanio/planetsasha'
    print 'Good Bye :('
    sys.exit(1)



pyossim_path = os.getenv('PYOSSIM_DIR')
sys.path.append(pyossim_path)

from lib.pyossim import *

def pyossimAdrgHeaderdump(argc,argv):
    if argc !=2:
        print "Usage: python ossim-adrg-dump.py <.imgfile>";
        sys.exit(0)



    # ADRG header file.
    f = ossimFilename(argv[1])

    print "Trying to open header:  "  + argv[1] 
        
    # Instantiate support data class to parse all needed header files.
    theAdrgHeader = ossimAdrgHeader(f)


    # Check for errors.
    if theAdrgHeader.errorStatus():
        print "Error in ossimAdrg header detected. "
        sys.exit(1)
   

    # Dump header to stdout.
    print theAdrgHeader

   

  
    

if __name__ == "__main__":
    ossimInit.instance().initialize()
    pyossimAdrgHeaderdump(len(sys.argv),sys.argv)

