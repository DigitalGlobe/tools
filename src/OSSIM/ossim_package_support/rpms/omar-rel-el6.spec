Name:          omar
Version:        %{RPM_OSSIM_VERSION} 
Release:        %{BUILD_RELEASE}%{?dist}
Summary:        OSSIM Server
Group:          System Environment/Libraries
#TODO: Which version?
License:        LGPLv2+
#URL:            http://github
Source0:        http://download.osgeo.org/ossim/source/%{name}-%{version}.tar.gz

BuildRequires: liboms
BuildRequires: ossim
BuildRequires: ossim-predator

%description
OMAR

%package -n	    omar-server
Summary:        Wrapper library/java bindings for interfacing with ossim.
Group:          System Environment/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description -n omar-server
OMAR Image server

%prep
#---
# Notes for debugging:
# -D on setup = Do not delete the directory before unpacking.
# -T on setup = Disable the automatic unpacking of the archives.
#---
#%setup -q -D -T
%setup -q

# Delete bundled libraw
#rm -rf ossim_plugins/libraw/LibRaw-0.9.0/


%build

# Exports for ossim build:
export OMAR_DEV_HOME=%{_builddir}/%{name}-%{version}
export OMAR_HOME=$OMAR_DEV_HOME/apps/omar
export OSSIM_BUILD_DIR=%{_builddir}/%{name}-%{version}/build
export OSSIM_BUILD_TYPE=RelWithDebInfo
export OSSIM_VERSION=%{RPM_OSSIM_VERSION}
export OSSIM_INSTALL_PREFIX=%{buildroot}/usr

mkdir -p plugins/omar-oms/lib
cp /usr/share/java/joms-%{version}.jar plugins/omar-oms/lib/joms-%{version}.jar

export OSSIM_INSTALL_PREFIX=%{buildroot}/usr
#./package-omar-plugins.sh
pushd apps/omar
grails refresh-dependencies --non-interactive
grails prod compile
grails prod war omar.war
popd

%install
# Exports for ossim build:
#export OSSIM_DEV_HOME=%{_builddir}/%{name}-%{version}
export OMAR_DEV_HOME=%{_builddir}/%{name}-%{version}
export OMAR_HOME=$OMAR_DEV_HOME/apps/omar
export OMAR_HOME=%{_builddir}/%{name}-%{version}/apps/omar
export OSSIM_VERSION=%{RPM_OSSIM_VERSION}
export OSSIM_INSTALL_PREFIX=%{buildroot}/usr
#export OSSIM_BUILD_DIR=%{_builddir}/%{name}-%{version}/build
#export OSSIM_BUILD_TYPE=RelWithDebInfo

pushd ${OMAR_HOME}
install -p -m644 -D omar.war %{buildroot}%{_datadir}/omar/omar.war
popd
install -p -m644 -D $OMAR_DEV_HOME/support/omarConfig.groovy %{buildroot}%{_datadir}/omar/omarConfig-template.groovy

%files
%{_datadir}/*
