Name:           ossim
Version:        1.8.18
Release:        1%{?dist}
Summary:        Open Source Software Image Map library and command line applications
Group:          System Environment/Libraries
#TODO: Which version?
License:        LGPLv2+
URL:            http://trac.osgeo.org/ossim/wiki
Source0:        http://download.osgeo.org/ossim/source/%{name}-%{version}.tar.gz

BuildRequires: ant
BuildRequires: cmake
BuildRequires: gdal-devel
BuildRequires: geos-devel
BuildRequires: hdf-devel
BuildRequires: hdf5-devel
BuildRequires: java-devel
BuildRequires: libcurl-devel
BuildRequires: libgeotiff-devel
BuildRequires: libjpeg-devel
BuildRequires: libpng-devel
# BuildRequires: libraw-devel
BuildRequires: minizip-devel
BuildRequires: OpenSceneGraph-devel
BuildRequires: OpenThreads-devel
BuildRequires: podofo-devel
BuildRequires: qt4-devel
BuildRequires: swig

%description
OSSIM is a powerful suite of geospatial libraries and applications
used to process remote sensing imagery, maps, terrain, and vector data.

%package 	    devel
Summary:        Develelopment files for ossim
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description devel
Development files for ossim.

%package 	    gdal-plugin
Summary:        GDAL ossim plugin
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description gdal-plugin
This sub-package contains the gdal ossim plugin for reading/writing images
supported by the gdal library.

%package 	    geocell
Summary:        Desktop electronic light table
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description geocell
Desktop electronic light table for geospatial image processing. Has 2D, 2 1/2D
and 3D viewer with image chain editing capabilities.

%package 	    geopdf-plugin
Summary:        geopdf ossim plugin
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description geopdf-plugin
This sub-package contains the geopdf ossim plugin for reading geopdf files via
the podofo library.

%if 0
%package 	    hdf-plugin
Summary:        HDF ossim plugin
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description hdf-plugin
This sub-package contains the Hierarchical Data Format(hdf) ossim plugin for
reading hdf4 and hdf5 images via the hdf4 and hdf5 libraries.
%endif

%package  	    kmlsuperoverlay-plugin
Summary:        kmlsuperoverlay ossim plugin
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description kmlsuperoverlay-plugin
This sub-package contains the kmlsuperoverlay ossim plugin for reading/writing
kml super overlays.

%package  	    las-plugin
Summary:        LAS ossim plugin
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description las-plugin
This sub-package contains the las ossim plugin for reading ASPRS LASer(LAS)
data.  Limited support for version 1.2.

# Removing until code changed to use external libraw package.(drb)
%if 0
%package  	    libraw-plugin
Summary:        libraw ossim plugin
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description libraw-plugin
This sub-package contains the libraw ossim plugin for reading data via the
libraw library.
%endif

# libwms does not depend on ossim
%package -n     libwms
Summary:        wms ossim library
Group:          System Environment/Libraries

%description -n libwms
This sub-package contains the web mapping service (wms) library.

%package -n 	libwms-devel
Summary:        Development files libwms
Group:          System Environment/Libraries
Requires:       libwms%{?_isa} = %{version}-%{release}

%description -n libwms-devel
This sub-package contains the development files for libwms.

%package  	    ndf-plugin
Summary:        ndf ossim plugin
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description ndf-plugin
This sub-package contains the ndf ossim plugin for reading National Landsat
Archive Production System (NLAPS) Data Format (NDF).

%package -n	    liboms
Summary:        Wrapper library/java bindings for interfacing with ossim.
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description -n liboms
This sub-package contains the oms wrapper library with java bindings for
interfacing with the ossim library from java.

%package -n	    liboms-devel
Summary:        Development files for ossim oms wrapper library.
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description -n liboms-devel
This sub-package contains the development files for oms.

%package 	    ossim-plugin
Summary:        Plugin with various SAR sensor models
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description ossim-plugin
This sub-package contains the ossim plugin which has various SAR sensor models,
readers, and support data parsers.  Most of this code was provided by the ORFEO
Toolbox (OTB) group.

%package 	    ossimjni
Summary:        Wrapper library/java bindings for interfacing with ossim
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description ossimjni
This sub-package contains the ossimjni wrapper library with java bindings for
interfacing with the ossim library from java.  This is a re-write of the oms
sub-package.

%package 	    ossimjni-devel
Summary:        Development files for ossimjni wrapper library.
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description ossimjni-devel
This sub-package contains the development files for ossimjni.

