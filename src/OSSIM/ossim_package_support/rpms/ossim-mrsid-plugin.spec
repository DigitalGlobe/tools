Name:           ossim-mrsid-plugin
Version:        1.8.18
Release:        1%{?dist}
Summary:        OSSIM MrSID Plugin
Group:          System Environment/Libraries
#TODO: Which version?
License:        Mrsid license restrictions.
URL:            http://trac.osgeo.org/ossim/wiki
Source0:        http://download.osgeo.org/ossim/source/%{name}-%{version}.tar.gz

BuildRequires: cmake
BuildRequires: java-devel
BuildRequires: libcurl-devel
BuildRequires: libtiff-devel
BuildRequires: ossim-devel

%description

OSSIM Mrsid Plugin

%prep
#---
# Notes for debugging: 
# -D on setup = Do not delete the directory before unpacking.
# -T on setup = Disable the automatic unpacking of the archives.
#---
# %setup -q -D -T
%setup -q

%build
OSSIM_DEV_HOME=%{_builddir}/%{name}-%{version}

#build plugin:
mkdir -p build
pushd build

# Note: Linking static libs.
%cmake \
-DCMAKE_MODULE_PATH=$OSSIM_DEV_HOME/CMakeModules \
-DMRSID_DIR=$OSSIM_DEV_HOME/mrsid \
../ossim-mrsid-plugin

make VERBOSE=1 %{?_smp_mflags}

popd

%install

# ossim mrsid plugin:
pushd build
make install DESTDIR=%{buildroot}
popd

# mrsid libraries:
install -p -m755 -D mrsid/Lidar_DSDK/lib/liblti_lidar_dsdk.so %{buildroot}%{_libdir}
install -p -m755 -D mrsid/Raster_DSDK/lib/libltidsdk.so %{buildroot}%{_libdir}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%{_libdir}/ossim/plugins/libossimmrsid_plugin.so
%{_libdir}/liblti_lidar_dsdk.so
%{_libdir}/libltidsdk.so

%changelog
* Fri Jan 10 2014 David Burken <dburken@comcast.net> - 1.8.18-1
- Initial package
