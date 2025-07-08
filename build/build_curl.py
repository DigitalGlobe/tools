# ------------------------------------------------------------------------------
#
# build_curl.py
#
# Summary : Builds the CURL library.
#
# ------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *
from XmlUtils import *


class Program:
    # ----------------------------------------------------------------------
    # a description of what the script does
    DESCRIPTION = "Builds the curl library."

    _LIBNAME = 'libcurl'
    _DEBUG_SUFFIX = '_d'

    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit binary files
    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit binary files
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "curl"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\curl"
    # ----------------------------------------------------------------------
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "curl"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\curl"
    # ----------------------------------------------------------------------
    # the name of the path that contains the cmake files
    _PATH_NAME_CMAKE_SOURCE = "."
    _PATH_NAME_CMAKE_BUILD = "build"
    _PATH_NAME_CMAKE_INSTALL = "install"

    def __init__(self):
        pass

    # ----------------------------------------------------------------------

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

        buildSourceName = os.path.join(buildPathName, Program._PATH_NAME_CMAKE_SOURCE)
        cmakeBuildPath = os.path.join(buildPathName, Program._PATH_NAME_CMAKE_BUILD)
        cmakeInstallPath = os.path.join(
            cmakeBuildPath, Program._PATH_NAME_CMAKE_INSTALL
        )

        systemManager.removeDirectory(cmakeBuildPath)

        sdkOutDir = os.path.join(
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

        # copy Boost source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        includeBase = os.path.join(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, "..")
        libSuffix = f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib'
        externalLibs = {
            "BROTLI_INCLUDE_DIR": os.path.join(includeBase, "brotli"),
            "BROTLIENC_LIBRARY": os.path.join(sdkOutDir, f"brotlienc{libSuffix}"),
            "BROTLIDEC_LIBRARY": os.path.join(sdkOutDir, f"brotlidec{libSuffix}"),
            "BROTLICOMMON_LIBRARY": os.path.join(sdkOutDir, f"brotlicommon{libSuffix}"),
            "LIBPSL_INCLUDE_DIR": os.path.join(includeBase, "libpsl"),
            "LIBPSL_LIBRARY": os.path.join(sdkOutDir, f"psl{libSuffix}"),
            "NGHTTP2_INCLUDE_DIR": os.path.join(includeBase, "nghttp2"),
            "NGHTTP2_LIBRARY": os.path.join(sdkOutDir, f"nghttp2{libSuffix}"),
            "ZLIB_INCLUDE_DIR": os.path.join(includeBase, "zlib"),
            "ZLIB_LIBRARY": os.path.join(sdkOutDir, f"zlib{libSuffix}"),
            "ZSTD_INCLUDE_DIR": os.path.join(includeBase, "zstd"),
            "ZSTD_LIBRARY": os.path.join(sdkOutDir, f"zstd{libSuffix}"),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DBUILD_SHARED_LIBS=ON "
            + f"-DBUILD_CURL_EXE=OFF "
            + f"-DBUILD_TESTING=OFF "
            + f"-DBUILD_LIBCURL_DOCS=OFF "
            + f"-DBUILD_MISC_DOCS=OFF "
            + f"-DENABLE_CURL_MANUAL=OFF "
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
            + f"{os.path.join(cmakeBuildPath, "install")}"
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        systemManager.distributeFiles(
            os.path.join( cmakeInstallPath,"include", "curl"),
            os.path.join(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )

        dllName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
            + ".dll"
        )
        libName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
            + ".lib"
        )
        pdbName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
            + ".pdb"
        )

        systemManager.copyFile(
            os.path.join(cmakeInstallPath, "lib", f"{Program._LIBNAME}{"" if (buildSettings.ReleaseSpecified()) else "-d"}_imp.lib"),
            os.path.join(sdkOutDir, libName),
        )

        systemManager.copyFile(
            os.path.join(cmakeInstallPath, "bin", f"{Program._LIBNAME}{"" if (buildSettings.ReleaseSpecified()) else "-d"}.dll"),
            os.path.join(sdkOutDir, dllName),
        )
        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                os.path.join(
                    cmakeBuildPath,
                    "lib",
                    conf,
                    f"{Program._LIBNAME}{"" if (buildSettings.ReleaseSpecified()) else "-d"}.pdb",
                ),
                os.path.join(sdkOutDir, pdbName),
            )


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
