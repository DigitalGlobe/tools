# ------------------------------------------------------------------------------
#
# build_osg.py
#
# Summary : Builds OpenSceneGraph
#
# ------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *


class Program:
    DESCRIPTION = "Builds OpenSceneGraph."
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "osg"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\openscenegraph"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "osg"
    _DEBUG_SUFFIX = "_d"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "include\\osg"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\osg"
    # ----------------------------------------------------------------------
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

        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getNmakePathName(buildSettings.X64Specified())
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

        # copy source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

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
        externalLibs = {
            "CURL_INCLUDE_DIR": pathFinder.path(includeBase),
            "CURL_LIBRARY": pathFinder.path(sdkOutDir, f"libcurl{libSuffix}"),
            "FREETYPE_INCLUDE_DIR": pathFinder.path(includeBase, "freetype"),
            "FREETYPE_LIBRARY": pathFinder.path(sdkOutDir, f"freetype{libSuffix}"),
            "GDAL_INCLUDE_DIR": pathFinder.path(includeBase, "gdal"),
            "GDAL_LIBRARY": pathFinder.path(sdkOutDir, f"gdal{libSuffix}"),
            "JPEG_INCLUDE_DIR": pathFinder.slasher( pathFinder.path(includeBase, "libjpeg") ),
            "JPEG_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libjpeg{libSuffix}")),
            "LIBXML2_INCLUDE_DIR": pathFinder.path(includeBase),
            "LIBXML2_LIBRARY": pathFinder.path(sdkOutDir, f"libxml2{libSuffix}"),
            "PNG_PNG_INCLUDE_DIR": pathFinder.path(includeBase, "libpng"),
            "PNG_LIBRARY": pathFinder.path(sdkOutDir, f"libpng{libSuffix}"),
            "TIFF_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libtiff")),
            "TIFF_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libtiff{libSuffix}")),
            "ZLIB_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "zlib")),
            "ZLIB_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"zlib{libSuffix}")),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DOSG_MSVC_VERSIONED_DLL=OFF "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DCMAKE_BUILD_TYPE={conf} "
            + f"-DCMAKE_INSTALL_PREFIX={cmakeInstallPath} "
            + f"{externalLibStr} "
            + f"{buildSourceName}"
        )

        # print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        cmakeCommandLine = (
            f"{pathFinder.getCMakeFileName()} "
            + f"--build "
            + f". "
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
            + f"{pathFinder.path(cmakeBuildPath, "install")}"
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        incdir = pathFinder.path(cmakeInstallPath, "include", "osg")
        libdir = pathFinder.path(cmakeInstallPath, "lib")
        bindir = pathFinder.path(cmakeInstallPath, "bin")

        systemManager.distributeFiles(
            incdir,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )

        for f in glob.glob(pathFinder.path(libdir, "*.lib")):
            fname = f[len(libdir) + 1 :]
            fname = f"{fname[:fname.find(f"{"" if (buildSettings.ReleaseSpecified()) else "d"}.lib")]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib"

            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        for f in glob.glob(pathFinder.path(bindir, "*.dll")):
            fname = f[len(bindir) + 1 :]
            fname = f"{fname[:fname.find(f"{"" if (buildSettings.ReleaseSpecified()) else "d"}.dll")]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.dll"
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        if not buildSettings.ReleaseSpecified():
            for f in glob.glob(pathFinder.path(bindir, "*.pdb")):
                fname = f[len(bind) + 1 :]
                fname = f"{fname[:fname.find(f"{"" if (buildSettings.ReleaseSpecified()) else "d"}.pdb")]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.pdb"
                systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
