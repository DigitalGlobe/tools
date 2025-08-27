# ------------------------------------------------------------------------------
#
# build_qwt.py
#
# Summary : Builds the QWT libraries.
#
# ------------------------------------------------------------------------------

import glob
import os
import shutil
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

class Program:
    DESCRIPTION = "Builds QWT libs and exes."
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "qwt"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\qwt"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "qwt"
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "include"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\qwt"

    _PATH_NAME_QMAKE_BUILD = "build"

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
            pathFinder.getQMakePathName(buildSettings.X64Specified())
        )

        vcVars = pathFinder.getVCVARSFileName(buildSettings.X64Specified())

        compileOutDir = ""
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

        os.environ["QTDIR"] = pathFinder.getQtPathName(buildSettings.X64Specified())

        # remove build dir
        systemManager.removeDirectory(buildPathName)

        systemManager.copyDirectory(sourcePathName, buildPathName)
        systemManager.changeDirectory(buildPathName)

        qmakeCommandLine = (
            f"qmake "
            + f'"CONFIG+={"release" if buildSettings.ReleaseSpecified() else "debug"}" '
            )

        cmd = f'"{vcVars}" && {qmakeCommandLine}'
        print("cmd: " + cmd)
        qmakeResult = systemManager.execute(cmd)
        if qmakeResult != 0:
            sys.exit(-1)

        nmakeCommandLine = f"nmake "

        cmd = f'"{vcVars}" && {nmakeCommandLine}'

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.removeDirectory(
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE)
        )

        systemManager.distributeFiles(
            pathFinder.path(buildPathName, "src"),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
            )

        if buildSettings.ReleaseSpecified():
            systemManager.distributeFiles(
                pathFinder.path(
                buildPathName,
                "lib"
                ),
                pathFinder.path(sdkOutDir),
                "*.lib",
                )
            systemManager.distributeFiles(
                pathFinder.path(buildPathName, "lib"),
                pathFinder.path(sdkOutDir),
                "*.dll",
                )
        else:
            libdir = pathFinder.path(buildPathName, "lib")

        # we need to rename the debug libs to have a _d suffix (they have a 'd' suffix now)
        for f in glob.glob(pathFinder.path(libdir, "*d.lib")):
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, f.replace("d.lib", "_d.lib")))

        for f in glob.glob(pathFinder.path(libdir, "*d.dll")):
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, f.replace("d.dll", "_d.dll")))

        for f in glob.glob(pathFinder.path(libdir, "*d.pdb")):
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, f.replace("d.pdb", "_d.pdb")))

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
