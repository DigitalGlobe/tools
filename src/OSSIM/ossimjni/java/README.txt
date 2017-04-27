//---
// File: README.txt
//---


For detailed build instruction see:  http://trac.osgeo.org/ossim/wiki/ossimjni_build

Typical first time build on linux:
Note the ossimjni/cpp side must be built first, usually with ossim cmake build.

$ sudo yum install ant ant-contrib cpptasks
$ cd ossimjni/java/
$ cp local.properties.template local.properties
$ xemacs local.properties
$ ant

Quick test:
Assumes you are in ossimjni/java directory, adjust paths and image as needed:

$ java -Djava.library.path=build/lib -cp build/lib/ossimjni.jar org.ossim.jni.apps.OssimInfo /data1/test/data/public/tif/Clinton_IA.tif
image0.band0.max_value:255
image0.band0.min_value:1
image0.band0.null_value:0
image0.entry:0
image0.geometry.decimal_degrees_per_pixel_lat:0.000134898088837533
image0.geometry.decimal_degrees_per_pixel_lon:0.000180856111100552
image0.geometry.decimations:(1,1) (0.5,0.5) (0.25,0.25) (0.125,0.125) (0.0625,0.0625) (0.03125,0.03125) (0.015625,0.015625) (0.0078125,0.0078125)
image0.geometry.gsd:(15,15)
...
...
...
tiff.image0.samples_per_pixel:1
tiff.image0.semi_major_axis:6378137
tiff.image0.semi_minor_axis:6356752.3142
tiff.image0.tile_length:256
tiff.image0.tile_width:256


