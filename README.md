Third-Party Libraries for Signature Analyst<p>[![Gitter](https://badges.gitter.im/DigitalGlobe/tools.svg)](https://gitter.im/DigitalGlobe/tools?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
===================

This repository contains the third-party libraries ("tools") that the Signature Analysis application uses.  Building these third-party libraries requires the following versions of the following applications running on 64-bit Microsoft Windows 10 or greater:

 - *Microsoft Visual Studio 2015* or greater
 - *Python 3.5.2* or greater

This repository contains the following directories.

####build
The **build** directory contains Python scripts to build each of the third-party libraries.  Each build script requires two command-line arguments:

 - *Bitness*: either <code>x86</code>, to build a 32-bit version of the library, or <code>x64</code>, to build a 64-bit version of the library
 - *Configuration*: either <code>debug</code>, to build a debug version of the library, or <code>release</code>, to build a release version of the library

For example, to build the CURL library, there is a <code>build_curl.py</code> Python script.  The following commands show how to run this script to build all four bitness and configuration combinations of this library:

 - *32-bit Debug*: <code>python.exe build_curl.py x86 debug </code>
 - *32-bit Release*: <code>python.exe build_curl.py x86 release </code>
 - *64-bit Debug*: <code>python.exe build_curl.py x64 debug </code>
 - *64-bit Release*: <code>python.exe build_curl.py x64 release</code>

Moreover, there is a "master" build file that calls each of build scripts to build all four configurations of every third-party library:

&nbsp;&nbsp;&nbsp;&nbsp;<code>python.exe build.py</code>

The Python build scripts copy their build outputs into the **sdk** directory, which the next section describes.

####sdk
The **sdk** directory contains the built files, organized into the following subdirectory structure:

 - **x86**
	 - **bin**:  contains 32-bit debug and release EXE files
	 - **lib**: contains 32-bit debug and release LIB, DLL, and PDB files
 - **x64**:
	 - **bin**:  contains 64-bit debug and release EXE files
	 - **lib**: contains 64-bit debug and release LIB, DLL, and PDB files

By convention, the file names of the debug versions of the libraries will have <code>&#95;d</code> suffixes (e.g., <code>libcurl&#95;d.lib</code>), and the file names of the release versions will not have these suffixes (e.g., <code>libcurl.lib</code>).

####src
The **src** directory contains the source code of the third-party libraries.

####tests
The **tests** directory contains source code to build applications that use, demonstrate, and test the built third-party libraries, along with scripts to build and run these applications.

The diagram at the following link shows the hierarchy of the directories and subdirectories in this repository:

&nbsp;&nbsp;&nbsp;&nbsp;[Directory Structure of Signature Analyst Third-Party Tools](https://github.com/DigitalGlobe/sa/blob/master/docs/DirectoryStruct.png "Directory Structure of Signature Analysis Third-Party Tools")
