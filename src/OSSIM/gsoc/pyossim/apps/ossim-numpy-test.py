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


import numpy as np
def pyossimNumpyTest(argc,argv):
    if argc < 2:
        print_usage()

    #np.set_printoptions(threshold='nan')

    imgname = ossimFilename(argv[1]) 

    if argc < 3:
        TestNumpyRead(imgname)
    elif argv[2] == 'read':
        TestNumpyRead(imgname)
    elif argv[2] == 'write':
        TestNumpyWrite(imgname)    
    elif argv[2] == 'both':
        TestNumpyReadWrite(imgname)

    else:
        print_usage()
    


def TestNumpyReadWrite(imgname):
    

    registry = ossimImageHandlerRegistry.instance()
    handler = registry.open(imgname)    
    if not handler.isOpen():
        print "Could not open first image at <" + imgname.c_str() +  ">. Aborting..."
        sys.exit(0)




    array = ossimImageSourceAsArray(handler)

    memSource = ossimMemoryImageSource()
    stype = PYOSSIM_FLOAT32
    imdata = ossimImageData(memSource,stype,1)
    imdata.initialize()

    WriteArrayToImageData(imdata,array,0)
    outfile = "out_from_rw.jpg"
    WriteImageDataToFile(imdata,outfile)
    #raw_input()


def TestNumpyRead(imgname):

    registry = ossimImageHandlerRegistry.instance()
    handler = registry.open(imgname)    
    if not handler.isOpen():
        print "Could not open first image at <" + imgname.c_str() +  ">. Aborting..."
        sys.exit(0)

    array = ossimImageSourceAsArray(handler)

    print array[0]

    print type(array)


def TestNumpyWrite(imgname):

    memSource = ossimMemoryImageSource()
    stype = PYOSSIM_FLOAT32
    imdata = 	ossimImageData(memSource,stype,1)
    x = 64 #default tile width
    y = 64 #default tile height
    size = x*y
    #imdata.setWidth(x)
    #imdata.setHeight(y)  
    imdata.initialize()

    np_array = numpy.empty([y,x],dtype=np.float32)
    np_array.fill(180.0)

    WriteArrayToImageData(imdata,np_array,0)
    outfile = "out_from_w.jpg"
    WriteImageDataToFile(imdata,outfile)


def print_usage():

    print "Usage: python ossim-numpy-test.py <image1> <option>"
    print "Options are 'read' or 'write' or 'both'. default is read."
    sys.exit(0)

if __name__ == "__main__":
    init = ossimInit.instance()
    init.initialize()
    pyossimNumpyTest(len(sys.argv),sys.argv)


