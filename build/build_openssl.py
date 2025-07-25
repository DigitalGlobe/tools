# ------------------------------------------------------------------------------
#
# build_openssl.py
#
# Summary : Builds the OpenSSL library.
#
# ------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *


# ------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program:

    # --------------------------------------------------------------------------
    # constants

    # ----------------------------------------------------------------------
    # a description of what the script does
    DESCRIPTION = "Builds the OpenSSL library."
    # ----------------------------------------------------------------------

    _LIBNAME = "openssl"
    _DEBUG_SUFFIX = "_d"

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "openssl"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\openssl"
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # the pattern for binary files
    _FILE_PATTERN_BINARY = "*.exe"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit binary files
    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit binary files
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"

    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\openssl"
    # ----------------------------------------------------------------------
    _PATH_NAME_NMAKE_INSTALL = "install"

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

        vcVars = pathFinder.getVCVARSFileName(buildSettings.X64Specified())

        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getWindowsSdkBinPathName(buildSettings.X64Specified())
        )

        # determine path names
        buildPathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_BUILD
        )
        sourcePathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_SOURCE
        )

        sdkOutDir = (
            buildPathName
            + "\\..\\"
            + (
                Program._PATH_NAME_DISTRIBUTION_X64
                if buildSettings.X64Specified()
                else Program._PATH_NAME_DISTRIBUTION_X86
            )
        )

        # remove build dir
        systemManager.removeDirectory(buildPathName)
        systemManager.copyDirectory(sourcePathName, buildPathName)
        systemManager.changeDirectory(buildPathName)

        perlCommandLine = (
            f'perl Configure '
            + f'{"VC-WIN64A" if buildSettings.X64Specified() else "VC-WIN32" } '
            + f'--prefix={pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL) } '
            + f'--openssldir={pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL, "ssl" ) } '
            + f'{"--release" if buildSettings.ReleaseSpecified() else "--debug"} '
            + f'enable-brotli-dynamic '
            + f'--with-brotli-include={pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, "..")} '
            + f'--with-brotli-lib={sdkOutDir} '
            + f'zlib-dynamic '
            + f'--with-zlib-include={pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, "..","zlib")} '
            + f'--with-zlib-lib={sdkOutDir} '
            + f'enable-zstd-dynamic '
            + f'--with-zstd-include={pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, "..","zstd")} '
            + f'--with-zstd-lib={sdkOutDir} '
            + f'no-makedepend '
            + f'no-apps '
            + f'CFLAGS="/FS /Z7" CXXFLAGS="/FS /Z7" CPPFLAGS="/FS /Z7" '
        )

        cmd = f'"{vcVars}" && {perlCommandLine}'

        print("cmd: " + cmd)
        perlResult = systemManager.execute(cmd)
        if perlResult != 0:
            sys.exit(-1)

        nmakeCommandLine = f"nmake "

        cmd = f'"{vcVars}" && {nmakeCommandLine}'

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        nmakeCommandLine = f"nmake install "

        cmd = f'"{vcVars}" && {nmakeCommandLine}'

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.removeDirectory(
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE)
        )

        systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL, "include", "openssl"),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h"
        )

        libdir = pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL, "lib")
        bindir = pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL, "bin")

        systemManager.distributeFiles(
            libdir,
            sdkOutDir,
            "*.lib",
            suffix=None if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX,
        )

        systemManager.copyDirectory(
            pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL, "ssl"),
            sdkOutDir,
        )

        for f in glob.glob(pathFinder.path(bindir, "*.dll")):
            fname = f[len(bindir) + 1 :]
            dst = pathFinder.path(
                    sdkOutDir,
                    fname.replace(
                        f"-3-{pathFinder.PATH_NAME_X64 if buildSettings.X64Specified() else pathFinder._PATH_NAME_X86}.dll",
                        f"{"" if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX}.dll",
                    )
                )
            systemManager.copyFile(f,dst)

        for f in glob.glob(pathFinder.path(libdir, "**/*.dll"), recursive=True):
            fname = f[len(bindir) + 1 :]
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname.replace(f".dll", f"{"" if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX}.dll")))

        if not buildSettings.ReleaseSpecified():
            for f in glob.glob(pathFinder.path(bindir, "**/*.pdb"), recursive=True):
                fname = f[len(bindir) + 1 :]
                systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname.replace(f"-3-{pathFinder.PATH_NAME_X64 if buildSettings.X64Specified() else pathFinder._PATH_NAME_X86}.pdb", f"{"" if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX}.pdb")))
            for f in glob.glob(pathFinder.path(libdir, "**/*.pdb"), recursive=True):
                fname = f[len(bindir) + 1 :]
                systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname.replace(f".pdb", f"{"" if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX}.pdb")))


# --------------------------------------------------------------------------

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
