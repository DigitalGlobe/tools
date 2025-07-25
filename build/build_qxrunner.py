# ------------------------------------------------------------------------------
#
# build_qxrunner.py
#
# Summary : Builds the qxrunner library.
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
    DESCRIPTION = "Builds the qxrunner library."
    _FILE_NAME_SOLUTION = "libgist.vcxproj"

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "qxrunner"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\qxrunner"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "qxrunner"
    _DEBUG_SUFFIX = "_d"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "include"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\qxrunner"

    _PATH_NAME_QXRUNNER = "src\\qxrunner"
    _PATH_NAME_QXCPPUNIT = "src\\qxcppunit"

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

        systemManager.changeDirectory(pathFinder.path(buildPathName, Program._PATH_NAME_QXRUNNER))

        qmakeCommandLine = (
            f'qmake -r '
            + f'CONFIG+={"release" if  buildSettings.ReleaseSpecified() else "debug"}_dll '
        )

        cmd = f'"{vcVars}" && {qmakeCommandLine}'
        print("cmd: " + cmd)
        qmakeResult = systemManager.execute(cmd)
        if qmakeResult != 0:
            sys.exit(-1)

        nmakeCommandLine = (
            f'nmake '
        )

        cmd = f'"{vcVars}" && {nmakeCommandLine}'

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.removeDirectory(pathFinder.path( buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE))
        systemManager.distributeFiles(
            pathFinder.path( buildPathName, "include"),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )

        systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_QXRUNNER, f"{"release" if  buildSettings.ReleaseSpecified() else "debug"}_dll"),
            pathFinder.path(sdkOutDir),
            "*.lib",
        )
        systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_QXRUNNER, f"{"release" if  buildSettings.ReleaseSpecified() else "debug"}_dll"),
            pathFinder.path(sdkOutDir),
            "*.dll",
        )

        if not buildSettings.ReleaseSpecified():
            systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_QXRUNNER, f"{"release" if  buildSettings.ReleaseSpecified() else "debug"}_dll"),
            pathFinder.path(sdkOutDir),
            "*.pdb",
            )

        systemManager.changeDirectory(
            pathFinder.path(buildPathName, Program._PATH_NAME_QXCPPUNIT)
        )

        qmakeCommandLine = (
            f"qmake -r "
            + f'CONFIG+={"release" if  buildSettings.ReleaseSpecified() else "debug"}_dll '
            + f"INCLUDEPATH+={pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, "..")} "
            # + f"CPPUNIT={pathFinder.path(sdkOutDir, "..")} "
            + f"LIBS+={pathFinder.path(sdkOutDir, 'qxrunner' + ('' if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX) + '.lib')} "
            + f"LIBS+={pathFinder.path(sdkOutDir, 'cppunit' + ('' if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX) + '.lib')} "
        )

        cmd = f'"{vcVars}" && {qmakeCommandLine}'
        print("cmd: " + cmd)
        qmakeResult = systemManager.execute(cmd)
        if qmakeResult != 0:
            sys.exit(-1)

        cmd = f'"{vcVars}" && {nmakeCommandLine}'

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_QXCPPUNIT, f"{"release" if  buildSettings.ReleaseSpecified() else "debug"}_dll"),
            pathFinder.path(sdkOutDir),
            "*.lib",
        )
        systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_QXCPPUNIT, f"{"release" if  buildSettings.ReleaseSpecified() else "debug"}_dll"),
            pathFinder.path(sdkOutDir),
            "*.dll",
        )

        if not buildSettings.ReleaseSpecified():
            systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_QXCPPUNIT, f"{"release" if  buildSettings.ReleaseSpecified() else "debug"}_dll"),
            pathFinder.path(sdkOutDir),
            "*.pdb",
            )

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
