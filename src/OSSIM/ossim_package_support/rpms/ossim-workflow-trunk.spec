Name:          ossim-workflow
Version:        %{RPM_OSSIM_VERSION} 
Release:        %{BUILD_RELEASE}%{?dist}
Summary:        OSSIM Server
Group:          System Environment/Libraries
#TODO: Which version?
License:        LGPLv2+
#URL:            http://github
Source0:        http://download.osgeo.org/ossim/source/%{name}-%{version}.tar.gz

BuildRequires: ant
BuildRequires: java-devel


%description
OSSIM workflow


%prep
#---
# Notes for debugging:
# -D on setup = Do not delete the directory before unpacking.
# -T on setup = Disable the automatic unpacking of the archives.
#---
#%setup -q -D -T
%setup -q


%build

# Exports for ossim build:
export OSSIM_INSTALL_PREFIX=%{buildroot}/usr
export KETTLE_HOME=RPM_KETTLE_HOME

rm -f $KETTLE_HOME/libext/ossim/OSSIMKettle*.jar

export OSSIM_INSTALL_PREFIX=%{buildroot}/usr
ant clean install

%install
# Exports for ossim build:
export OSSIM_INSTALL_PREFIX=%{buildroot}/usr
export OSSIM_VERSION=%{RPM_OSSIM_VERSION}
export KETTLE_HOME=RPM_KETTLE_HOME

pushd ${KETTLE_HOME}
install -p -m644 -D libext/ossim/OSSIMKettle*.jar %{buildroot}%{_datadir}/ossim/workflow
popd
install -p -m644 -D plugins/radiantblue/*.jar %{buildroot}%{_datadir}/ossim/workflow
%files
%{_datadir}