%package 	    planet
Summary:        3D ossim library interface via OpenSceneGraph
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description planet
3D ossim library interface via OpenSceneGraph.

%package 	    planet-devel
Summary:        Development files for ossim planet.
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description planet-devel
This sub-package contains development files for ossim planet.

%package 	    png-plugin
Summary:        PNG ossim plugin
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description png-plugin
This sub-package contains the Portable Network Graphic (png) ossim plugin for
reading/writing png images via the png library. 

%package 	    predator
Summary:        Ossim vedeo library.
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description predator
Ossim vedeo library.

%package 	    predator-devel
Summary:        Development files for ossim planet.
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description predator-devel
This sub-package contains development files for ossim planet.

%package 	    web-plugin
Summary:        web ossim plugin
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description web-plugin
This sub-package contains the web ossim plugin for interfacing with http via
curl library. 


%prep

#---
# Notes for debugging: 
# -D on setup = Do not delete the directory before unpacking.
# -T on setup = Disable the automatic unpacking of the archives.
#---
# %setup -q -D -T
%setup -q

# Delete bundled libraw
rm -rf ossim_plugins/libraw/LibRaw-0.9.0/


%build

# Exports for ossim build:
export OSSIM_DEV_HOME=%{_builddir}/%{name}-%{version}

mkdir -p build
pushd build
%cmake \
-DBUILD_CSMAPI=OFF \
-DBUILD_OMS=ON \
-DBUILD_OSSIM=ON \
-DBUILD_OSSIMCSM_PLUGIN=OFF \
-DBUILD_OSSIM_PLUGIN=ON  \
-DBUILD_OSSIMGEOPDF_PLUGIN=ON \
-DBUILD_OSSIMGDAL_PLUGIN=ON \
-DBUILD_OSSIMHDF_PLUGIN=OFF \
-DBUILD_OSSIMJNI=ON \
-DBUILD_OSSIMKMLSUPEROVERLAY_PLUGIN=ON \
-DBUILD_OSSIMLAS_PLUGIN=ON \
-DBUILD_OSSIMLIBRAW_PLUGIN=OFF \
-DBUILD_OSSIMNDF_PLUGIN=ON \
-DBUILD_OSSIMPNG_PLUGIN=ON \
-DBUILD_OSSIMWEB_PLUGIN=ON \
-DBUILD_OSSIMGUI=ON \
-DBUILD_OSSIM_MPI_SUPPORT=OFF \
-DBUILD_OSSIMPLANET=ON \
-DBUILD_OSSIMPLANETQT=ON \
-DBUILD_OSSIMPREDATOR=ON \
-DBUILD_OSSIMQT4=OFF \
-DBUILD_OSSIM_TEST_APPS=OFF \
-DBUILD_WMS=ON \
-DCMAKE_MODULE_PATH=$OSSIM_DEV_HOME/ossim_package_support/cmake/CMakeModules \
-DOSSIMPLANET_ENABLE_EPHEMERIS=OFF \
../ossim_package_support/cmake
make VERBOSE=1 %{?_smp_mflags}
popd

# Exports for java builds:
export JAVA_HOME=/usr/lib/jvm/java
export OSSIM_INSTALL_PREFIX=%{buildroot}/usr

# Build c++ jni bindings and java side of oms module:
pushd oms/joms
cp local.properties.template local.properties
ant
popd

