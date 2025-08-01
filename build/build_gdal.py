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
        # systemManager.removeDirectory(buildPathName)

        # copy APR source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        systemManager.removeDirectory(cmakeBuildPath)
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
            "CRYPTOPP_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "crypto")),
            "CRYPTOPP_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"crypto{libSuffix}")),
            "CRYPTOPP_TEST_KNOWNBUG": "TRUE",
            "EXPAT_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "expat")),
            "EXPAT_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libexpat{libSuffix}")),
            "GEOS_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "geos")),
            "GEOS_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"geos_c{libSuffix}")),
            "GEOTIFF_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libgeotiff")),
            "GEOTIFF_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libgeotiff{libSuffix}")),
            "GEOTIFF_LIBRARY_DEBUG": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libgeotiff_d.lib")),
            "GEOTIFF_LIBRARY_RELEASE": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libgeotiff.lib")),
            "HDF5_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "hdf5")),
            "HDF5_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"hdf5{libSuffix}")),
            "Iconv_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libiconv")),
            "Iconv_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libiconv{libSuffix}")),
            "Iconv_CHARSET_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"charset{libSuffix}")),
            "JPEG_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libjpeg")),
            "JPEG_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libjpeg{libSuffix}")),
            "LIBKML_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase)),
            "LIBKML_BASE_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"kmlbase{libSuffix}")),
            "LIBKML_DOM_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"kmldom{libSuffix}")),
            "LIBKML_ENGINE_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"kmlengine{libSuffix}")),
            "LIBXML2_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libxml")),
            "LIBXML2_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libxml{libSuffix}")),
            "MUPARSER_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "muparser")),
            "MUPARSER_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"muparser{libSuffix}")),
            "OPENSSL_ROOT_DIR": pathFinder.path(buildPathName, "..", "openssl"),
            "PNG_PNG_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libpng")),
            "PNG_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libpng{libSuffix}")),
            "PODOFO_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "podofo")),
            "PODOFO_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"podofo{libSuffix}")),
            "PROJ_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "proj")),
            "PROJ_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"proj{libSuffix}")),
            "XercesC_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "xerces")),
            "XercesC_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"xerces{libSuffix}")),
            "XercesC_VERSION": "3.3.0",
            "SQLite3_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "sqlite3")),
            "SQLite3_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"sqlite3{libSuffix}")),
            "TIFF_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libtiff")),
            "TIFF_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libtiff{libSuffix}")),
            "ZLIB_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "zlib")),
            "ZLIB_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"zlib{libSuffix}")),
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
            # + f'--debug-output '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DBUILD_PYTHON_BINDINGS=OFF "
            + f"-DBUILD_TESTING=OFF "
            # + f"{"" if (buildSettings.ReleaseSpecified()) else "-DEXPORT_PDB=ON "}"
            + f"-DCMAKE_BUILD_TYPE={conf} "
            + f"-DCMAKE_INSTALL_PREFIX={cmakeInstallPath} "
            + f"{externalLibStr} "
            + f"{buildSourceName}"
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        cmakeCommandLine = (
            f"{pathFinder.getCMakeFileName()} "
            + f"--build "
            + f". "
            + f"-j 1 "
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

        dllName = (
            f"{Program._LIBNAME}"
            + f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}'
            + f".dll"
        )
        libName = (
            f"{Program._LIBNAME}"
            + f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}'
            + f".lib"
        )
        pdbName = (
            f"{Program._LIBNAME}"
            + f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}'
            + f".pdb"
        )

        incdir = pathFinder.path(cmakeInstallPath, "include")
        bindir = pathFinder.path(cmakeInstallPath, "bin")
        libdir = pathFinder.path(cmakeInstallPath, "lib")

        systemManager.distributeFiles(
            incdir,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )
        systemManager.distributeFiles(
            pathFinder.path(cmakeInstallPath, "share", "proj"),
            pathFinder.path(sdkOutDir, "..", "proj"),
            "*",
        )
        for f in glob.glob(pathFinder.path(libdir, "*.lib")):
            fname = f[len(libdir) + 1 :]
            fname = f"{fname[:fname.find(Program._LIBNAME) + len(Program._LIBNAME) :]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib"

            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        for f in glob.glob(pathFinder.path(bindir, "*.dll")):
            fname = f[len(bindir) + 1 :]
            fname = f"{fname[:fname.find(Program._LIBNAME) + len(Program._LIBNAME)]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.dll"
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        if not buildSettings.ReleaseSpecified():
            for f in glob.glob(pathFinder.path(libdir, "*.pdb")):
                fname = f[len(libdir) + 1 :]
                fname = f"{fname[:fname.find(Program._LIBNAME) + len(Program._LIBNAME)]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.pdb"
                systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
