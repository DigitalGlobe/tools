#------------------------------------------------------------------------------
#
# build_gdal.py
#
# Summary : Builds the GDAL Library
#
#------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
        DESCRIPTION = "Builds the GDAL library"
        _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
        _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
        _PATH_NAME_BUILD = "GDAL-1.11.5"
        _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
        _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
        _PATH_NAME_SOURCE = "..\\src\\GDAL-1.11.5"

        _PATH_NAME_BUILD_FILEGDB = "FileGDB"
        _PATH_NAME_SOURCE_FILEGDB = "..\\src\\FileGDB"

        _INLCUDE_BASE = '..\\..\\include'
        _LIB_BASE = '..\\..\\sdk\\'
        _LIB_BASE_2 = 'lib'
        
        _LIBICONV_INCLUDE = 'libiconv'
        _LIBICONV_LIB = 'libiconv'

        _JPEG_INCLUDE = 'libjpeg'
        _JPEG_LIB = 'libjpeg'

        _PNG_INCLUDE = 'libpng'
        _PNG_LIB = 'libpng'
        
        _LIBKML_INCLUDE = ['kml', '.']
        _LIBKML_LIBS = ['libkmlbase', 'libkmlconvenience', 'libkmldom', 'libkmlengine', 'libkmlregionator', 'libkmlxsd', 'minizip', 'libexpat', 'uriparser', 'zlib']
        
        _TIFF_INCLUDE = 'libtiff'
        _TIFF_LIB = 'libtiff'

        _GEOTIFF_INCLUDE = 'libgeotiff'
        _GEOTIFF_LIB = 'libgeotiff'

        _OGDI_INCLUDE = 'ogdi'
        _OGDI_LIB = 'libogdi'

        _EXPAT_INCLUDE = 'expat'
        _EXPAT_LIB = 'libexpat'

        _XERCES_INCLUDE = 'xerces'
        _XERCES_LIB = 'xerces-c'

        _HDF5_INCLUDE = 'hdf5'
        _HDF5_LIB = 'hdf5'

        _PROJ_INCLUDE = 'proj4'
        _PROJ_LIB = 'proj'
        
        _CURL_INCLUDE = '.'
        _CURL_LIB = 'libcurl'
        _CURL_LIB_2 = ['wsock32.lib', 'wldap32.lib', 'winmm.lib']

        _GEOS_INCLUDE = 'geos'
        _GEOS_LIB = 'geos'

        _PODOFO_INCLUDE = 'podofo'
        _PODOFO_LIB = ['podofo','freetype']
        _PODOFO_LIB_2 = ['gdi32.lib']
     
        # the name of the path for all include files
        _PATH_NAME_INCLUDE = '.'
        #----------------------------------------------------------------------
        # the name of the distribution path for all include files
        _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\gdal'
        #----------------------------------------------------------------------
        
        def __init__(self) :
        
            pass
        #----------------------------------------------------------------------
        

        def main(self) :
        
            systemManager = SystemManager()
            pathFinder    = PathFinder()
            
            # process command-line arguments
            buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)
            
            # initialize environment variables
            systemManager.initializeIncludeEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.initializeLibraryEnvironmentVariable( buildSettings.X64Specified() )
            systemManager.appendToPathEnvironmentVariable( pathFinder.getVisualStudioBinPathName( buildSettings.X64Specified() ), True )

            # MSBuild is under "Program Files (x86)"
            systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "MSBuild\\14.0\\Bin") )

            compileOutDir = ""
            if ( buildSettings.X64Specified() ) :
                print("64bit Build")

                # append path for 64-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x64"                 ) )
            else:
                print("32bit Build")

                # append path for 32-bit rc.exe
                systemManager.appendToPathEnvironmentVariable( os.path.join( systemManager.getProgramFilesPathName(False) , \
                                                                             "Windows Kits\\10\\bin\\x86"                 ) )

            # determine path names
            print("Getting Paths")
            buildPathName  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
            sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
        
            buildPathNameFileGDB  = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD_FILEGDB)        
            sourcePathNameFileGDB = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE_FILEGDB)

            print("build path: " + buildPathName)
            print("source path: " + sourcePathName)

            # initialize directories
            print("removing previous build dir")
            systemManager.changeDirectory(sourcePathName)
            systemManager.removeDirectory(buildPathName)
            systemManager.removeDirectory(buildPathNameFileGDB)
            
            print("copying File GDB source to build dir")
            systemManager.copyDirectory( sourcePathNameFileGDB, buildPathNameFileGDB)

            print("copying source to build dir")
            systemManager.copyDirectory( sourcePathName, buildPathName)
        
            # start building
            systemManager.changeDirectory(buildPathName)
            cmd = "nmake -f makefile.vc MSVC_VER=1900 GDAL_HOME=" + buildPathName + ' '
            cmd = cmd + "FGDB_SDK=" + buildPathNameFileGDB + " "
           
            if buildSettings.ReleaseSpecified():
                cmd += ' GDALSTATICLIB=gdal-static.lib'
                cmd += ' GDALDYNLIB=gdal.lib'
                cmd += ' GDAL_DLL=gdal$(VERSION).dll'
            else:
                cmd += ' GDALSTATICLIB=gdal-static_d.lib'
                cmd += ' GDALDYNLIB=gdal_d.lib'
                cmd += ' GDAL_DLL=gdal$(VERSION)_d.dll'
            
            suffix = ""
            
            # extend command line based on options
            if ( buildSettings.X64Specified() ) :
                cmd = cmd + " WIN64=YES "
                distribPath = Program._PATH_NAME_DISTRIBUTION_X64
                exePath = Program._PATH_NAME_BINARY_X64
                
                if buildSettings.ReleaseSpecified():
                    suffix = ""
                else:
                    cmd = cmd + " DEBUG=1 "
                    suffix = "_d"

            else :
                distribPath = Program._PATH_NAME_DISTRIBUTION_X86
                exePath = Program._PATH_NAME_BINARY_X86

                if buildSettings.ReleaseSpecified():
                    suffix = ""
                else:
                    cmd = cmd + " DEBUG=1 "
                    suffix = "_d"

            incpath = os.path.join(buildPathName, Program._INLCUDE_BASE)
            libpath = os.path.join(os.path.join(os.path.join(buildPathName, Program._LIB_BASE), ('x64' if buildSettings.X64Specified() else 'x86')), Program._LIB_BASE_2)
            libsuffix = suffix + '.lib'
            
            #cmd += ' LIBICONV_INCLUDE=-I' + os.path.join(incpath, Program._LIBICONV_INCLUDE) 
            #cmd += ' LIBICONV_LIB=' + os.path.join(libpath, Program._LIBICONV_LIB) + libsuffix
            #cmd += ' LIBICONV_CFLAGS=-DICONV_CONST=const'
            

            cmd += ' JPEG_LIB=' + os.path.join(libpath, Program._JPEG_LIB) + libsuffix
            cmd += ' JPEGDIR=' + os.path.join(incpath, Program._JPEG_INCLUDE) 
            cmd += ' JPEG_EXTERNAL_LIB=1'
            
            cmd += ' PNG_LIB=' + os.path.join(libpath, Program._PNG_LIB) + libsuffix
            cmd += ' PNGDIR=' + os.path.join(incpath, Program._PNG_LIB) 
            cmd += ' PNG_EXTERNAL_LIB=1'
            
            _LIBKML_INCLUDE = ['kml', '.']
            _LIBKML_LIBS = ['libkmlbase', 'libkmlconvenience', 'libkmldom', 'libkmlengine', 'libkmlregionator', 'libkmlxsd', 'minizip', 'libexpat', 'uriparser', 'zlib']
            
            cmd += ' LIBKML_INCLUDE="'
            for i in Program._LIBKML_INCLUDE:
               cmd += ' -I' + os.path.join(incpath, i)
            cmd += '"'

            cmd += ' LIBKML_LIBS="'
            for i in Program._LIBKML_LIBS:
               cmd += ' ' + os.path.join(libpath, i) + libsuffix
            cmd += '"'
                        
            cmd += ' TIFF_INC=-I' + os.path.join(incpath, Program._TIFF_INCLUDE) 
            cmd += ' TIFF_LIB=' + os.path.join(libpath, Program._TIFF_LIB) + libsuffix
            cmd += ' TIFF_OPTS=-DBIGTIFF_SUPPORT'

            cmd += ' GEOTIFF_INC=-I' + os.path.join(incpath, Program._GEOTIFF_INCLUDE) 
            cmd += ' GEOTIFF_LIB=' + os.path.join(libpath, Program._GEOTIFF_LIB) + libsuffix
            
            cmd += ' OGDI_INCLUDE=-I' + os.path.join(incpath, Program._OGDI_INCLUDE) 
            cmd += ' OGDILIB=' + os.path.join(libpath, Program._OGDI_LIB) + libsuffix
            cmd += ' OGDIVER=32'

            #cmd += ' EXPAT_INCLUDE=-I' + os.path.join(incpath, Program._EXPAT_INCLUDE) 
            #cmd += ' EXPAT_LIB=' + os.path.join(libpath, Program._EXPAT_LIB) + libsuffix

            cmd += ' XERCES_INCLUDE=-I' + os.path.join(incpath, Program._XERCES_INCLUDE) 
            cmd += ' XERCES_LIB=' + os.path.join(libpath, Program._XERCES_LIB) + libsuffix

            cmd += ' HDF5_LIB=' + os.path.join(libpath, Program._HDF5_LIB) + libsuffix
            cmd += ' HDF5_PLUGIN=NO'

            cmd += ' PROJ_INCLUDE=-I' + os.path.join(incpath, Program._PROJ_INCLUDE) 
            cmd += ' PROJ_LIBRARY=' + os.path.join(libpath, Program._PROJ_LIB) + libsuffix
            
            cmd += ' CURL_INC=-I' + os.path.join(incpath, Program._CURL_INCLUDE) 
            cmd += ' CURL_LIB="' + os.path.join(libpath, Program._CURL_LIB) + libsuffix
            for i in Program._CURL_LIB_2:
               cmd += ' ' + i
            cmd += '"'

            cmd += ' GEOS_CFLAGS="-I' + os.path.join(incpath, Program._GEOS_INCLUDE)
            cmd += ' -DHAVE_GEOS"'
            cmd += ' GEOS_LIB=' + os.path.join(libpath, Program._GEOS_LIB) + libsuffix
                        
            #cmd += ' PODOFO_CFLAGS=-I' + os.path.join(incpath, Program._PODOFO_INCLUDE)            
            #cmd += ' PODOFO_LIBS="'
            #for i in Program._PODOFO_LIB:
            #   cmd += ' ' + os.path.join(libpath, i) + libsuffix
            #for i in Program._PODOFO_LIB_2:
            #   cmd += ' ' + i
            #cmd += '"'
            #cmd += ' PODOFO_ENABLED=YES'

            sdkOutDir = buildPathName + "\\..\\" + distribPath
            exeOutDir = buildPathName + '\\..\\' + exePath
            
            print("command is: " + cmd)

            # build
            res = systemManager.execute(cmd)
            if (res != 0) :
                sys.exit(-1)

            systemManager.removeDirectory(os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                                          os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                                          '*.h',                                                                 \
                                          True, False, True, True) 
            systemManager.distributeFiles(os.path.join( buildPathName, Program._PATH_NAME_INCLUDE),              \
                                          os.path.join( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE), \
                                          '*.inc',                                                                 \
                                          True, False, True, True) 
            
            # copy output to appropriate bin dir
            print("Copy files into SDK dir")
            shutil.copy( buildPathName + "\\gdal111" + suffix + ".dll", sdkOutDir + "\\gdal111" + suffix + ".dll")
            shutil.copy( buildPathName + "\\gdal" + suffix + ".exp", sdkOutDir + "\\gdal" + suffix + ".exp")
            shutil.copy( buildPathName + "\\gdal" + suffix + ".lib", sdkOutDir + "\\gdal" + suffix + ".lib")
            shutil.copy( buildPathName + "\\gdal-static" + suffix + ".lib", sdkOutDir + "\\gdal-static" + suffix + ".lib")
            if not buildSettings.ReleaseSpecified():
                    shutil.copy( buildPathName + "\\gdal111" + suffix + ".pdb", sdkOutDir + "\\gdal111" + suffix + ".pdb")

			# Apps
            for file in glob.glob(buildPathName + "\\apps\\*.exe"):
                print( "copying " + file + " -> " + exeOutDir)
                shutil.copy(file, exeOutDir)
                
			# FileGDB plugin
            for file in glob.glob(buildPathName + "\\ogr\\ogrsf_frmts\\filegdb\\ogr_FileGDB.*"):
                print( "copying " + file + " -> " + sdkOutDir)
                shutil.copy(file, sdkOutDir)
			
			# ESRI DLL's
            if ( buildSettings.X64Specified() ):
            	for file in glob.glob(buildPathNameFileGDB + "\\bin64\\*.*"):
            		print( "copying " + file + " -> " + sdkOutDir)
            		shutil.copy(file, sdkOutDir)
            else:
            	for file in glob.glob(buildPathNameFileGDB + "\\bin\\*.*"):
            		print( "copying " + file + " -> " + sdkOutDir)
            		shutil.copy(file, sdkOutDir)
			

            #for file in glob.glob(compileOutDir + "\\*.dll"):
            #    print( "copying " + file + " -> " + sdkOutDir)
            #    shutil.copy(file, sdkOutDir)
            #for file in glob.glob(compileOutDir + "\\*.pdb"):
            #    print( "copying " + file + " -> " + sdkOutDir)
            #    shutil.copy(file, sdkOutDir)





#------------------------------------------------------------------------------
Program().main()
#------------------------------------------------------------------------------
