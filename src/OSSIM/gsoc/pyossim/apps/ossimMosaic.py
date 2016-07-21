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

def pyossimMosaic(argc,argv):

    if argc < 3:
        print_usage()

    kwlfile = ossimFilename(argv[1])

    if not kwlfile.exists() and argc < 5:
        print_usage()

    if argc < 3:
        print_usage()



    kwl = ossimKeywordlist()
    keywordlistSupplied = False
    optionGiven = False

   

    kwl.addFile(ossimFilename(kwlfile))
    keywordlistSupplied = True
    optionGiven         = True


    opt = argv[2] #-m val
    opt = opt.strip()
    opt = opt.upper()
    if opt not in("SIMPLE","BLEND","FEATHER"):
         opt = "SIMPLE"
          
    kwl.add("mosaic.type", opt, True)
   

    #templateFilename =ossimFilename(argv[3]) # -t val
    #outputTemplateKeywordlist(templateFilename)
    #sys.exit(0)


    #Get the input files.
    for i in range(3,argc-1):
        f = ossimFilename(argv[i])
        if f.exists():
            s = "file " + str(i)
            kwl.add(s,"filename", f.c_str(),True)

        keywordlistSupplied = True
        optionGiven         = True
   

    # Get the output file.
    outfile = ossimFilename(argv[argc-1])
    if outfile.exists():
        print "mosaic ERROR:"
        print "Ouput file " + argv[argc-1] + " exits and will not be overwritten!"
        print "\nExiting..."
        sys.exit(1)
   


    if keywordlistSupplied:
        writeMosaic(kwl, outfile)




def writeMosaic(kwl,outputFile):

    outfile = ossimFilename(outputFile)

    if outfile== "":
        outfile = "./output.tif"
    # build the image and a remapper for it.
    # if we were successful then we will add the image
    # renderer that will transform it it some output
    # product projection.

    inputSources = buildChainsFromKeywordList(kwl)
    if not inputSources == None:

        productProjection = buildProductProjection(kwl,inputSources)

        if productProjection is None:
            print "unable to create product projection"
            return False
      
        
        # now let's build up the renderers
        rendererBuild = buildRenderers(kwl, inputSources, productProjection)


        if rendererBuild is True:

            writer = ossimImageWriterFactoryRegistry.instance().createWriter(kwl,"writer.")
            # now we need to add a mosaic

            mosaic = createMosaic(kwl,inputSources)

            if not writer:
                writer = ossimTiffWriter()
                writer.setFilename(outfile)
                writer.open()


            if writer:
                if not mosaic == None:
                    writer.connectMyInputTo(0, mosaic)
                    writer.initialize()

                    mapInfo = ossimMapProjectionInfo(productProjection,  ossimDrect(mosaic.getBoundingRect()))
               
                    tempTiffPtr = ossimTiffWriter()
               
                    if tempTiffPtr and mapInfo:
                    
                        tempTiffPtr.setFilename(outfile)
                        tempTiffPtr.setProjectionInfo(mapInfo)
                        tempTiffPtr.execute()
               
                    else:
                        writer.execute()
    
                writer.disconnect()
                mosaic.disconnect()


            else:
                return False
   
    return True

def buildChainsFromKeywordList(kwl):
    index = 0
    result = kwl.getNumberOfSubstringKeys(ossimString("file[0-9]+\\.filename"))
    lookup = None
    numberOfMatches = 0;
    fileList = []
   
    while numberOfMatches < result:
   
        searchValue = "file" + str(index)
      
        filename = searchValue + ".filename"
        lookup = kwl.find(filename)
        if lookup:
            fileList.append(ossimFilename(lookup))
            numberOfMatches = numberOfMatches + 1
      
        index = index + 1


    return buildChainsFromFilelist(fileList)

def buildChainsFromFilelist(fileList):

    chains = []
    for f in fileList:
        handler = ossimImageHandlerRegistry.instance().open(f)


        if handler:
            imageChain = ossimSingleImageChain()
            imageChain.add(handler)
            chains.append(imageChain)
      
        else:
            print "Error: Unable to load image " + f.c_str()
            return None
     
    return chains


