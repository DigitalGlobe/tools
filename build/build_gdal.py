# ------------------------------------------------------------------------------
#
# build_gdal.py
#
# Summary : Builds the GDAL Library
#
# ------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program :
    DESCRIPTION = "Builds the GDAL library"

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "gdal"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\gdal"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "libgdal"
    _DEBUG_SUFFIX = "_d"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\gdal"
    # ----------------------------------------------------------------------
    # the name of the path that contains the cmake files
    _PATH_NAME_CMAKE_SOURCE = "."
    _PATH_NAME_CMAKE_BUILD = "build"
    _PATH_NAME_CMAKE_INSTALL = "install"

    _FILE_NAME_FINDLIBKML_CMAKE = "cmake/modules/packages/FindLibKML.cmake"
    _FILE_NAME_OGR_KML_CMAKE = "ogr/ogrsf_frmts/libkml/CMakeLists.txt"
    # --------------------------------------------------------------------------
    # constructors

    # ----------------------------------------------------------------------
    # Constructs this program.
    #
    # Parameters :
    #     self : this program
    def __init__(self):

        pass

    # ----------------------------------------------------------------------

    # --------------------------------------------------------------------------
    # public methods

    # ----------------------------------------------------------------------
    # The main method of the program.
    #
    # Parameters :
    #     self : this program
    def main(self):

        systemManager = SystemManager()
        pathFinder = PathFinder()

        # process command-line arguments
        buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)

        # initialize environment variables
        systemManager.initializeIncludeEnvironmentVariable(buildSettings.X64Specified())
        systemManager.initializeLibraryEnvironmentVariable(buildSettings.X64Specified())

        # MSBuild is under "Program Files (x86)"
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getMSBuildFileName(buildSettings.X64Specified())
        )

        compileOutDir = ""
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getWindowsSdkBinPathName(buildSettings.X64Specified())
        )

        # get the paths
        buildPathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_BUILD
        )
        sourcePathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_SOURCE
        )

        buildSourceName = pathFinder.path(
            buildPathName, Program._PATH_NAME_CMAKE_SOURCE
        )
        cmakeBuildPath = pathFinder.path(buildPathName, Program._PATH_NAME_CMAKE_BUILD)
        cmakeInstallPath = pathFinder.path(
            cmakeBuildPath, Program._PATH_NAME_CMAKE_INSTALL
        )

        systemManager.removeDirectory(cmakeBuildPath)

        sdkOutDir = pathFinder.path(
            buildPathName,
            "..",
            (
                Program._PATH_NAME_DISTRIBUTION_X64
                if buildSettings.X64Specified()
                else Program._PATH_NAME_DISTRIBUTION_X86
            ),
        )

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        systemManager.removeDirectory(buildPathName)

        # copy APR source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        # modify the vcxproj to work with our version of vscode
        sedCommandLine = (
            f"{pathFinder.path(pathFinder.PATH_GNU_TOOLS, "sed.exe")} "
            + f'-i.bak "s/MINIZIP/MINIZIP MINIZIP_AES/g" '
            + f"{Program._FILE_NAME_FINDLIBKML_CMAKE}"
        )

        print("cmd: " + sedCommandLine)
        sedResult = systemManager.execute(sedCommandLine)
        if sedResult != 0:
            sys.exit(-1)

        sedCommandLine = (
            f"{pathFinder.path(pathFinder.PATH_GNU_TOOLS, "sed.exe")} "
            + f'-i.bak "s/TARGET LIBKML::MINIZIP/TARGET LIBKML::MINIZIP AND TARGET LIBKML::MINIZIP_AES/g" '
            + f"{Program._FILE_NAME_OGR_KML_CMAKE}"
        )

        print("cmd: " + sedCommandLine)
        sedResult = systemManager.execute(sedCommandLine)
        if sedResult != 0:
            sys.exit(-1)

        sedCommandLine = (
            f"{pathFinder.path(pathFinder.PATH_GNU_TOOLS, "sed.exe")} "
            + f'-i.bak "s/PRIVATE LIBKML::MINIZIP/PRIVATE LIBKML::MINIZIP LIBKML::MINIZIP_AES/g" '
            + f"{Program._FILE_NAME_OGR_KML_CMAKE}"
        )

        print("cmd: " + sedCommandLine)
        sedResult = systemManager.execute(sedCommandLine)
        if sedResult != 0:
            sys.exit(-1)

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        includeBase = pathFinder.path(
            buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, ".."
        )
        libSuffix = (
            f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib'
        )
        exeSuffix = (
            f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.exe'
        )

        externalLibs = {
            "BISON_EXECUTABLE": pathFinder.slasher(pathFinder.path(sdkOutDir, "..", "bin", f"bison{exeSuffix}")),
            "CURL_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase)),
            "CURL_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libcurl{libSuffix}")),
            "CRYPTOPP_HINTPATH": pathFinder.slasher(pathFinder.path(includeBase, "..")),
            f"CRYPTOPP_LIBRARY_{"RELEASE" if buildSettings.ReleaseSpecified() else "DEBUG"}": pathFinder.slasher(pathFinder.path(sdkOutDir, f"cryptopp{libSuffix}")),
            "CRYPTOPP_TEST_KNOWNBUG": "TRUE",
            "EXPAT_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "expat")),
            "EXPAT_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libexpat{libSuffix}")),
            "EXPAT_USE_STATIC_LIB": "OFF",
            "FileGDB_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "filegdb")),
            "FileGDB_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"filegdbapi{libSuffix}")),
            "FileGDB_LIBRARY_DEBUG": pathFinder.slasher(pathFinder.path(sdkOutDir, f"filegdbapi{Program._DEBUG_SUFFIX}.lib")),
            "FileGDB_LIBRARY_RELEASE": pathFinder.slasher(pathFinder.path(sdkOutDir, f"filegdbapi.lib")),
            "GEOS_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "geos")),
            "GEOS_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"geos_c{libSuffix}")),
            "GEOTIFF_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libgeotiff")),
            "GEOTIFF_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libgeotiff{libSuffix}")),
            "HDF5_ROOT": pathFinder.slasher(pathFinder.path(buildPathName, "..", "HDF5", "build", "install")),
            "Iconv_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libiconv")),
            "Iconv_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libiconv{libSuffix}")),
            "Iconv_CHARSET_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"charset{libSuffix}")),
            "JPEG_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libjpeg")),
            "JPEG_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libjpeg{libSuffix}")),
            "LIBKML_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase)),
            "LIBKML_MINIZIP_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"minizip{libSuffix}")),
            "LIBKML_MINIZIP_AES_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"aes{libSuffix}")),
            "LIBKML_URIPARSER_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"uriparser{libSuffix}")),
            "LIBKML_BASE_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"kmlbase{libSuffix}")),
            "LIBKML_DOM_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"kmldom{libSuffix}")),
            "LIBKML_ENGINE_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"kmlengine{libSuffix}")),
            "LIBXML2_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase)),
            "LIBXML2_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libxml2{libSuffix}")),
            "MUPARSER_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "muparser")),
            "MUPARSER_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"muparser{libSuffix}")),
            "OPENSSL_ROOT_DIR": pathFinder.path(buildPathName, "..", "openssl", "install"),
            "PNG_PNG_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libpng")),
            "PNG_LIBRARY_DEBUG": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libpng{Program._DEBUG_SUFFIX}.lib")),
            "PNG_LIBRARY_RELEASE": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libpng.lib")),
            # "PODOFO_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "podofo")),
            # "PODOFO_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"podofo{libSuffix}")),
            "PROJ_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "proj")),
            "PROJ_LIBRARY_DEBUG": pathFinder.slasher(pathFinder.path(sdkOutDir, f"proj{Program._DEBUG_SUFFIX}.lib")),
            "PROJ_LIBRARY_RELEASE": pathFinder.slasher(pathFinder.path(sdkOutDir, f"proj.lib")),
            "SQLite3_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "sqlite3")),
            "SQLite3_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"sqlite3{libSuffix}")),
            "TIFF_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libtiff")),
            "TIFF_LIBRARY_DEBUG": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libtiff{Program._DEBUG_SUFFIX}.lib")),
            "TIFF_LIBRARY_RELEASE": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libtiff.lib")),
            "XercesC_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase)),
            "XercesC_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"xerces{libSuffix}")),
            "XercesC_VERSION": "3.3.0",
            "ZLIB_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "zlib")),
            "ZLIB_LIBRARY_DEBUG": pathFinder.slasher(pathFinder.path(sdkOutDir, f"zlib{Program._DEBUG_SUFFIX}.lib")),
            "ZLIB_LIBRARY_RELEASE": pathFinder.slasher(pathFinder.path(sdkOutDir, f"zlib.lib")),
            "ZSTD_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "zstd")),
            "ZSTD_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"zstd{libSuffix}")),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        # run CMake
        # -DCMAKE_POLICY_VERSION_MINIMUM is to avoid min compatability errors in CMake
        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f'-A {platform} '
            + f'-DCMAKE_POLICY_VERSION_MINIMUM=3.10 '
            + f'-DBUILD_PYTHON_BINDINGS=OFF '
            + f'-DBUILD_TESTING=OFF '
            + f'-DCMAKE_INCLUDE_PATH={pathFinder.slasher(pathFinder.path(includeBase))} '
            + f'-DCMAKE_LIBRARY_PATH={pathFinder.slasher(pathFinder.path(sdkOutDir))} '
            + f'-DCMAKE_INSTALL_PREFIX={cmakeInstallPath} '
            # + f'-DCMAKE_C_FLAGS="/FS /DWIN32 /D_WINDOWS /W3 /GR /EHsc" '
            # + f'-DCMAKE_CXX_FLAGS="/FS /DWIN32 /D_WINDOWS /W3 /GR /EHsc" '
            + f'{externalLibStr} '
            + f'{buildSourceName}'
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        cmakeCommandLine = (
            f"{pathFinder.getCMakeFileName()} "
            + f"--build "
            + f". "
            + f"-j 6 "
            + f"--config {conf} "
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        cmakeCommandLine = (
            f"{pathFinder.getCMakeFileName()} "
            + f"--install "
            + f". "
            + f"--config {conf} "
            + f"--prefix "
            + f"{cmakeInstallPath}"
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        incdir = pathFinder.path(cmakeInstallPath, "include")
        bindir = pathFinder.path(cmakeInstallPath, "bin")
        libdir = pathFinder.path(cmakeInstallPath, "lib")

        systemManager.distributeFiles(
            incdir,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )
        systemManager.distributeFiles(
            pathFinder.path(cmakeInstallPath, "share", "gdal"),
            pathFinder.path(sdkOutDir, "..", "gdal"),
            "*",
        )
        for f in glob.glob(pathFinder.path(libdir, "*.lib")):
            fname = f[len(libdir) + 1 :]
            fname = f"{fname[:fname.find(f"{"" if (buildSettings.ReleaseSpecified()) else "d"}.lib")]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib"

            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        for f in glob.glob(pathFinder.path(bindir, "*.dll")):
            fname = f[len(libdir) + 1 :]
            fname = f"{fname[:fname.find(f"{"" if (buildSettings.ReleaseSpecified()) else "d"}.dll")]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.dll"
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        if not buildSettings.ReleaseSpecified():
            pdfdir = pathFinder.path(cmakeBuildPath, conf)
            for f in glob.glob(pathFinder.path(pdfdir, "*.pdb")):
                fname = f[len(pdfdir) + 1 :]
                fname = f"{fname[:fname.find(f"{"" if (buildSettings.ReleaseSpecified()) else "d"}.pdb")]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.pdb"
                systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        systemManager.distributeFiles(
            bindir,
            sdkOutDir,
            "*.exe",
            suffix=None if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX
        )


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
