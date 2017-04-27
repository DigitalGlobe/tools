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

def pyossimBandMerge(argc,argv):
    if argc < 2:
        print "Usage: python ossim-band-merge.py  --create-overview <tile_width> <output_type> <input_file1> <input_file2> ... <output_file>"
        print "tile_width (default = 32)"
        #outputWriterTypes()
        sys.exit(0)
        


    #Keyword list to initialize image writers with.
    kwl =ossimKeywordlist()
    PREFIX = "imagewriter."

    tile_width = 32
    create_overview = False

    if argv[1] == "--create-overview":
        create_overview = True
        print "Output overview building enabled."


    if int(argv[2]):
        tile_width = int(argv[2])
        if not((tile_width % 16) == 0):
            print "NOTICE: Tile width must be a multiple of 16!"
            print "Defaulting to 32"
            tile_width = 32
     
        print "Tile width set to:  " + str(tile_width)

 
    number_of_source_images = argc - 5;
    output_type = argv[3]
    
    print "Output type:" + output_type
    
    

    ihs = ossimConnectableObjectList()

    for h in range(0,number_of_source_images):
        f = ossimFilename(argv[h + 4])
        print "Input_image[" + str(h) + "]:     " +  argv[h + 4]
        im = ossimImageHandlerRegistry.instance().open(f)
        ihs.push_back(ossimConnectableObjectPtr(im))



    # Get the output file.
    output_name = argv[argc - 1]
    output_file = ossimFilename(output_name)
    print "Output file:        "  + output_name  

    kwl.add(PREFIX, "type", output_type)

    bm = ossimBandMergeSource(ihs)
    fileWriter = ossimImageWriterFactoryRegistry.instance().createWriter(kwl, PREFIX)
    

    if not fileWriter:
        print "invalid file writer"
        bm.disconnect()
        print "Error making an image writer..."
        print "Exiting application..."
        sys.exit(1)


    bm_geom = ossimKeywordlist()
    geom = bm.getImageGeometry()
    geom.saveState(bm_geom)
    
    
    geom_file = output_name + ".geom" 

    bm_geom.write(geom_file)

    fileWriter.connectMyInputTo(0, bm)
    

    if tile_width!=32:
        fileWriter.setTileSize(ossimIpt(tile_width, tile_width))
   
   
    fileWriter.open(output_file)
   
    #prog = ossimStdOutProgress(2)
    #fileWriter->addListener(&prog)

    fileWriter.setAreaOfInterest(bm.getBoundingRect())
    
    if fileWriter.canCastTo(ossimString("ossimTiffWriter")):
        print "canbe cast to ossimTiffWriter"
        if fileWriter.valid():
            try:
                fileWriter.execute()
            except:
                print "error writing tiff file"  
        
        
    elif fileWriter.canCastTo(ossimString("ossimJpegWriter")):
        print "canbe cast to ossimJpegWriter"

        fileWriter.initialize()
        try:
            fileWriter.execute()
        except:
            print "error writing jpeg file" 
        
    fileWriter.writeOverviewFile()
    fileWriter.disconnect()         

    raw_input()
  

def outputWriterTypes():
    lut = ossimImageTypeLut()
    index = 0
    otype = lut.getTableIndexString(index)

    while otype.size():
        print"\t" + otype
        ++index
        otype = lut.getTableIndexString(index)

    

if __name__ == "__main__":
    ossimInit.instance().initialize()
    pyossimBandMerge(len(sys.argv), sys.argv)


