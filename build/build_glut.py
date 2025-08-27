# ------------------------------------------------------------------------------
#
# build_glut.py
#
# Summary : Builds the UriParser libraries.
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
    DESCRIPTION = "Builds the glut library."

    # ----------------------------------------------------------------------
    # the name of the dynamic solution file
    _FILE_NAME_SOLUTION = "glut_2012.vcxproj"

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "glut"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\glut"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "glut32"
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "include"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\glut"
    # ----------------------------------------------------------------------

    def __init__(self):

        pass

        # ----------------------------------------------------------------------

    def main(self):
        systemManager = SystemManager()
        pathFinder = PathFinder()
        xmlUtils = XmlUtils()

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
            pathFinder.PATH_GNU_TOOLS
            )

        # get the paths
        buildPathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_BUILD
            )
        sourcePathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_SOURCE
            )

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        sdkOutDir = pathFinder.getSDKLibPath(buildPathName, platform, conf)

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        systemManager.removeDirectory(buildPathName)

        # copy UriParser to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        # modify the vcxproj to work with our version of vscode
        sedCommandLine = (
            f"{pathFinder.path(pathFinder.PATH_GNU_TOOLS, "sed.exe")} "
            + f"-i.bak s/^<PlatformToolset^>v110/^<PlatformToolset^>{pathFinder.VISUAL_STUDIO_VERSION_NUM}/g "
            + f"{Program._FILE_NAME_SOLUTION}"
            )

        print("cmd: " + sedCommandLine)
        sedResult = systemManager.execute(sedCommandLine)
        if sedResult != 0:
            sys.exit(-1)

        # build the solution
        solutionFileName = pathFinder.path(buildPathName, Program._FILE_NAME_SOLUTION)
        msBuildCommandLine = ('"%s" ' + "/p:platform=%s " + '"%s"') % (
            pathFinder.getMSBuildFileName(buildSettings.X64Specified()),
            platform,
            solutionFileName,
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

        buildOutDir = pathFinder.path(buildPathName, "build")
        propfile = pathFinder.path(buildPathName, "linker.props")

        msBuildCommandLine += " /p:OutDir=" + buildOutDir
        msBuildCommandLine += " /p:TargetExtension=dll"
        msBuildCommandLine += " /p:SolutionDir=" + buildPathName + "\\"
        msBuildCommandLine += (
            " /p:TargetName="
            + Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
        )
        msBuildCommandLine += " /p:Configuration=" + conf
        msBuildCommandLine += " /p:BuildProjectReferences=false"

        linkerprops = {}
        # linkerprops = ["OutputFile"] = pathFinder.path(buildOutDir, dllName)}
        linkerprops["ImportLibrary"] = pathFinder.path(buildOutDir, libName)
        if buildSettings.ReleaseSpecified():
            linkerprops["DebugSymbols"] = "false"
        else:
            linkerprops["DebugSymbols"] = "true"
            linkerprops["DebugType"] = "full"
            linkerprops["ProgramDatabaseFile"] = "$(OutDir)\\" + pdbName

        xmlUtils.buildDll(conf, platform, {}, linkerprops, propfile)

        msBuildCommandLine += ' /p:ForceImportBeforeCppTargets="' + propfile + '"'

        print("cmd: " + msBuildCommandLine)

        msbuildResult = systemManager.execute(msBuildCommandLine)
        if msbuildResult != 0:
            sys.exit(-1)

        systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_INCLUDE),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h",
            )

        systemManager.copyFile(
            pathFinder.path(buildOutDir, libName), pathFinder.path(sdkOutDir, libName)
        )
        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(buildOutDir, pdbName), pathFinder.path(sdkOutDir, pdbName)
            )

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