def buildProductProjection(kwl,inputSources):

    productType = kwl.find("product.type")
    productGeom = kwl.find("product.geom_file")
    result = None

    # if we don't have a product output specified
    # we will just use the first image.
    if not productType and not productGeom:

        if len(inputSources) < 1:
            return result

        geom = ossimKeywordlist()
        imageChain =  inputSources[0]
        geom = imageChain.getImageGeometry()
        result = geom.get().getProjection()

    else:
        result = ossimProjectionFactoryRegistry.instance().createProjection(kwl,"product.")

    return result


def buildRenderers(specFile, imageSources, productProjection):

    # add the renderer to each chain.  First, look for
    # the ossimImageHandler and get the image
    # geometry for this chain.  The add the ossimImageRenderer
    # to each chain and then connect it up
   
    for src in imageSources:
        imageChain = src
        geom = src.getImageGeometry()
        # now add the image/view transform to the renderer.

        if geom:
            last = imageChain.getFirstSource()
            renderer = ossimImageRenderer()
            renderer.connectMyInputTo(0, last)
            imageChain.add(renderer)
            transform = ossimImageViewProjectionTransform()
            transform.setImageGeometry(geom.get())
            viewGeom = ossimImageGeometry()
            viewGeom.setProjection(productProjection)
            # Make a copy of the view projection for each chain by passing in the object and not the pointer.
            transform.setViewGeometry(viewGeom)
            renderer.setImageViewTransform(transform)
            imageChain.initialize()
            
        else:
            return False
            
    return True

def createMosaic(kwl,inputSources):

    mosaicType = kwl.find("mosaic.type")
    mosaic = None

    if mosaicType:
        if mosaicType.upper() == "FEATHER":
            mosaic  = ossimFeatherMosaic()
        elif mosaicType.upper() == "BLEND":
            mosaic  = ossimBlendMosaic()
        else:
            mosaic  = ossimImageMosaic()
    else:
        mosaic = ossimImageMosaic()


    for src in inputSources:
        mosaic.connectMyInputTo(src)

    mosaic.initialize()

    return mosaic


def outputTemplateKeywordlist(templateFilename):

    out = 0
    """
    ofstream out(templateFilename.c_str());

   out << "file1.filename: <full path and file name>" << endl
       << "file2.filename: <full path and file name>" << endl
       << "// :\n"
       << "// :\n"
       << "// fileN: <full path and file name to the Nth file in the list>" << endl
       << "\n// currently this option has been tested\n"
       << "// with ossimTiffWriter and ossimJpegWriter\n"
       << "// writer.type: ossimTiffWriter"            << endl
       << "// writer.filename: <full path to output file>"  << endl
       << "\n// Currently, the mosaic application supports\n"
       << "// SIMPLE mosaics (ie. no blending algorithms)\n"
       << "// BLEND  for maps or layers that you want to blend together\n"
       << "// FEATHER for applying a spatial feaher along overlapped regions\n"
       << "// mosaic.type: SIMPLE"                     << endl
       << "\n// product type and projection information" << endl
       << "// is optional.  It will use the first images"<<endl
       << "// geometry information instead." << endl
       << "// product.type: "        << endl
       << "// product.meters_per_pixel_y: "       << endl
       << "// product.meters_per_pixel_x: "       << endl
       << "// product.central_meridian:   " << endl
       << "// product.origin_latitude:"    << endl;
    """
    print "Wrote file: " + templateFilename.c_str()


def print_usage():

    print "Usage: python ossim-mosaic.py  <kwl_file> <mosiac_type> <input_file1> <input_file2> <input_file...> <output_file>"
    print "kwl_file: keyword list file to load"
    print "mosaic_type:  (SIMPLE, BLEND, or FEATHER) (default=SIMPLE)"
    sys.exit(0)

if __name__ == "__main__":
    init = ossimInit.instance()
    init.initialize()
    pyossimMosaic(len(sys.argv),sys.argv)


