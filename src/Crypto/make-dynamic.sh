#! /usr/bin/env bash

# Written and placed in public domain by Jeffrey Walton
#
# This shel script creates the Visual Studio 2010 project files for
#   dynamic C/C++ runtime linking. Its based on vs2010.zip, which
#   uses static C/C++ runtime linking. Place it in the root of the
#   Crypto++ directory, and run it in place. The output artifact is
#   vs2010-dynamic.zip.

if [ ! -e vs2010.zip ]; then
	echo "vs2010.zip is missing"
	[ "$0" = "$BASH_SOURCE" ] && exit 1 || return 1
fi

unzip -aoq vs2010.zip -d vs2010-dynamic
cd vs2010-dynamic

SED=sed
SEDOPTS=-i

# Use gsed on OS X if available
if [ ! -z $(which gsed 2>/dev/null) ]; then
	SED=$(which gsed)
fi

# Apple sed must use options -i ''
if [ $(uname -s) == "Darwin" ] && [ "$SED" == "/usr/bin/sed" ]; then
	SEDOPTS=(-i "")
fi

PROJ_FILES=($(find . -name "*.vcxproj"))
for pf in ${PROJ_FILES[@]}; do
  "$SED" "${SEDOPTS[@]}" -e 's|<RuntimeLibrary>MultiThreaded</RuntimeLibrary>|<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>|g' $pf
  "$SED" "${SEDOPTS[@]}" -e 's|<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>|<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>|g' $pf
done

# Remove trailing whitespace if unix2dos is available (sed strips the CR and leaves the LF)
if [ ! -z $(which unix2dos 2>/dev/null) ]; then
  "$SED" "${SEDOPTS[@]}" -e 's/[[:space:]]*$//' $(find $PWD -not -path '\.*')
  unix2dos --keepdate --quiet $(find $PWD -not -path '\.*')
fi

# Create a new ZIP; avoid hidden files like .DS_Store and .Spotlight
rm -f vs2010-dynamic.zip 2>/dev/null
zip -j9 vs2010-dynamic.zip $(find $PWD -not -path '\.*') ../make-dynamic.sh
cp vs2010-dynamic.zip ../

cd ..
rm -rf vs2010-dynamic

[ "$0" = "$BASH_SOURCE" ] && exit 0 || return 0
