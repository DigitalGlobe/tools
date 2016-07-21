###
# $Id: qt4env.sh 10735 2007-04-18 14:57:22Z dburken $
# For sh / bash users do: . .qt4env_sh
# The '.' is a dot.
# 
# Edit prefix as needed to point to your qt install directory.
###
qt_prefix=/usr/local/Trolltech/Qt-4.2.3

if ! echo ${PATH} | /bin/grep -q $qt_prefix/bin ; then
   PATH=$qt_prefix/bin:${PATH}
fi

if [ -n $LD_LIBRARY_PATH ] ; then
  if ! echo ${LD_LIBRARY_PATH} | /bin/grep -q $qt_prefix/lib ; then
    LD_LIBRARY_PATH=$qt_prefix/lib:$LD_LIBRARY_PATH
  fi
else
  LD_LIBRARY_PATH=$qt_prefix/lib
fi

QTDIR="$qt_prefix"
QTINC="$qt_prefix/include"
QTLIB="$qt_prefix/lib"

export QTDIR QTINC QTLIB PATH LD_LIBRARY_PATH
