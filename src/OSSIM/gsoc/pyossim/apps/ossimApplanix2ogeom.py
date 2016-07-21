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

def pyossimApplanix2ogeom(argc,argv):

        
    cameraFile     = ossimFilename(argv[1])
    eoFile         = ossimFilename(argv[2])
    outputDir  = ossimFilename()
    maxIdx = argc -1
    if argc > 4:
        outputDir = ossimFilename(argv[argc -1])
        if not outputDir.isDir():
            maxIdx = maxIdx - 1
            if not outputDir.createDirectory():
                print "Could not create directory:  " + argv[argc-1]
                sys.exit(1)
         
      
    outputDirOverrideFlag = outputDir.exists()
    print "camera file:       " + cameraFile.c_str()
    print "eo file:           " + eoFile.c_str()

         
         
    kwl = ossimKeywordlist()
  
   # Open up the exterior orientation file.
    eo = ossimApplanixEOFile()
    
    if  not eo.parseFile(eoFile):
        print "Could not open:   " + eoFile.c_str()
        sys.exit(1)
        
   
    eo.indexRecordIds()
    
    if argc >4:
    
        for idx  in range(3,maxIdx):
            kwl.clear()
            # Add the eo_file keyword to the keyword list.
            kwl.add("eo_file", eoFile.c_str(), True)

            # Add the eo_file keyword to the keyword list.
            kwl.add("camera_file", cameraFile.c_str(), True)
            kwl.add("compute_gsd_flag", True, True)

            # See if the ID is in the eo file.
            imageToProcess = ossimFilename(argv[idx])
            sid = imageToProcess.fileNoExtension()
            rec = eo.getRecordGivenId(sid)
            if not rec:

                print "applanix2ogeom ERROR:"
                print "Matching id for imageToProcess not found in eo file!"
                print "file:  " + imageToProcess
                print "id:    " + sid
                print "Returning..."
                sys.exit(1)

            kwl.add("image_id", sid.c_str(), True)
            if not outputDirOverrideFlag:
                outputDir = imageToProcess.path()
                processImage(eo, kwl, sid, outputDir);
      

    elif argc <=4:


        outputDir = ossimFilename(argv[argc-1])

        if not outputDir.isDir():
            idFile = ossimFilename(outputDir.file())
            outputDir = outputDir.path()
            idFile2 = idFile.setExtension(ossimString(""))
            rec = eo.getRecordGivenId(idFile)
              
            if not rec.valid():
                # Add the eo_file keyword to the keyword list.
                kwl.add("eo_file", eoFile.c_str(), True)
                kwl.add("eo_id", idFile2.c_str(), True)

                # Add the eo_file keyword to the keyword list.
                kwl.add("camera_file", cameraFile.c_str(), True)
                kwl.add("compute_gsd_flag", True, True)
                processImage(eo, kwl, idFile, outputDir)

        
    else:

        nRecords = eo.getNumberOfRecords()

        idIdx = eo.getFieldIdx(ossimString("ID"))
        if idIdx >= 0:
            for idx  in xrange(nRecords):
                rec = eo.getRecord(idx)
                print "here"
         
                if rec.valid():
                    sid = rec[idIdx]
                    # Add the eo_file keyword to the keyword list.
                    kwl.add("eo_file", eoFile.c_str(), True)
                    kwl.add("eo_id", sid.c_str(), True)

                    # Add the eo_file keyword to the keyword list.
                    kwl.add("camera_file", cameraFile.c_str(), True)
                    kwl.add("compute_gsd_flag", True, True)

                    processImage(eo, kwl, id, outputDir)
        else:
         
            print "NO ID RECORD FOUND!!"
            sys.exit(0)
         


def processImage(eo,kwl,gid,outputDir):

    if eo.isUtmFrame():
        model = ossimApplanixUtmModel()

    else:
        model = ossimApplanixEcefModel()
     
    if not model.loadState(kwl):
        print "applanix2ogeom:processImage ERROR:"
        print "ossimApplanixEcefModel::loadState failded for id:  " + gid
        return False

      
    geomFile = ossimFilename(gid)
    geomFile.setExtension(ossimString(".geom"))
    if not outputDir == ossimFilename.NIL:
        geomFile = outputDir.dirCat(geomFile)
   

    geomKwl = ossimKeywordlist()
    model.saveState(geomKwl)

    if not geomKwl.write(geomFile.c_str()):
        print "applanix2ogeom:processImage ERROR:"
        print "Could not write file:  "  + geomFile.c_str()
        return False
   
    else:
        print "Wrote file:  " + geomFile.c_str()

    return True


if __name__ == "__main__":
    if len(sys.argv) < 4:
        print "Usage: python ossim-applanix2ogeom.py <camera_file> <exterior_orientation_file> <imageToProcess> <optional_output_directory>"
        print "Description:"
        print "ossim-applanix2ogeom Creates ossim geometry file from Applannix support data."
        sys.exit(0)
        
    ossimInit.instance().initialize()
    pyossimApplanix2ogeom(len(sys.argv),sys.argv)