# Build c++ jni bindings and java side of ossimjni module:
pushd ossimjni/java
cp local.properties.template local.properties
cp $OSSIM_DEV_HOME/oms/joms/util/*.jar util/.
ant
popd

%install

# Exports for ossim build:
export OSSIM_DEV_HOME=%{_builddir}/%{name}-%{version}

pushd build
make install DESTDIR=%{buildroot}
popd

install -p -m644 -D ossim/etc/linux/profile.d/ossim.sh %{buildroot}%{_sysconfdir}/profile.d/ossim.sh
install -p -m644 -D ossim/etc/linux/profile.d/ossim.csh %{buildroot}%{_sysconfdir}/profile.d/ossim.csh
install -p -m644 -D ossim/etc/templates/ossim_preferences_template %{buildroot}%{_datadir}/ossim/ossim_preferences

# Exports for java builds:
export JAVA_HOME=/usr/lib/jvm/java
export OSSIM_INSTALL_PREFIX=%{buildroot}/usr

# oms "ant" build:
pushd oms/joms
ant install
# Fix bad perms:
chmod 755 %{buildroot}%{_libdir}/libjoms.so
popd

# ossimjni "ant" build:
pushd ossimjni/java
ant install
# Fix bad perms:
chmod 755 %{buildroot}%{_libdir}/libossimjni-swig.so
popd

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%post -n libwms -p /sbin/ldconfig
%postun -n libwms -p /sbin/ldconfig

%post -n liboms -p /sbin/ldconfig
%postun -n liboms -p /sbin/ldconfig

%post planet -p /sbin/ldconfig
%postun planet -p /sbin/ldconfig

%files
%{_bindir}/*
%{_datadir}/ossim/
%doc ossim/LICENSE.txt
%{_libdir}/libossim.so.*
%{_sysconfdir}/profile.d/ossim.sh
%{_sysconfdir}/profile.d/ossim.csh

# Weed out apps:
%exclude %{_bindir}/ossim-adrg-dump
%exclude %{_bindir}/ossim-btoa
%exclude %{_bindir}/ossim-chgkwval
%exclude %{_bindir}/ossim-computeSrtmStats
%exclude %{_bindir}/ossim-correl 
%exclude %{_bindir}/ossim-create-bitmask
%exclude %{_bindir}/ossim-dump-ocg
%exclude %{_bindir}/ossim-image-compare
%exclude %{_bindir}/ossim-modopt
%exclude %{_bindir}/ossimplanet
%exclude %{_bindir}/ossimplanetklv
%exclude %{_bindir}/ossimplanet-chip
%exclude %{_bindir}/ossimplanettest
%exclude %{_bindir}/ossim-rejout
%exclude %{_bindir}/ossim-rpf 
%exclude %{_bindir}/ossim-senint
%exclude %{_bindir}/ossim-space-imaging
%exclude %{_bindir}/ossim-src2src
%exclude %{_bindir}/ossim-swapbytes
%exclude %{_bindir}/ossim-ws-cmp

# These are in the geocell package:
%exclude %{_bindir}/ossim-geocell
%exclude %{_bindir}/ossimplanetviewer

%files devel
%{_includedir}/ossim
%{_libdir}/libossim.so
%{_libdir}/pkgconfig/ossim.pc

%files gdal-plugin
%{_libdir}/ossim/plugins/libossimgdal_plugin.so

%files geocell
%{_bindir}/ossim-geocell
%{_libdir}/libossimGui.so*

%files geopdf-plugin
%{_libdir}/ossim/plugins/libossimgeopdf_plugin.so

%if 0
%files hdf-plugin
%{_libdir}/ossim/plugins/libossimhdf_plugin.so
%endif

%files kmlsuperoverlay-plugin
%{_libdir}/ossim/plugins/libossimkmlsuperoverlay_plugin.so

%files las-plugin
%{_libdir}/ossim/plugins/libossimlas_plugin.so

# Removing until code changed to use external libraw package.(drb)
%if 0
%files libraw-plugin
%{_libdir}/ossim/plugins/libossimlibraw_plugin.so
%endif

%files -n libwms
%{_libdir}/libwms.so.*

%files -n libwms-devel
%{_includedir}/wms/
%{_libdir}/libwms.so

%files ndf-plugin
%{_libdir}/ossim/plugins/libossimndf_plugin.so

%files -n liboms
%{_javadir}/joms.jar
%{_libdir}/libjoms.so
%{_libdir}/liboms.so.*

%files -n liboms-devel
%{_includedir}/oms/
%{_libdir}/liboms.so

%files ossim-plugin
%{_libdir}/ossim/plugins/libossim_plugin.so

%files ossimjni
%{_javadir}/ossimjni.jar
%{_libdir}/libossimjni.so*
%{_libdir}/libossimjni-swig.so

%files ossimjni-devel
%{_includedir}/ossimjni

%files planet
%{_bindir}/ossimplanet
%{_bindir}/ossimplanetviewer
%{_libdir}/libossimPlanet.so*
%{_libdir}/libossimPlanetQt.so*

%files planet-devel
%{_includedir}/ossimPlanet

%files png-plugin
%{_libdir}/ossim/plugins/libossimpng_plugin.so

%files predator
%{_libdir}/libossimPredator.so*

%files predator-devel
%{_includedir}/ossimPredator

%files web-plugin
%{_libdir}/ossim/plugins/libossimweb_plugin.so

%changelog
* Fri Jan 10 2014 David Burken <dburken@comcast.net> - 1.8.18-1
- Somewhat working version.
* Sun Dec 29 2013 Volker Fröhlich <volker27@gmx.at> - 1.8.18-1
- Initial package
