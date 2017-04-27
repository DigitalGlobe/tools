#!/usr/bin/env python
import platform
import os
import subprocess
from osgeo import gdal

home = os.environ.get('HOME')
PathToOssimPreference = home+str('/ossim_preferences')
#PathToGdalGrass  = str('/Users/Shared/source/grass7/distLEO/')

def WhichPlatform():
    global platform
    platforms = platform.system()
    return platforms

def OssimImg2rr(img1,output):
    myenv = os.environ.copy()
    systemplatform = WhichPlatform()
    if systemplatform == 'Darwin':
        PathToExecutable = str('/var/tmp/XcodeBuilds/Release/')
        myenv['DYLD_FRAMEWORK_PATH'] = PathToExecutable
        #myenv['GDAL_DRIVER_PATH'] = PathToGdalGrass
    else :
        PathToExecutable = str('/usr/local/bin/')
    myenv['OSSIM_PREFS_FILE'] = PathToOssimPreference
    output = subprocess.Popen([PathToExecutable+'ossim-img2rr' ,str(img1), '-o', str(output)], stdout=subprocess.PIPE, env = myenv).communicate()[0]

def OssimCreateHisto(img1,output):
    myenv = os.environ.copy()
    systemplatform = WhichPlatform()
    if systemplatform == 'Darwin':
        PathToExecutable = str('/var/tmp/XcodeBuilds/Release/')
        myenv['DYLD_FRAMEWORK_PATH'] = PathToExecutable
        #myenv['GDAL_DRIVER_PATH'] = PathToGdalGrass
    else :
        PathToExecutable = str('/usr/local/bin/')
    myenv['OSSIM_PREFS_FILE'] = PathToOssimPreference
    output = subprocess.Popen([PathToExecutable+'ossim-create-histo' ,str(img1), '-o', str(output)], stdout=subprocess.PIPE, env = myenv).communicate()[0]

def OssimImageInfo(band1):
    myenv = os.environ.copy()
    systemplatform = WhichPlatform()
    if systemplatform == 'Darwin':
        PathToExecutable = str('/var/tmp/XcodeBuilds/Release/')
        myenv['DYLD_FRAMEWORK_PATH'] = PathToExecutable
        #myenv['GDAL_DRIVER_PATH'] = PathToGdalGrass
    else :
        PathToExecutable = str('/usr/local/bin/')
    myenv['OSSIM_PREFS_FILE'] = PathToOssimPreference
    output = subprocess.Popen([PathToExecutable+'ossim-info' ,str(band1)], stdout=subprocess.PIPE, env = myenv).communicate()[0]
    return output

def OssimBandMerge(band1,band2,band3,output):
    myenv = os.environ.copy()
    systemplatform = WhichPlatform()
    if systemplatform == 'Darwin':
        PathToExecutable = str('/var/tmp/XcodeBuilds/Release/')
        myenv['DYLD_FRAMEWORK_PATH'] = PathToExecutable
        #myenv['GDAL_DRIVER_PATH'] = PathToGdalGrass
    else :
        PathToExecutable = str('/usr/local/bin/')
    myenv['OSSIM_PREFS_FILE'] = PathToOssimPreference
    output = subprocess.Popen([PathToExecutable+'ossim-band-merge' , 'tiff_strip' ,str(band1) , str(band2) , str(band3) , str(output)],stdout=subprocess.PIPE, env = myenv).communicate()[0]

def OssimCmm(band1):
    myenv = os.environ.copy()
    systemplatform = WhichPlatform()
    if systemplatform == 'Darwin':
        PathToExecutable = str('/var/tmp/XcodeBuilds/Release/')
        myenv['DYLD_FRAMEWORK_PATH'] = PathToExecutable
        #myenv['GDAL_DRIVER_PATH'] = PathToGdalGrass
    else :
        PathToExecutable = str('/usr/local/bin/')
    myenv['OSSIM_PREFS_FILE'] = PathToOssimPreference
    output = subprocess.Popen([PathToExecutable+'ossim-cmm' ,str(band1)],stdout=subprocess.PIPE, env = myenv).communicate()[0]

