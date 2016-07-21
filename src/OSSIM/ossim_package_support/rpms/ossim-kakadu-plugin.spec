Name:           ossim-kakadu-plugin
Version:        1.8.18
Release:        1%{?dist}
Summary:        OSSIM Kakadu Plugin
Group:          System Environment/Libraries
#TODO: Which version?
License:        Kakadu license restrictions.
URL:            http://trac.osgeo.org/ossim/wiki
Source0:        http://download.osgeo.org/ossim/source/%{name}-%{version}.tar.gz

BuildRequires: cmake
BuildRequires: libcurl-devel
BuildRequires: ossim-devel
BuildRequires: libtiff-devel

%description

OSSIM Kakadu Plugin

%prep
#---
# Notes for debugging: 
# -D on setup = Do not delete the directory before unpacking.
# -T on setup = Disable the automatic unpacking of the archives.
#---
# %setup -q -D -T
%setup -q

%build
export OSSIM_DEV_HOME=%{_builddir}/%{name}-%{version}

# Build kakadu:
pushd kakadu/make
make -f Makefile-Linux-x86-64-gcc
popd

#build plugin:
mkdir -p build
pushd build

# Link with static libs so we don't have to copy/install them.
%cmake \
-DCMAKE_MODULE_PATH=%{_builddir}/%{name}-%{version}/CMakeModules \
-DKAKADU_ROOT_SRC=%{_builddir}/%{name}-%{version}/kakadu \
-DKAKADU_AUX_LIBRARY=%{_builddir}/%{name}-%{version}/kakadu/lib/Linux-x86-64-gcc/libkdu.a \
-DKAKADU_LIBRARY=%{_builddir}/%{name}-%{version}//kakadu/lib/Linux-x86-64-gcc/libkdu_aux.a  \
../ossim-kakadu-plugin

make VERBOSE=1 %{?_smp_mflags}

popd

%install
pushd build
make install DESTDIR=%{buildroot}
popd

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%{_libdir}/ossim/plugins/libossimkakadu_plugin.so

%changelog
* Fri Jan 10 2014 David Burken <dburken@comcast.net> - 1.8.18-1
- Initial package
