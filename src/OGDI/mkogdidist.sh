#!/bin/sh
#
# Copyright (C) 2001 Her Majesty the Queen in Right of Canada.
# Permission to use, copy, modify and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright notice appear in all copies, that
# both the copyright notice and this permission notice appear in
# supporting documentation, and that the name of Her Majesty the Queen
# in Right  of Canada not be used in advertising or publicity pertaining
# to distribution of the software without specific, written prior
# permission.  Her Majesty the Queen in Right of Canada makes no
# representations about the suitability of this software for any purpose.
# It is provided "as is" without express or implied warranty.
#

if [ $# -lt 1 ] ; then
  echo "Usage: mkogdidist version"
  echo
  echo "Example: mkogdidist 3.1alpha"
  exit
fi

OGDI_VERSION=$1
DIST_NAME=ogdi-$OGDI_VERSION

rm -rf dist_wrk  
mkdir dist_wrk
cd dist_wrk

git clone https://github.com/libogdi/ogdi devdir

if [ \! -d devdir ] ; then
  echo "git clone reported an error ... abandoning mkogdidist"
  exit
fi

# remove junks
find devdir -name .git -exec rm -rf {} \;

# fix wrongly encoded files from tarball
set +x
for f in `find . -type f` ; do
   if file $f | grep -q ISO-8859 ; then
      set -x
      iconv -f ISO-8859-1 -t UTF-8 $f > ${f}.tmp && \
         mv -f ${f}.tmp $f
      set +x
   fi
   if file $f | grep -q CRLF ; then
      set -x
      sed -i -e 's|\r||g' $f
      set +x
   fi
done
set -x

mv devdir $DIST_NAME

echo "${OGDI_VERSION}: `date`" > $DIST_NAME/VERSION

rm -f ../${DIST_NAME}.tar.gz ../${DIST_NAME}.zip

tar cf ../${DIST_NAME}.tar ${DIST_NAME}
gzip -9 ../${DIST_NAME}.tar
zip -r ../${DIST_NAME}.zip ${DIST_NAME}

cd ..
rm -rf dist_wrk
