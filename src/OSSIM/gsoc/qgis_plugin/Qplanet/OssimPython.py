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
    output = subprocess.Popen([PathToExecutable+'img2rr' ,str(img1), '-o', str(output)], stdout=subprocess.PIPE, env = myenv).communicate()[0]

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
    output = subprocess.Popen([PathToExecutable+'create_histo' ,str(img1), '-o', str(output)], stdout=subprocess.PIPE, env = myenv).communicate()[0]

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
    output = subprocess.Popen([PathToExecutable+'image_info' ,str(band1)], stdout=subprocess.PIPE, env = myenv).communicate()[0]
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
    output = subprocess.Popen([PathToExecutable+'band_merge' , 'tiff_strip' ,str(band1) , str(band2) , str(band3) , str(output)],stdout=subprocess.PIPE, env = myenv).communicate()[0]

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
    output = subprocess.Popen([PathToExecutable+'cmm' ,str(band1)],stdout=subprocess.PIPE, env = myenv).communicate()[0]

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
    output = subprocess.Popen([PathToExecutable+'ossim_height', str(x) , str(y)],stdout=subprocess.PIPE, env = myenv).communicate()[0]
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
    output = subprocess.Popen([PathToExecutable+'mosaic' ,str(img1), str(img2), str(output)],stdout=subprocess.PIPE, env = myenv).communicate()[0]

def GdalInfo(input):
    myenv = os.environ.copy()
    output = subprocess.Popen(['gdalinfo' ,str(input)], stdout=subprocess.PIPE, env = myenv).communicate()[0]
    return output

def Gdal_TranslateVrt(input,output):
    instr = 'gdal_translate -of VRT %s %s' % (input,output)
    os.system(instr)        

#### functions not yet finished #####

def orthoigen(input,outputdir,outputname,tile):
    apppath = os.path.abspath(os.path.dirname(sys.argv[0]))
    workdir = os.chdir()
    ds = gdal.Open(input)
    geotransform = ds.GetGeoTransform()
    xpixelsize=geotransform[1]
    ypixelsize=geotransform[5]
    const = 108000
    xtile = (cont+xpixelsize)/xpixelsize
    ytile = (cont+ypixelsize)/ypixelsize
    template = """igen.slave_tile_buffers: 5
    igen.tiling.type: ossimTiling
    igen.tiling.tiling_distance: 1 1
    igen.tiling.tiling_distance_type: degrees
    igen.tiling.delta: %s %s
    igen.tiling.delta_type: total_pixels
    igen.tiling.padding_size_in_pixels: 0 0
    object1.description:
    object1.enabled:  1
    object1.id:  1
    object1.object1.description:  
    object1.object1.enabled:  1
    object1.object1.id:  2
    object1.object1.resampler.magnify_type:  bilinear
    object1.object1.resampler.minify_type:  bilinear
    object1.object1.type:  ossimImageRenderer
    object1.object2.type:  ossimCastTileSourceFilter
    object1.object2.scalar_type: ossim_sint16
    object1.type:  ossimImageChain
    object2.type: ossimGeneralRasterWriter
    object2.byte_order: big_endian
    object2.create_overview: false
    object2.create_histogram: false
    object2.create_external_geometry: false
    product.projection.type: ossimEquDistCylProjection""" % (xtile,ytile)
    
