#!/usr/bin/env python
import sys
import os
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

import numpy as np
def pyossimIgen(argc,argv):
    if argc < 2:
        print_usage()

    #np.set_printoptions(threshold='nan')

    imgname = ossimFilename(argv[1]) 
    igen = ossimIgen()
    start=0
    stop=0
   
    ossimMpi.instance().initialize(argc, argv)
    start = ossimMpi.instance().getTime()

    kwl = ossimKeywordlist()

   
    if ossimMpi.instance().getRank() > 0:
        # then it will set the keyword list form the master so set this to empty
        igen.initialize(ossimKeywordlist())

    elif argc > 1:
        if kwl.addFile(argv[1]):
            thumbnail_res = argv[2]
            if thumbnail_res != '':
                kwl.add("igen.thumbnail", "true", True)
                kwl.add("igen.thumbnail_res", thumbnail_res, True)
            else:
                kwl.add("igen.thumbnail", "false", True)

            kwl.add("igen.thumbnail_res", thumbnail_res, True)
         
            igen.initialize(kwl)


    igen.outputProduct()

   
    if ossimMpi.instance().getRank() == 0:
        stop = ossimMpi.instance().getTime()
        print "Time elapsed: " + str(staop-start)
        

    ossimMpi.instance().finalize()


def print_usage():

    print "Usage: python ossim-igen.py <image1> <option>"
    print "Options are 'read' or 'write' or 'both'. default is read."
    sys.exit(0)

if __name__ == "__main__":
    init = ossimInit.instance()
    init.initialize()
    pyossimIgen(len(sys.argv),sys.argv)


