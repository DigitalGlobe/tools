#!/usr/bin/env python
import os
import sys
try:
    ossimlib= os.environ['PYOSSIM_DIR']
    #print ossimlib

except KeyError:
    print 'PyOSSIM python bindings not installed or PYOSSIM_DIR not set.'
    print 'Contact PlanetSasha developers'
    print 'https://github.com/epifanio/planetsasha'
    print 'Good Bye :('
    sys.exit(1)



pyossim_path = os.getenv('PYOSSIM_DIR')
sys.path.append(pyossim_path)
try:
    from lib.pyossim import *
except ImportError:
    
    sys.path.append(ossimlib + '/lib') 
    from pyossim import * 
import grass.script as grass
import grass.script.array as garray

import numpy as np
def pyossimGrass(argc,argv):
    if argc < 3:
        print_usage()

    #np.set_printoptions(threshold='nan')
    if argv[2] == '-tograss':
        OssimToGrass(argv[1])
    elif argv[2] == '-toossim':
        GrassToOssim(argv[1])    
    else:
        print_usage()
    


def OssimToGrass(filename):
   
    registry = ossimImageHandlerRegistry.instance()
    ossimfile = ossimFilename(filename) 
    handler = registry.open(ossimfile)    
    if not handler.isOpen():
        print "Could not open first image at <" + imgname.c_str() +  ">. Aborting..."
        sys.exit(0)

    ossimdata = ossimImageSourceAsArray(handler)
    base=os.path.basename(filename)
    grass_file = os.path.splitext(base)[0]

    for band in xrange(ossimdata.shape[0]):

        gdata = garray.array()
        gdata[...] = ossimdata[band][:gdata.shape[0],:gdata.shape[1]]
        grass_band = grass_file + "." + str(band+1)
        gdata.write(grass_band)


def GrassToOssim(mapname):
    registry = ossimImageHandlerRegistry.instance()
    # read map
    array = garray.array()
    array.read(mapname)
 
    ##print grass.raster_info(mapname)['datatype']

    memSource = ossimMemoryImageSource()
    stype = PYOSSIM_FLOAT32
    imdata = ossimImageData(memSource,stype,1)
    memSource.initialize()
    WriteArrayToImageData(imdata,array,0)
    outfile = mapname +".jpg"
    WriteImageDataToFile(imdata,outfile)

def print_usage():

    print "Usage: python ossim-grass.py <image1> <option>"
    print "Options are '-tograss' or '-tossim'. default is read."
    print "Example: ossim-grass.py <grass-raster-name> -toossim"
    print "Example: ossim-grass.py <image-file-name> -tograss"
    sys.exit(0)

if __name__ == "__main__":
    init = ossimInit.instance()
    init.initialize()
    pyossimGrass(len(sys.argv),sys.argv)


