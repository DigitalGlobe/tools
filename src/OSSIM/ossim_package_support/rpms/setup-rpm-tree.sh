#!/bin/bash

#---
# File: setup-rpm-tree.sh
#---

if [ -z "$OSSIM_DEV_HOME" ]; then
   echo "OSSIM_DEV_HOME environment variable is not set!  Exiting..."
   exit 1
fi

RELEASE_URL=https://svn.osgeo.org/ossim/branches/v1.8.16
TRUNK_URL=https://svn.osgeo.org/ossim/trunk

RELEASE_VERSION=1.8.16
TRUNK_VERSION=1.8.18

# Pick trunk or release:
OSSIM_VERSION=$TRUNK_VERSION
OSSIM_URL=$TRUNK_URL

# Equivalent to: rpmdev-setuptree
mkdir -p $HOME/rpmbuild/BUILD
mkdir -p $HOME/rpmbuild/BUILDROOT
mkdir -p $HOME/rpmbuild/RPMS
mkdir -p $HOME/rpmbuild/SOURCES
mkdir -p $HOME/rpmbuild/SPECS
mkdir -p $HOME/rpmbuild/SRPMS

pushd $HOME/rpmbuild/SOURCES

# OSSIM
# svn co $OSSIM_URL ossim-${OSSIM_VERSION}
# tar cvzf ossim-${OSSIM_VERSION}.tar.gz ossim-${OSSIM_VERSION}

# Kakadu
mkdir -p ossim-kakadu-plugin-${OSSIM_VERSION}
cp -r ossim-${OSSIM_VERSION}/ossim_package_support/cmake/CMakeModules ossim-kakadu-plugin-${OSSIM_VERSION}/.
cp -r ossim-${OSSIM_VERSION}/ossim_plugins/kakadu ossim-kakadu-plugin-${OSSIM_VERSION}/ossim-kakadu-plugin
cp ossim-${OSSIM_VERSION}/ossim_plugins/ossimPluginConstants.h ossim-kakadu-plugin-${OSSIM_VERSION}/.

# Relies on "latest" link to current version:
cp -rL $OSSIM_DEV_HOME/kakadu/latest  ossim-kakadu-plugin-${OSSIM_VERSION}/kakadu

tar cvzf ossim-kakadu-plugin-${OSSIM_VERSION}.tar.gz ossim-kakadu-plugin-${OSSIM_VERSION}

# MrSID
mkdir -p ossim-mrsid-plugin-${OSSIM_VERSION}
cp -r ossim-${OSSIM_VERSION}/ossim_package_support/cmake/CMakeModules ossim-mrsid-plugin-${OSSIM_VERSION}/.
cp -r ossim-${OSSIM_VERSION}/ossim_plugins/mrsid ossim-mrsid-plugin-${OSSIM_VERSION}/ossim-mrsid-plugin
cp ossim-${OSSIM_VERSION}/ossim_plugins/ossimPluginConstants.h ossim-mrsid-plugin-${OSSIM_VERSION}/.

# Relies on "latest" link to current version:
cp -rL $OSSIM_DEV_HOME/mrsid/latest ossim-mrsid-plugin-${OSSIM_VERSION}/mrsid

tar cvzf ossim-mrsid-plugin-${OSSIM_VERSION}.tar.gz ossim-mrsid-plugin-${OSSIM_VERSION}

cp ossim-${OSSIM_VERSION}/ossim_package_support/rpms/ossim.spec ../SPECS/.
cp ossim-${OSSIM_VERSION}/ossim_package_support/rpms/ossim-kakadu-plugin.spec ../SPECS/.
cp ossim-${OSSIM_VERSION}/ossim_package_support/rpms/ossim-mrsid-plugin.spec ../SPECS/.

popd

exit 0
# End


