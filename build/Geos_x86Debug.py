#------------------------------------------------------------------------------
#
# Geos_x86Debug.py
#
#------------------------------------------------------------------------------

import os
import shutil

#------------------------------------------------------------------------------
# Geos_x86Debug script removes the .obj files from the selected directory
#then builds GEOS x86 debug build

#you'll need to run autogen.bat the first time.  This file is located in
#C:\DigitalGlobe\tools\src\GEOS_x86Debug

#you also should probably run vcvarsall x86 to initial the visual studio paths 


#get current working directory
currentDir = os.getcwd()
print("currentDir")
print(currentDir)

baseDir = os.path.normpath(os.getcwd() + os.sep + os.pardir)
print("baseDir")
print(baseDir)

srcDirectory = baseDir + "\\src\\GEOS_x86Debug\\src"

print("srcDirectory")
print(srcDirectory)

dest_dll = baseDir + "\\sdk\\x86\\bin"
dest_lib = baseDir + "\\sdk\\x86\\lib"

makefilePath = baseDir + "\\src\\GEOS_x86Debug"

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
            
#build x86 debug dlls
os.system("nmake /f makefile.vc MSVC_VER=1900 BUILD_DEBUG=YES")

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
