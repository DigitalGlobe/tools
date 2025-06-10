# ------------------------------------------------------------------------------
#
# PathFinder.py
#
# ------------------------------------------------------------------------------

import os


# ------------------------------------------------------------------------------
# The PathFinder class represents finders that locate the paths of build
# tools.
class PathFinder:

    # --------------------------------------------------------------------------
    # private constants
    # ----------------------------------------------------------------------

    _DRIVE = "C:"
    # ----------------------------------------------------------------------
    # the relative path name of Visual Studio
    _PATH_NAME_VISUAL_STUDIO = "Microsoft Visual Studio"
    _PATH_NAME_VISUAL_STUDIO_VERSIONED = "Microsoft Visual Studio 17.0"
    # ----------------------------------------------------------------------
    # the version of Visual Studio
    _PATH_NAME_VISUAL_STUDIO_VERSION = "2022"

    # the version of MSVC
    _PATH_NAME_MSVC_VERSION = "14.44.35207"

    # ----------------------------------------------------------------------
    # the name of the MSBuild executable
    _FILE_NAME_MSBUILD = "MSBuild.exe"
    # ----------------------------------------------------------------------
    # the path MSBuild directories
    _PATH_NAME_MSBUILD = "MSBuild\\Current"
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # the relative 64-bit path
    _PATH_NAME_X64 = "x64"
    # ----------------------------------------------------------------------
    # the relative 32-bit path
    _PATH_NAME_X86 = "x86"
    # ----------------------------------------------------------------------
    # the relative 64-bit bin path
    _PATH_NAME_BIN_X64 = "amd64"
    # ----------------------------------------------------------------------
    # the relative 32-bit  bin path
    _PATH_NAME_BIN_X86 = ""

    # ----------------------------------------------------------------------
    # the name of the Nmake executable file
    _FILE_NAME_NMAKE = "nmake.exe"
    # ----------------------------------------------------------------------
    # the relative 64-bit Visual Studio bin path
    _PATH_NAME_IDE = "Common7\\IDE"

    # ----------------------------------------------------------------------
    # the relative path name of the Windows SDK
    _PATH_NAME_WINDOWS_SDK_BASE = "Windows Kits"

    _PATH_NAME_WINDOWS_SDK_MAJOR_VERSION = "10"
    # ----------------------------------------------------------------------
    # the Windows SDK version
    _PATH_NAME_WINDOWS_SDK_VERSION = "10.0.26100.0"
    # ----------------------------------------------------------------------

    # the relative path name of the Windows SDK
    _PATH_NAME_NETFXSDK_BASE = "NETFXSDK"
    # ----------------------------------------------------------------------
    # the Windows SDK version
    _PATH_NAME_NETFXSDK_VERSION = "4.8"
    # ----------------------------------------------------------------------

    _PATH_NAME_CMAKE = "D:\\Users\\tim.tisler\\Apps\\CMake"
    # --------------------------------------------------------------------------
    # public constants
    # ----------------------------------------------------------------------
    # the name of the environment variable that contains the path of the
    # 32-bit program files folder
    PROGRAM_FILES_X86 = "Program Files (x86)"
    # ----------------------------------------------------------------------
    # the name of the 64-bit program-files environment variable
    PROGRAM_FILES_X64 = "Program Files"
    # ----------------------------------------------------------------------
    # the name of the CMake executable file
    FILE_NAME_CMAKE = "cmake.exe"

    # Name of the Visual Studio Version (mostly for CMake)
    VISUAL_STUDIO_VERSION = "Visual Studio 17 2022"

    # --------------------------------------------------------------------------
    # constructors

    # ----------------------------------------------------------------------
    # Constructs this finder as a default finder.
    #
    # Parameters :
    #     self : this finder
    def __init__(self):
        pass

    # ----------------------------------------------------------------------

    # --------------------------------------------------------------------------
    # public methods

    # ----------------------------------------------------------------------
    # Gets the path of the MSBuild executable file.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the MSBuild executable file
    def getCMakeFileName(self):

        fileName = os.path.join(
            PathFinder._PATH_NAME_CMAKE,
            "bin",
            PathFinder.FILE_NAME_CMAKE
        )

        if not os.path.exists(fileName):
            raise Exception(f"Can't find {PathFinder.FILE_NAME_CMAKE} in {fileName}")
        return fileName

    # ----------------------------------------------------------------------
    # Gets the path of the MSBuild executable file.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the MSBuild executable file
    def getMSBuildFileName(self, x64Specified):

        fileName = os.path.join(
            self._getVisualStudioPathName(x64Specified),
            "MSBuild",
            "Current",
            "Bin",
            (PathFinder._PATH_NAME_BIN_X64 if x64Specified else ""),
            PathFinder._FILE_NAME_MSBUILD,
        )

        if not os.path.exists(fileName):
            raise Exception(f"Can't find {PathFinder._FILE_NAME_MSBUILD} in {fileName}")
        return fileName

    # ----------------------------------------------------------------------
    # Gets the name of the Nmake executable file.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Nmake executable file
    def getNmakePathName(self, x64Specified):

        fileName = os.path.join(
            self._getMSVCPathName(x64Specified),
            "bin",
            "Hostx64",
            (PathFinder._PATH_NAME_X64 if x64Specified else PathFinder._PATH_NAME_X86),
            PathFinder._FILE_NAME_NMAKE
        )

        if not os.path.exists(fileName):
            raise Exception(f"Can't find name in {fileName}")
        return os.path.dirname(fileName)

    # ----------------------------------------------------------------------
    # Gets the name of the Nmake executable file.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Nmake executable file
    def getNmakeFileName(self, x64Specified):

        fileName = os.path.join(
            self._getMSVCPathName(x64Specified),
            "bin",
            "Hostx64",
            (PathFinder._PATH_NAME_X64 if x64Specified else PathFinder._PATH_NAME_X86),
            PathFinder._FILE_NAME_NMAKE,
        )

        if not os.path.exists(fileName):
            raise Exception(f"Can't find name in {fileName}")
        return fileName

    # ----------------------------------------------------------------------
    # Gets the name of the Visual Studio bin path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Visual Studio bin path
    def getVisualStudioBinPathName(self, x64Specified):

        pathName = os.path.join(
            self._getVisualStudioPathName(x64Specified),
            (
                PathFinder._PATH_NAME_BIN_X64
                if (x64Specified)
                else PathFinder._PATH_NAME_BIN_X86
            ),
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Visual Studio bin pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Visual Studio bin path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Visual Studio bin path
    def getVisualStudioIDEPathName(self, x64Specified):

        pathName = os.path.join(
            self._getVisualStudioPathName(x64Specified), PathFinder._PATH_NAME_IDE
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Visual Studio IDE pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the ATL include path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Visual Studio include path
    def getATLIncludePathName(self, x64Specified):

        pathName = os.path.join(
            self.getATLPathName(x64Specified),
            "include",
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad ATL include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the ATL include path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Visual Studio include path
    def getATLLibraryPathName(self, x64Specified):

        pathName = os.path.join(
            self.getATLPathName(x64Specified),
            "lib",
            (PathFinder._PATH_NAME_X64 if (x64Specified) else PathFinder._PATH_NAME_BIN_X86),
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad ATL include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the ATL  path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Visual Studio include path
    def getATLPathName(self, x64Specified):

        pathName = os.path.join(
            self._getMSVCPathName(x64Specified),
            "atlmfc"
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad ATL pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the ATL include path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Visual Studio include path
    def getMSVCLibraryPathName(self, x64Specified):

        pathName = os.path.join(
            self._getMSVCPathName(x64Specified),
            "lib",
            (PathFinder._PATH_NAME_X64 if (x64Specified) else PathFinder._PATH_NAME_X86)
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad MSVC pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the MS VC include path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Visual Studio include path
    def getMSVCIncludePathName(self, x64Specified):

        pathName = os.path.join(
            self._getMSVCPathName(x64Specified),
            "include"
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad MSVC include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the MS VC path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Visual Studio include path
    def _getMSVCPathName(self, x64Specified):

        pathName = os.path.join(
            self._getVisualStudioPathName(x64Specified),
            "VC",
            "Tools",
            "MSVC",
            PathFinder._PATH_NAME_MSVC_VERSION,
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad MSVC pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Visual Studio path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Visual Studio path
    # Throws :
    #     Exception : if this method failed to find Visual Studio
    def _getVisualStudioPathName(self, x64Specified):

        pathName = os.path.join(
            PathFinder._DRIVE,
            PathFinder.PROGRAM_FILES_X86,
            PathFinder._PATH_NAME_VISUAL_STUDIO,
            PathFinder._PATH_NAME_VISUAL_STUDIO_VERSION,
            "BuildTools"
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Visual Studio pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def _getWindowsSdkIncludePathName(self, x64Specified):

        pathName = os.path.join(
            self.getWindowsSdkPathName(x64Specified),
            "Include",
            PathFinder._PATH_NAME_WINDOWS_SDK_VERSION,
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getWindowsSdkCppWinRTIncludePathName(self, x64Specified):

        pathName = os.path.join(
            self._getWindowsSdkIncludePathName(x64Specified),
            "cppwinrt",
            "winrt"
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getWindowsSdkSharedIncludePathName(self, x64Specified):

        pathName = os.path.join(
            self._getWindowsSdkIncludePathName(x64Specified),
            "shared",
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getWindowsSdkUCrtIncludePathName(self, x64Specified):

        pathName = os.path.join(
            self._getWindowsSdkIncludePathName(x64Specified),
            "ucrt",
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getWindowsSdkUMIncludePathName(self, x64Specified):

        pathName = os.path.join(
            self._getWindowsSdkIncludePathName(x64Specified),
            "um",
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getWindowsSdkWinRTIncludePathName(self, x64Specified):

        pathName = os.path.join(
            self._getWindowsSdkIncludePathName(x64Specified),
            "winrt"
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getWindowsSdkUCrtLibraryPathName(self, x64Specified):

        pathName = os.path.join(
            self._getWindowsSdkLibraryPathName(x64Specified),
            "ucrt",
            (PathFinder._PATH_NAME_X64 if (x64Specified) else PathFinder._PATH_NAME_X86)

        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getWindowsSdkUMLibraryPathName(self, x64Specified):

        pathName = os.path.join(
            self._getWindowsSdkLibraryPathName(x64Specified),
            "um",
            (PathFinder._PATH_NAME_X64 if (x64Specified) else PathFinder._PATH_NAME_X86)

        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def _getWindowsSdkLibraryPathName(self, x64Specified):

        pathName = os.path.join(
            self.getWindowsSdkPathName(x64Specified),
            "Lib",
            PathFinder._PATH_NAME_WINDOWS_SDK_VERSION,
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getWindowsSdkBinPathName(self, x64Specified):

        pathName = os.path.join(
            self.getWindowsSdkPathName(x64Specified),
            "bin",
            PathFinder._PATH_NAME_WINDOWS_SDK_VERSION,
            (
                PathFinder._PATH_NAME_X64
                if (x64Specified)
                else PathFinder._PATH_NAME_X86
            ),
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK Bin pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getWindowsSdkPathName(self, x64Specified):

        pathName = os.path.join(
            PathFinder._DRIVE,
            PathFinder.PROGRAM_FILES_X86,
            PathFinder._PATH_NAME_WINDOWS_SDK_BASE,
            PathFinder._PATH_NAME_WINDOWS_SDK_MAJOR_VERSION
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad Windows SDK pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getNetFxSdkIncludePathName(self, x64Specified):

        pathName = os.path.join(
            self._getNetFxSdkPathName(x64Specified),
            "Include",
            "um",
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad NetFx SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def getNetFxSdkLibraryPathName(self, x64Specified):
        pathName = os.path.join(
            self._getNetFxSdkPathName(x64Specified),
            "Lib",
            "um",
            (
                PathFinder._PATH_NAME_X64
                if (x64Specified)
                else PathFinder._PATH_NAME_X86
            ),
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad NetFx SDK pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------
    # Gets the name of the Windows SDK path.
    #
    # Parameters :
    #     self         : this finder
    #     x64Specified : if <code>true</code>, 64-bit is specified; if
    #                    <code>false</code>, 32-bit is specified
    # Returns :
    #     the name of the Windows SDK path
    def _getNetFxSdkPathName(self, x64Specified):
        pathName = os.path.join(
            PathFinder._DRIVE,
            PathFinder.PROGRAM_FILES_X86,
            PathFinder._PATH_NAME_WINDOWS_SDK_BASE,
            PathFinder._PATH_NAME_NETFXSDK_BASE,
            PathFinder._PATH_NAME_NETFXSDK_VERSION,
        )
        if not os.path.exists(pathName):
            raise Exception(f"Bad NetFx SDK include pathname {pathName}")
        return pathName

    # ----------------------------------------------------------------------


# ------------------------------------------------------------------------------
