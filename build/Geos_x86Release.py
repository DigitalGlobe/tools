#------------------------------------------------------------------------------
#
# Geos_x86Release.py
#
#------------------------------------------------------------------------------

import os
import shutil

#------------------------------------------------------------------------------
# Geos_x86Release script removes the .obj files from the selected directory
# then builds Geos and places the dlls and libs into the appropriate
# sdk directory

#you'll need to run autogen.bat the first time.  This file is located in
#C:\DigitalGlobe\tools\src\GEOS_x86Release

#you also should probably run vcvarsall x86 to initial the visual studio paths 

#get current working directory
currentDir = os.getcwd()
print("currentDir")
print(currentDir)

baseDir = os.path.normpath(os.getcwd() + os.sep + os.pardir)
print("baseDir")
print(baseDir)

srcDirectory = baseDir + "\\src\\GEOS_x86Release\\src"

print("srcDirectory")
print(srcDirectory)

dest_dll = baseDir + "\\sdk\\x86\\bin"
dest_lib = baseDir + "\\sdk\\x86\\lib"

makefilePath = baseDir + "\\src\\GEOS_x86Release"

print("makefilePath")
print(makefilePath)


#delete all .obj files in the project
def deleteObjFiles(pathToSrcFile):
    for subdir, dirs, files in os.walk(srcDirectory):
        for file in files:
            print(os.path.join(subdir, file))
            fname = os.path.join(subdir,file)
            if fname.endswith('.obj'):
                os.unlink(fname)

deleteObjFiles(srcDirectory)

os.chdir(makefilePath)
os.system("nmake /f makefile.vc MSVC_VER=1900" )

os.chdir(srcDirectory)
            
src_files = os.listdir(srcDirectory)
for file_name in src_files:
    full_file_name = os.path.join(srcDirectory, file_name)
    if (os.path.isfile(full_file_name)):
        print(full_file_name)
        if full_file_name.endswith('.dll'):
            shutil.copy(full_file_name, dest_dll)
        elif full_file_name.endswith('.lib'):
            shutil.copy(full_file_name, dest_lib)

