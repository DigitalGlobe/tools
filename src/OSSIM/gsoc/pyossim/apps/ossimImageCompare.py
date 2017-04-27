#!/usr/bin/env python
import os
import sys
try:
    ossimlib=os.environ['PYOSSIM_DIR']
except KeyError:
    print 'PyOSSIM python bindings not installed or PYOSSIM_DIR not set.'
    print 'Contact PlanetSasha developers'
    print 'https://github.com/epifanio/planetsasha'
    print 'Good Bye :('
    sys.exit(1)

pyossim_path = os.getenv('PYOSSIM_DIR')
sys.path.append(pyossim_path)

from lib.pyossim import *

def pyossimImageCompare(argc,argv):
    if argc !=3:
        print "Usage: python ossim-image-compare.py <image1> <image2>";
        sys.exit(0)

    f1 = ossimFilename(argv[1]) 
    f2 = ossimFilename(argv[2])  
    
    print "Comparing <" + f1.c_str() + "> to <" + f2.c_str() + ">..."  
    
    registry = ossimImageHandlerRegistry.instance()
    h1 = registry.open(f1)    
    if not h1.isOpen():
        print "  Could not open first image at <" + f1.c_str() +  ">. Aborting..."
        return 1
    
    h2 = registry.open(f2)
    if not h2.isOpen():
        print "Could not open second image at <" + f2.c_str() + ">. Aborting..."
        h1.close()
        return 1
        
        
    sequencer1 = ossimImageSourceSequencer(h1)
    sequencer2 = ossimImageSourceSequencer(h2)
    sequencer1.setToStartOfSequence()
    sequencer2.setToStartOfSequence()
      
    tile_count = 1
    diff_found = False

    tile1 = sequencer1.getNextTile()
    tile2 = sequencer2.getNextTile()

    if tile1.valid() and tile2.valid() and not diff_found:
        diff_found = checkTiles(tile1.get(),tile2.get())
        tile1 = sequencer1.getNextTile()
        tile2 = sequencer2.getNextTile()
        ++tile_count
     
    h1.close()
    h2.close()
    
    
    if diff_found:
        print "DIFFERENCE FOUND AT TILE " + str(tile_count)
        return 1
    else:
        print "No differences found."
    return 0   

       

def checkTiles(t1,t2):

    size1 = t1.getImageRectangle().size()
    size2 = t2.getImageRectangle().size()
    nbands1 = t1.getNumberOfBands()
    nbands2 = t2.getNumberOfBands()
    stype = t1.getScalarType()
    if size1 != size2 or nbands1 != nbands2:
        return True

    num_pixels = size1.x * size1.y
    for band in range(0,nbands1):
        if stype == PYOSSIM_UINT8 or stype == PYOSSIM_SINT8:
            buf1 = t1.getUcharBuf(band)
            buf2 = t2.getUcharBuf(band)

        elif stype == PYOSSIM_UINT16 or stype == PYOSSIM_SINT16 or PYOSSIM_USHORT11:
            buf1 = t1.getUcharBuf(band)
            buf2 = t2.getUcharBuf(band)

        elif stype == PYOSSIM_UINT32 or stype == PYOSSIM_SINT32:
            buf1 = t1.getUcharBuf(band)
            buf2 = t2.getUcharBuf(band)                

        elif stype == PYOSSIM_FLOAT32:
            buf1 = t1.getUcharBuf(band)
            buf2 = t2.getUcharBuf(band)

        elif stype == PYOSSIM_FLOAT64:
            buf1 = t1.getUcharBuf(band)
            buf2 = t2.getUcharBuf(band)

        else:
            print "This datatype is not supported. Aborting..."          
                          
        if not buf1 == buf2:
            return True

    return False
    

if __name__ == "__main__":
    init = ossimInit.instance()
    init.initialize()
    ret = pyossimImageCompare(len(sys.argv),sys.argv)