def OssimHeight(x,y):
    myenv = os.environ.copy()
    systemplatform = WhichPlatform()
    if systemplatform == 'Darwin':
        PathToExecutable = str('/var/tmp/XcodeBuilds/Release/')
        myenv['DYLD_FRAMEWORK_PATH'] = PathToExecutable
        #myenv['GDAL_DRIVER_PATH'] = PathToGdalGrass
    else :
        PathToExecutable = str('/usr/local/bin/')
    myenv['OSSIM_PREFS_FILE'] = PathToOssimPreference
    output = subprocess.Popen([PathToExecutable+'ossim-height', str(x) , str(y)],stdout=subprocess.PIPE, env = myenv).communicate()[0]
    for line in output.splitlines():
        if line.startswith('Height above MSL:'):
            data = line[17:].strip()
            return data
            msl = repr(float(data))
            msl = float(msl)
            print msl
            return msl

def OssimMosaic(img1,img2,output):
    myenv = os.environ.copy()
    systemplatform = WhichPlatform()
    if systemplatform == 'Darwin':
        PathToExecutable = str('/var/tmp/XcodeBuilds/Release/')
        myenv['DYLD_FRAMEWORK_PATH'] = PathToExecutable
        #myenv['GDAL_DRIVER_PATH'] = PathToGdalGrass
    else :
        PathToExecutable = str('/usr/local/bin/')
    myenv['OSSIM_PREFS_FILE'] = PathToOssimPreference
    output = subprocess.Popen([PathToExecutable+'ossim-mosaic' ,str(img1), str(img2), str(output)],stdout=subprocess.PIPE, env = myenv).communicate()[0]

def GdalInfo(input):
    myenv = os.environ.copy()
    output = subprocess.Popen(['gdalinfo' ,str(input)], stdout=subprocess.PIPE, env = myenv).communicate()[0]
    return output

def Gdal_TranslateVrt(input,output):
    instr = 'gdal_translate -of VRT %s %s' % (input,output)
    os.system(instr)        

#### functions not yet finished #####


def makeTemplateSRTM(tile):
    template = 'igen.slave_tile_buffers: 10 \n'
    template += 'igen.tiling.type: ossimTiling \n'
    template += 'igen.tiling.tiling_distance: 1 1 \n'
    template += 'igen.tiling.tiling_distance_type: degrees \n'
    template += 'igen.tiling.delta: %s %s \n' % (tile,tile)
    template += 'igen.tiling.delta_type: total_pixels \n'
    template += 'igen.tiling.tile_name_mask: %SRTM% \n'
    template += 'igen.tiling.padding_size_in_pixels: 0 0 \n'
    template += 'object1.description: \n'
    template += 'object1.enabled:  1 \n'
    template += 'object1.id:  1 \n'
    template += 'object1.object1.description: \n'
    template += 'object1.object1.enabled:  1 \n'
    template += 'object1.object1.id:  2 \n'
    template += 'object1.object1.resampler.magnify_type:  bilinear \n'
    template += 'object1.object1.resampler.minify_type:  bilinear \n'
    template += 'object1.object1.type:  ossimImageRenderer \n'
    template += 'object1.object2.type:  ossimCastTileSourceFilter \n'
    template += 'object1.object2.scalar_type: ossim_sint16 \n'
    template += 'object1.type:  ossimImageChain \n'
    template += 'object2.type: ossimGeneralRasterWriter \n'
    template += 'object2.byte_order: big_endian \n'
    template += 'product.projection.type: ossimEquDistCylProjection \n'
    return template



def orthoigen(input,outputdir,outputname,tile):
    #apppath = os.path.abspath(os.path.dirname(sys.argv[0]))
    #workdir = os.chdir()
    #ds = gdal.Open(input)
    #geotransform = ds.GetGeoTransform()
    #xpixelsize=geotransform[1]
    #ypixelsize=geotransform[5]
    #const = 108000
    #xtile = (cont+xpixelsize)/xpixelsize
    #ytile = (cont+ypixelsize)/ypixelsize
    template = makeTemplateSRTM(tile)
    return template
    
