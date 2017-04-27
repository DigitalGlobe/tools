;  Initial Developer's Public License.
;  The contents of this file are subject to the  Initial Developer's Public
;  License Version 1.0 (the "License"). You may not use this file except
;  in compliance with the License. You may obtain a copy of the License at
;    http://www.ibphoenix.com?a=ibphoenix&page=ibp_idpl
;  Software distributed under the License is distributed on an "AS IS" basis,
;  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
;  for the specific language governing rights and limitations under the
;  License.
;
;  The Original Code is copyright 2001-2003 Paul Reeves for IBPhoenix.
;
;  The Initial Developer of the Original Code is Paul Reeves for IBPhoenix.
;
;  All Rights Reserved.
;
;   Contributor(s):
;     Tilo Muetze, Theo ? and Michael Rimov for improved detection
;     of an existing install directory.
;     Simon Carter for the WinSock2 detection.
;     Philippe Makowski for internationalization and french translation
;     Sergey Nikitin for migrating to ISS v5.

;   Usage Notes:
;
;   This script has been designed to work with Inno Setup v5.3.5
;   (Note: This is the classic version, not the new unicode version.)
;   It is available as a quick start pack from here:
;     http://www.jrsoftware.org/isdl.php#qsp
;
;
;   Known bugs and problems etc etc.
;
;   o The uninstaller can only uninstall what it installed.
;     For instance, if Firebird is installed as a service, but is actually
;     running as an application when the uninstaller is run then the
;     application will not be stopped and the uninstall will not complete
;     cleanly.
;
;   o The uninstaller does not know how to stop multiple instances of a classic
;     server. They must be stopped manually.
;
;
;
#define MyAppPublisher "Firebird Project"
#define MyAppURL "http://www.firebirdsql.org/"
#define MyAppName "Firebird"
#define MyAppId "FBDBServer"

#define FirebirdURL MyAppURL
#define UninstallBinary "{app}\bin\fbserver.exe"

;Hard code some defaults to aid debugging and running script standalone.
;In practice, these values are set in the environment and we use the env vars.
#define MajorVer "2"
#define MinorVer "5"
#define PointRelease "0"
#define BuildNumber "0"
#define PackageNumber "0"
#define FilenameSuffix ""


;-------Start of Innosetup script debug flags section

; if iss_release is undefined then iss_debug is set
; Setting iss_release implies that the defines for files,
; examples and compression are set. If debug is set then the
; iss_debug.inc file controls the settings of files, examples
; and compression.

;A dynamically generated sed script sets the appropriate define
;See BuildExecutableInstall.bat for more details.


;#define iss_debug

#ifndef iss_debug
#define iss_release
#endif

#ifdef iss_release
#define files
#define examples
#define compression
#else
#define iss_debug
#endif

#ifdef iss_debug
;Be more verbose
#pragma option -v+
#pragma verboselevel 9

;Useful for cases where engine is built without examples.
#undef examples
;Useful when testing structure of script - adding files takes time.
#undef files
;We speed up compilation (and hence testing) by not compressing contents.
#undef compression

;Default to x64 for testing
#define PlatformTarget "x64"
#endif


;-------end of Innosetup script debug flags section

;-------Start of Innosetup script


;---- These three defines need a bit of tidying up in the near future,
;     but for now they must stay, as the BuildExecutableInstall.bat
;     uses them.
#define release
#define no_pdb
#define i18n


;------If necessary we can turn off i18n by uncommenting this undefine
;#undef  i18n

;----- If we are debugging the script (and not executed from command prompt)
;----- there is no guarantee that the environment variable exists. However an
;----- expression such as #define FB_MAJOR_VER GetEnv("FB_MAJOR_VER") will
;----- 'define' the macro anyway so we need to test for a valid env var before
;----- we define our macro.
#if Len(GetEnv("FB_MAJOR_VER")) > 0
#define FB_MAJOR_VER GetEnv("FB_MAJOR_VER")
#endif
#ifdef FB_MAJOR_VER
#define MajorVer FB_MAJOR_VER
#endif

#if Len(GetEnv("FB_MINOR_VER")) > 0
#define FB_MINOR_VER GetEnv("FB_MINOR_VER")
#endif
#ifdef FB_MINOR_VER
#define MinorVer FB_MINOR_VER
#endif

#if Len(GetEnv("FB_REV_NO")) > 0
#define FB_REV_NO GetEnv("FB_REV_NO")
#endif
#ifdef FB_REV_NO
#define PointRelease FB_REV_NO
#endif

#if Len(GetEnv("FB_BUILD_NO")) > 0
#define FB_BUILD_NO GetEnv("FB_BUILD_NO")
#endif
#ifdef FB_BUILD_NO
#define BuildNumber FB_BUILD_NO
#endif

#if Len(GetEnv("FBBUILD_PACKAGE_NUMBER")) > 0
#define FBBUILD_PACKAGE_NUMBER GetEnv("FBBUILD_PACKAGE_NUMBER")
#endif
#ifdef FBBUILD_PACKAGE_NUMBER
#define PackageNumber FBBUILD_PACKAGE_NUMBER
#endif

#if Len(GetEnv("FBBUILD_FILENAME_SUFFIX")) > 0
#define FBBUILD_FILENAME_SUFFIX GetEnv("FBBUILD_FILENAME_SUFFIX")
#endif
#ifdef FBBUILD_FILENAME_SUFFIX
#define FilenameSuffix FBBUILD_FILENAME_SUFFIX
#if pos('_',FilenameSuffix) == 0
#define FilenameSuffix "_" + FilenameSuffix
#endif
#endif


#if BuildNumber == "0"
#define MyAppVerString MajorVer + "." + MinorVer + "." + PointRelease
#else
#define MyAppVerString MajorVer + "." + MinorVer + "." + PointRelease + "." + BuildNumber
#endif
#define MyAppVerName MyAppName + " " + MyAppVerString

;---- If we haven't already set PlatformTarget then pick it up from the environment.
#ifndef PlatformTarget
#define PlatformTarget GetEnv("FB_TARGET_PLATFORM")
#endif
#if PlatformTarget == ""
#define PlatformTarget "win32"
#endif

;This location is relative to SourceDir (declared below)
#define FilesDir="output_" + PlatformTarget
#if PlatformTarget == "x64"
#define WOW64Dir="output_win32"
#endif
#define msvc_version 8

;BaseVer should be used for all v2.5.n installs.
;This allows us to upgrade silently from 2.5.m to 2.5.n
#define BaseVer MajorVer + "_" + MinorVer
#define AppVer MajorVer + "_" + MinorVer
#define GroupnameVer MajorVer + "." + MinorVer

;These variables are set in BuildExecutableInstall
#define FB15_cur_ver GetEnv("FBBUILD_FB15_CUR_VER")
#define FB20_cur_ver GetEnv("FBBUILD_FB20_CUR_VER")
#define FB21_cur_ver GetEnv("FBBUILD_FB21_CUR_VER")
#define FB25_cur_ver GetEnv("FBBUILD_FB25_CUR_VER")
#define FB_cur_ver FB25_cur_ver

; We can save space by shipping a pdb package that just includes
; the pdb files. It would then upgrade an existing installation,
; however, it wouldn't work on its own. This practice would go
; against previous policy, which was to make the pdb package a
; complete package. OTOH, now we have 32-bit and 64-bit platforms
; to support we would benefit from slimming down the pdb packages.
;;Uncomment the following lines to build the pdb package
;;with just the pdb files.
;#ifdef ship_pdb
;#undef files
;#undef examples
;#endif


;Some more strings to distinguish the name of final executable
#ifdef ship_pdb
#define pdb_str="_pdb"
#else
#define pdb_str=""
#endif
#ifdef debug
#define debug_str="_debug"
#else
#define debug_str=""
#endif


[Setup]
AppName={#MyAppName}
;The following is important - all ISS install packages should
;duplicate this. See the InnoSetup help for details.
#if PlatformTarget == "x64"
AppID={#MyAppId}_{#BaseVer}_{#PlatformTarget}
#else
AppID={#MyAppId}_{#BaseVer}
#endif
AppVerName={#MyAppVerName} ({#PlatformTarget})
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppVersion={#MyAppVerString}
VersionInfoVersion={#MyAppVerString}

SourceDir=..\..\..\..\
OutputBaseFilename={#MyAppName}-{#MyAppVerString}_{#PackageNumber}_{#PlatformTarget}{#debug_str}{#pdb_str}{#FilenameSuffix}
;OutputManifestFile={#MyAppName}-{#MyAppVerString}_{#PackageNumber}_{#PlatformTarget}{#debug_str}{#pdb_str}{#FilenameSuffix}_Setup-Manifest.txt
OutputDir=builds\install_images
;!!! These directories are as seen from SourceDir !!!
#define ScriptsDir "builds\install\arch-specific\win32"
#define LicensesDir "builds\install\misc"
#define GenDir "gen\Win32\readmes"
LicenseFile={#LicensesDir}\IPLicense.txt

WizardImageFile={#ScriptsDir}\firebird_install_logo1.bmp
WizardSmallImageFile={#ScriptsDir}\firebird_install_logo1.bmp

DefaultDirName={code:ChooseInstallDir|{pf}\Firebird\Firebird_{#AppVer}}
DefaultGroupName=Firebird {#GroupnameVer} ({#PlatformTarget})

UninstallDisplayIcon={code:ChooseUninstallIcon|{#UninstallBinary}}
#ifndef compression
Compression=none
#else
SolidCompression=yes
#endif

ShowUndisplayableLanguages={#defined iss_debug}
AllowNoIcons=true
AlwaysShowComponentsList=true
PrivilegesRequired=admin


#if PlatformTarget == "x64"
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
#endif

;This feature is incomplete, as more thought is required.
#ifdef setuplogging
;New with IS 5.2
SetupLogging=yes
#endif

[Languages]
Name: en; MessagesFile: compiler:Default.isl; InfoBeforeFile: {#GenDir}\installation_readme.txt; InfoAfterFile: {#GenDir}\readme.txt;
#ifdef i18n
Name: ba; MessagesFile: compiler:Languages\Bosnian.isl; InfoBeforeFile: {#GenDir}\ba\Instalacija_ProcitajMe.txt; InfoAfterFile: {#GenDir}\ba\ProcitajMe.txt;
Name: cz; MessagesFile: compiler:Languages\Czech.isl; InfoBeforeFile: {#GenDir}\cz\instalace_ctime.txt; InfoAfterFile: {#GenDir}\cz\ctime.txt;
Name: fr; MessagesFile: compiler:Languages\French.isl; InfoBeforeFile: {#GenDir}\fr\installation_lisezmoi.txt; InfoAfterFile: {#GenDir}\fr\lisezmoi.txt;
Name: de; MessagesFile: compiler:Languages\German.isl; InfoBeforeFile: {#GenDir}\de\installation_liesmich.txt; InfoAfterFile: {#GenDir}\de\liesmich.txt;
Name: es; MessagesFile: compiler:Languages\Spanish.isl; InfoBeforeFile: {#GenDir}\es\leame_instalacion.txt; InfoAfterFile: {#GenDir}\es\leame.txt;
Name: hu; MessagesFile: compiler:Languages\Hungarian.isl; InfoBeforeFile: {#GenDir}\hu\telepitesi_segedlet.txt; InfoAfterFile: {#GenDir}\hu\olvass_el.txt;
Name: it; MessagesFile: compiler:Languages\Italian.isl; InfoBeforeFile: {#GenDir}\it\leggimi_installazione.txt; InfoAfterFile: {#GenDir}\it\leggimi.txt
Name: pl; MessagesFile: compiler:Languages\Polish.isl; InfoBeforeFile: {#GenDir}\pl\instalacja_czytajto.txt; InfoAfterFile: {#GenDir}\pl\czytajto.txt;
Name: pt; MessagesFile: compiler:Languages\Portuguese.isl; InfoBeforeFile: {#GenDir}\pt\instalacao_leia-me.txt; InfoAfterFile: {#GenDir}\pt\leia-me.txt
Name: ru; MessagesFile: compiler:Languages\Russian.isl; InfoBeforeFile: {#GenDir}\ru\installation_readme.txt; InfoAfterFile: {#GenDir}\ru\readme.txt;
;Name: si; MessagesFile: compiler:Languages\Slovenian.isl; InfoBeforeFile: {#GenDir}\si\instalacija_precitajMe.txt; InfoAfterFile: {#GenDir}\readme.txt;
#endif

[Messages]
en.BeveledLabel=English
#ifdef i18n
ba.BeveledLabel=Bosanski
cz.BeveledLabel=Czech
fr.BeveledLabel=Français
de.BeveledLabel=Deutsch
es.BeveledLabel=Español
hu.BeveledLabel=Magyar
it.BeveledLabel=Italiano
pl.BeveledLabel=Polski
pt.BeveledLabel=Português
ru.BeveledLabel=Ðóññêèé
;si.BeveledLabel=Slovenski
#endif

[CustomMessages]
#include "custom_messages.inc"
#ifdef i18n
#include "ba\custom_messages_ba.inc"
#include "cz\custom_messages_cz.inc"
#include "fr\custom_messages_fr.inc"
#include "de\custom_messages_de.inc"
#include "es\custom_messages_es.inc"
#include "hu\custom_messages_hu.inc"
#include "it\custom_messages_it.inc"
#include "pl\custom_messages_pl.inc"
#include "pt\custom_messages_pt.inc"
#include "ru\custom_messages_ru.inc"
;#include "si\custom_messages_si.inc"
#endif

#ifdef iss_debug
; By default, the languages available at runtime depend on the user's
; code page. A user with the Western European code page set will not
; even see that we support installation with the Slovenian language
; for example.
; It can be useful when debugging to force the display of all available
; languages by setting LanguageCodePage to 0. Of course, if the langauge
; is not supported by the user's current code page it be unusable.
[LangOptions]
LanguageCodePage=0
#endif

[Types]
Name: ServerInstall; Description: {cm:ServerInstall}
Name: DeveloperInstall; Description: {cm:DeveloperInstall}
Name: ClientInstall; Description: {cm:ClientInstall}
Name: CustomInstall; Description: {cm:CustomInstall}; Flags: iscustom;

[Components]
Name: ServerComponent; Description: {cm:ServerComponent}; Types: ServerInstall;
Name: ServerComponent\ClassicServerComponent; Description: {cm:ClassicServerComponent}; Types: ServerInstall; Flags: exclusive;
Name: ServerComponent\SuperServerComponent; Description: {cm:SuperServerComponent}; Types: ServerInstall; Flags: exclusive;
Name: DevAdminComponent; Description: {cm:DevAdminComponent}; Types: ServerInstall DeveloperInstall;
Name: ClientComponent; Description: {cm:ClientComponent}; Types: ServerInstall DeveloperInstall ClientInstall CustomInstall; Flags: fixed disablenouninstallwarning;

[Tasks]
;Server tasks
Name: UseGuardianTask; Description: {cm:UseGuardianTask}; Components: ServerComponent\SuperServerComponent; MinVersion: 4.0,4.0; Check: ConfigureFirebird;
Name: UseApplicationTask; Description: {cm:UseApplicationTaskMsg}; GroupDescription: {cm:TaskGroupDescription}; Components: ServerComponent; MinVersion: 4,4; Flags: exclusive; Check: ConfigureFirebird;
Name: UseServiceTask; Description: {cm:UseServiceTask}; GroupDescription: {cm:TaskGroupDescription}; Components: ServerComponent; MinVersion: 0,4; Flags: exclusive; Check: ConfigureFirebird;
Name: AutoStartTask; Description: {cm:AutoStartTask}; Components: ServerComponent; MinVersion: 4,4; Check: ConfigureFirebird;
Name: SuperClassicTask; Description: {cm:SuperClassicTask}; Components: ServerComponent\ClassicServerComponent; MinVersion: 4,4; Flags: unchecked; Check: ConfigureFirebird;
;Allow user to not install cpl applet
Name: InstallCPLAppletTask; Description: {cm:InstallCPLAppletTask}; Components: ServerComponent\SuperServerComponent; MinVersion: 4.0,4.0; Check: ShowInstallCPLAppletTask;
;Name: MenuGroupTask; Description: Create a Menu &Group; Components: DevAdminComponent; MinVersion: 4,4
;Copying of client libs to <sys>
Name: CopyFbClientToSysTask; Description: {cm:CopyFbClientToSysTask}; Components: ClientComponent; MinVersion: 4,4; Flags: Unchecked; Check: ShowCopyFbClientLibTask;
Name: CopyFbClientAsGds32Task; Description: {cm:CopyFbClientAsGds32Task}; Components: ClientComponent; MinVersion: 4,4; Check: ShowCopyGds32Task;


[Run]
#if msvc_version == 8
Filename: msiexec.exe; Parameters: "/qn /i ""{tmp}\vccrt{#msvc_version}_Win32.msi"" /L*v ""{tmp}\vccrt{#msvc_version}_Win32.log"" "; StatusMsg: "Installing MSVC 32-bit runtime libraries to system directory"; Check: HasWI30; Components: ClientComponent;
#if PlatformTarget == "x64"
Filename: msiexec.exe; Parameters: "/qn /i ""{tmp}\vccrt{#msvc_version}_x64.msi"" /L*v ""{tmp}\vccrt{#msvc_version}_x64.log"" ";  StatusMsg: "Installing MSVC 64-bit runtime libraries to system directory"; Check: HasWI30; Components: ClientComponent;
#endif
#endif

;Only register Firebird if we are installing AND configuring
Filename: {app}\bin\instreg.exe; Parameters: "install "; StatusMsg: {cm:instreg}; MinVersion: 4.0,4.0; Components: ClientComponent; Flags: runminimized; Check: ConfigureFirebird;

Filename: {app}\bin\instclient.exe; Parameters: "install fbclient"; StatusMsg: {cm:instclientCopyFbClient}; MinVersion: 4.0,4.0; Components: ClientComponent; Flags: runminimized; Check: CopyFBClientLib;
Filename: {app}\bin\instclient.exe; Parameters: "install gds32"; StatusMsg: {cm:instclientGenGds32}; MinVersion: 4.0,4.0; Components: ClientComponent; Flags: runminimized; Check: CopyGds32
#if PlatformTarget == "x64"
Filename: {app}\WOW64\instclient.exe; Parameters: "install fbclient"; StatusMsg: {cm:instclientCopyFbClient}; MinVersion: 4.0,4.0; Components: ClientComponent; Flags: runminimized 32bit; Check: CopyFBClientLib;
Filename: {app}\WOW64\instclient.exe; Parameters: "install gds32"; StatusMsg: {cm:instclientGenGds32}; MinVersion: 4.0,4.0; Components: ClientComponent; Flags: runminimized 32bit; Check: CopyGds32
#endif

;If on NT/Win2k etc and 'Install and start service' requested
;First, if installing service we must try and remove remnants of old service. Otherwise the new install will fail and when we start the service the old service will be started.
Filename: {app}\bin\instsvc.exe; Parameters: "remove "; StatusMsg: {cm:instsvcSetup}; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask; Check: ConfigureFirebird;
Filename: {app}\bin\instsvc.exe; Parameters: "install {code:ServiceStartFlags} "; StatusMsg: {cm:instsvcSetup}; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask; Check: ConfigureFirebird;
Filename: {app}\bin\instsvc.exe; Description: {cm:instsvcStartQuestion}; Parameters: "start {code:ServiceName} "; StatusMsg: {cm:instsvcStartMsg}; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized postinstall runascurrentuser; Tasks: UseServiceTask; Check: StartEngine
;If 'start as application' requested
Filename: {code:StartApp|{app}\bin\fbserver.exe}; Description: {cm:instappStartQuestion}; Parameters: {code:StartAppParams|' -a '}; StatusMsg: {cm:instappStartMsg}; MinVersion: 0,4.0; Components: ServerComponent; Flags: nowait postinstall; Tasks: UseApplicationTask; Check: StartEngine

;This is a preliminary test of jumping to a landing page. In practice, we are going to need to know the users language and the version number they have installed.
Filename: "{#MyAppURL}/afterinstall"; Description: "After installation - What Next?"; Flags: postinstall shellexec skipifsilent; Components: ServerComponent DevAdminComponent;

[Registry]
;If user has chosen to start as App they may well want to start automatically. That is handled by a function below.
;Unless we set a marker here the uninstall will leave some annoying debris.
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: Firebird; ValueData: ; Flags: uninsdeletevalue; Tasks: UseApplicationTask; Check: ConfigureFirebird;

;This doesn't seem to get cleared automatically by instreg on uninstall, so lets make sure of it
Root: HKLM; Subkey: "SOFTWARE\Firebird Project"; Flags: uninsdeletekeyifempty; Components: ClientComponent DevAdminComponent ServerComponent

;Clean up Invalid registry entries from previous installs.
Root: HKLM; Subkey: "SOFTWARE\FirebirdSQL"; ValueType: none; Flags: deletekey;

;User _may_ be installing over an existing 1.5 install, and it may have been set to run as application on startup
;so we had better delete this entry unless they have chosen to autostart as application
; - except that this seems to be broken. Bah!
;Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\Run"; Valuetype: none; ValueName: 'Firebird'; ValueData: ''; flags: deletevalue; Check: IsNotAutoStartApp;
[Icons]
Name: {group}\Firebird Classic Server; Filename: {app}\bin\fb_inet_server.exe; Parameters: {code:StartAppParams|' -a '}; Flags: runminimized; MinVersion: 4.0,4.0;  Check: InstallClassicServerIcon; IconIndex: 0; Components: ServerComponent\ClassicServerComponent; Comment: {cm:RunCS}
Name: {group}\Firebird SuperClassic Server; Filename: {app}\bin\fb_inet_server.exe; Parameters: {code:StartAppParams|' -a '}; Flags: runminimized; MinVersion: 4.0,4.0;  Check: InstallSuperClassicIcon; IconIndex: 0; Components: ServerComponent\ClassicServerComponent; Comment: {cm:RunSC}
Name: {group}\Firebird SuperServer; Filename: {app}\bin\fbserver.exe; Parameters: {code:StartAppParams|' -a '}; Flags: runminimized; MinVersion: 4.0,4.0;  Check: InstallSuperServerIcon; IconIndex: 0; Components: ServerComponent\SuperServerComponent; Comment: {cm:RunSSNoGuardian}
Name: {group}\Firebird Guardian; Filename: {app}\bin\fbguard.exe; Parameters: {code:StartAppParams|' -a '}; Flags: runminimized; MinVersion: 4.0,4.0;  Check: InstallGuardianIcon; IconIndex: 1; Components: ServerComponent\SuperServerComponent; Comment: {cm:RunSSWithGuardian}
Name: {group}\Firebird ISQL Tool; Filename: {app}\bin\isql.exe; Parameters: -z; WorkingDir: {app}; MinVersion: 4.0,4.0;  Comment: {cm:RunISQL}
Name: {group}\Firebird {#FB_cur_ver} Release Notes; Filename: {app}\doc\Firebird_v{#FB_cur_ver}.ReleaseNotes.pdf; MinVersion: 4.0,4.0; Comment: {#MyAppName} {cm:ReleaseNotes}
#ifdef FB25_FULL_DOCS
;dummy define. We don't yet have full docs, so lets not create links
Name: {group}\Firebird {#FB_cur_ver} Installation Guide; Filename: {app}\doc\Firebird_v{#FB_cur_ver}.InstallationGuide.pdf; MinVersion: 4.0,4.0; Comment: {#MyAppName} {#FB_cur_ver} {cm:InstallationGuide}
Name: {group}\Firebird {#FB_cur_ver} Bug Fixes; Filename: {app}\doc\Firebird_v{#FB_cur_ver}.BugFixes.pdf; MinVersion: 4.0,4.0; Comment: {#MyAppName} {#FB_cur_ver} {cm:BugFixes}
#endif
#define DONT_INCLUDE_FB21_DOCS
#ifndef DONT_INCLUDE_FB21_DOCS
; dummy define. for now (beta 1) we include Fb 2.1 docs.
Name: {group}\Firebird {#FB21_cur_ver} Release Notes; Filename: {app}\doc\Firebird_v{#FB21_cur_ver}.ReleaseNotes.pdf; MinVersion: 4.0,4.0; Comment: {#MyAppName} {cm:ReleaseNotes}
Name: {group}\Firebird {#FB21_cur_ver} Bug Fixes; Filename: {app}\doc\Firebird_v{#FB21_cur_ver}.BugFixes.pdf; MinVersion: 4.0,4.0; Comment: {#MyAppName} {#FB_cur_ver} {cm:BugFixes}
#endif
;And for the time being we include the 2.1 install guide anyway.
Name: {group}\Firebird {#FB21_cur_ver} Installation Guide; Filename: {app}\doc\Firebird_v{#FB21_cur_ver}.InstallationGuide.pdf; MinVersion: 4.0,4.0; Comment: {#MyAppName} {#FB_cur_ver} {cm:InstallationGuide}
Name: {group}\Firebird {#GroupnameVer} Quick Start Guide; Filename: {app}\doc\Firebird-2.5-QuickStart.pdf; MinVersion: 4.0,4.0; Comment: {#MyAppName} {#FB_cur_ver}
Name: "{group}\After Installation"; Filename: "{app}\doc\After_Installation.url"; Comment: "New User? Here's a quick guide to what you should do next."
Name: "{group}\Firebird Web-site"; Filename: "{app}\doc\firebirdsql.org.url"
;Always install the original english version
Name: {group}\{cm:IconReadme,{#FB_cur_ver}}; Filename: {app}\readme.txt; MinVersion: 4.0,4.0;
#ifdef i18n
;And install translated readme.txt if non-default language is chosen.
Name: {group}\{cm:IconReadme,{#FB_cur_ver}}; Filename: {app}\{cm:ReadMeFile}; MinVersion: 4.0,4.0; Components: DevAdminComponent; Check: NonDefaultLanguage;
#endif
Name: {group}\{cm:Uninstall,{#FB_cur_ver}}; Filename: {uninstallexe}; Comment: Uninstall Firebird

[Files]
#ifdef files
Source: {#LicensesDir}\IPLicense.txt; DestDir: {app}; Components: ClientComponent; Flags: sharedfile ignoreversion;
Source: {#LicensesDir}\IDPLicense.txt; DestDir: {app}; Components: ClientComponent; Flags: sharedfile ignoreversion
Source: {#ScriptsDir}\After_Installation.url; DestDir: {app}\doc; Components: ServerComponent DevAdminComponent; Flags: sharedfile ignoreversion
Source: {#ScriptsDir}\firebirdsql.org.url; DestDir: {app}\doc; Components: ServerComponent DevAdminComponent; Flags: sharedfile ignoreversion
;Always install the original english version
Source: {#GenDir}\readme.txt; DestDir: {app}; Components: DevAdminComponent; Flags: ignoreversion;
#ifdef i18n
;Translated files
Source: {#GenDir}\ba\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: ba;
Source: {#GenDir}\cz\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: cz;
Source: {#GenDir}\fr\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: fr;
Source: {#GenDir}\de\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: de;
Source: {#GenDir}\es\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: es;
Source: {#GenDir}\hu\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: hu;
Source: {#GenDir}\it\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: it;
Source: {#GenDir}\pl\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: pl;
Source: {#GenDir}\pt\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: pt;
Source: {#GenDir}\ru\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: ru;
;Source: {#GenDir}\si\*.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: ignoreversion; Languages: si;
#endif
Source: {#FilesDir}\firebird.conf; DestDir: {app}; DestName: firebird.conf.default; Components: ServerComponent; check: FirebirdConfExists;
Source: {#FilesDir}\firebird.conf; DestDir: {app}; DestName: firebird.conf; Components: ServerComponent; Flags: uninsneveruninstall; check: NoFirebirdConfExists
Source: {#FilesDir}\fbtrace.conf; DestDir: {app}; DestName: fbtrace.conf.default; Components: ServerComponent;  check: fbtraceConfExists;
Source: {#FilesDir}\fbtrace.conf; DestDir: {app}; DestName: fbtrace.conf; Components: ServerComponent; Flags: uninsneveruninstall onlyifdoesntexist; check: NofbtraceConfExists;
Source: {#FilesDir}\aliases.conf; DestDir: {app}; Components: ClientComponent; Flags: uninsneveruninstall onlyifdoesntexist
Source: {#FilesDir}\security2.fdb; DestDir: {app}; Components: ServerComponent; Flags: uninsneveruninstall onlyifdoesntexist
Source: {#FilesDir}\firebird.msg; DestDir: {app}; Components: ClientComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\firebird.log; DestDir: {app}; Components: ServerComponent; Flags: uninsneveruninstall skipifsourcedoesntexist external dontcopy

Source: {#FilesDir}\bin\gbak.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\gdef.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: {#FilesDir}\bin\gfix.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\gpre.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: {#FilesDir}\bin\gsec.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\gsplit.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\gstat.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\fbguard.exe; DestDir: {app}\bin; Components: ServerComponent\SuperServerComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\fb_inet_server.exe; DestDir: {app}\bin; Components: ServerComponent\ClassicServerComponent; Flags: sharedfile ignoreversion uninsrestartdelete
Source: {#FilesDir}\bin\fbserver.exe; DestDir: {app}\bin; Components: ServerComponent\SuperServerComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\fb_lock_print.exe; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\ib_util.dll; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\instclient.exe; DestDir: {app}\bin; Components: ClientComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\instreg.exe; DestDir: {app}\bin; Components: ClientComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\instsvc.exe; DestDir: {app}\bin; Components: ServerComponent; MinVersion: 0,4.0; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\isql.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: {#FilesDir}\bin\nbackup.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: {#FilesDir}\bin\qli.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: {#FilesDir}\bin\fbsvcmgr.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: {#FilesDir}\bin\fbtracemgr.exe; DestDir: {app}\bin; Components: DevAdminComponent; Flags: ignoreversion
Source: {#FilesDir}\bin\fbclient.dll; DestDir: {app}\bin; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder
#if PlatformTarget == "x64"
Source: {#WOW64Dir}\bin\fbclient.dll; DestDir: {app}\WOW64; Components: ClientComponent; Flags: overwritereadonly sharedfile promptifolder
Source: {#WOW64Dir}\bin\instclient.exe; DestDir: {app}\WOW64; Components: ClientComponent; Flags: sharedfile ignoreversion
#endif
Source: {#FilesDir}\bin\icuuc30.dll; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\icuin30.dll; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
Source: {#FilesDir}\bin\icudt30.dll; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
#if PlatformTarget == "Win32"
Source: {#FilesDir}\bin\fbrmclib.dll; DestDir: {app}\bin; Components: ServerComponent; Flags: sharedfile ignoreversion
#endif

;Rules for installation of MS runtimes
;MSVC6 and MSVC7
;  Install locally and in <sys> for all versions.
;  NOTE: These dll's MUST never be sourced from the local system32 directory.
;  Deploy libraries from vcredist if MSVC6 is used. Use %FrameworkSDKDir% if compiling with Visual Studio.
;  The BuildExecutableInstall.bat will attempt to locate them and place them in {#FilesDir}\system32\
;
;MSVC8
;  If Win9n, or a WinNT version without Windows Installer 3 or later then install libraries directly into <sys>
;  using normal version info control.
;  If host O/S has Windows Installer 3 or later then use msi.
;MSVC9
;  More info to come
;
#if msvc_version == 6
Source: {#FilesDir}\bin\msvcrt.dll; DestDir: {app}\bin; Components: ClientComponent;
Source: {#FilesDir}\bin\msvcrt.dll; DestDir: {sys}; Components: ClientComponent; Flags: sharedfile onlyifdoesntexist uninsneveruninstall;
#endif
#if msvc_version == 7
Source: {#FilesDir}\bin\msvcr{#msvc_version}?.dll; DestDir: {app}\bin; Components: ClientComponent; Flags: sharedfile;
Source: {#FilesDir}\bin\msvcp{#msvc_version}?.dll; DestDir: {app}\bin; Components: ClientComponent; Flags: sharedfile;
Source: {#FilesDir}\bin\msvcr{#msvc_version}?.dll; DestDir: {sys}; Components: ClientComponent; Flags: sharedfile uninsneveruninstall;
Source: {#FilesDir}\bin\msvcp{#msvc_version}?.dll; DestDir: {sys}; Components: ClientComponent; Flags: sharedfile uninsneveruninstall;
#endif

#if msvc_version >= 8
;If Host O/S has Windows Installer 3.0 installed then we use msiexec to deploy the runtime libraries
;In fact, even if the runtime libraries did exist locally the system will always load a shared assembly if it is available.
Source: {#FilesDir}\bin\msvcr{#msvc_version}?.dll; DestDir: {app}\bin; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile;
Source: {#FilesDir}\bin\msvcp{#msvc_version}?.dll; DestDir: {app}\bin; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile;
Source: {#FilesDir}\bin\Microsoft.VC80.CRT.manifest; DestDir: {app}\bin; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile;
#if PlatformTarget == "x64"
;If we are installing on x64 we need some 32-bit libraries for compatibility with 32-bit applications
Source: {#WOW64Dir}\bin\msvcr{#msvc_version}?.dll; DestDir: {app}\WOW64; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile;
Source: {#WOW64Dir}\bin\msvcp{#msvc_version}?.dll; DestDir: {app}\WOW64; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile;
Source: {#WOW64Dir}\bin\Microsoft.VC80.CRT.manifest; DestDir: {app}\WOW64; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile;
#endif
#endif  /* if msvc_version >= 8 */

#if msvc_version == 8
;Try to install CRT libraries to <sys> via msi, _IF_ msvc_version is 8.
#if PlatformTarget == "x64"
;MinVersion 0,5.0 means no version of Win9x and at least Win2k if NT O/S
;In addition, O/S must have Windows Installer 3.0.
Source: {#FilesDir}\system32\vccrt8_x64.msi; DestDir: {tmp};  Check: HasWI30; MinVersion: 0,5.0; Components: ClientComponent;
Source: {#WOW64Dir}\system32\vccrt8_Win32.msi; DestDir: {tmp}; Check: HasWI30; MinVersion: 0,5.0; Components: ClientComponent;
#else
Source: {#FilesDir}\system32\vccrt8_Win32.msi; DestDir: {tmp}; Check: HasWI30; MinVersion: 0,5.0; Components: ClientComponent;
#endif

;Otherwise, have a go at copying the files into <sys>.
Source: {#FilesDir}\bin\msvcr{#msvc_version}?.dll; DestDir: {sys}; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile uninsneveruninstall;
Source: {#FilesDir}\bin\msvcp{#msvc_version}?.dll; DestDir: {sys}; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile uninsneveruninstall;
Source: {#FilesDir}\bin\Microsoft.VC80.CRT.manifest; DestDir: {sys}; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile uninsneveruninstall;
#if PlatformTarget == "x64"
Source: {#WOW64Dir}\bin\msvcr{#msvc_version}?.dll; DestDir: {syswow64}; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile uninsneveruninstall;
Source: {#WOW64Dir}\bin\msvcp{#msvc_version}?.dll; DestDir: {syswow64}; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile uninsneveruninstall;
Source: {#WOW64Dir}\bin\Microsoft.VC80.CRT.manifest; DestDir: {syswow64}; Check: HasNotWI30; Components: ClientComponent; Flags: sharedfile uninsneveruninstall;
#endif
#endif /* if msvc_version == 8 */

;Docs
Source: {#ScriptsDir}\installation_scripted.txt; DestDir: {app}\doc; Components: DevAdminComponent; Flags: skipifsourcedoesntexist  ignoreversion
Source: {#ScriptsDir}\installation_scripted.txt; DestDir: {tmp}; Flags: DontCopy;
Source: {#FilesDir}\doc\*.*; DestDir: {app}\doc; Components: DevAdminComponent; Flags: skipifsourcedoesntexist  ignoreversion
Source: {#FilesDir}\doc\sql.extensions\*.*; DestDir: {app}\doc\sql.extensions; Components: DevAdminComponent; Flags: skipifsourcedoesntexist ignoreversion

;Other stuff
Source: {#FilesDir}\help\*.*; DestDir: {app}\help; Components: DevAdminComponent; Flags: ignoreversion;
Source: {#FilesDir}\include\*.*; DestDir: {app}\include; Components: DevAdminComponent; Flags: ignoreversion;
Source: {#FilesDir}\intl\fbintl.dll; DestDir: {app}\intl; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: {#FilesDir}\intl\fbintl.conf; DestDir: {app}\intl; Components: ServerComponent; Flags: onlyifdoesntexist
Source: {#FilesDir}\lib\*.*; DestDir: {app}\lib; Components: DevAdminComponent; Flags: ignoreversion;
#if PlatformTarget == "x64"
Source: {#WOW64Dir}\lib\*.lib; DestDir: {app}\WOW64\lib; Components: DevAdminComponent; Flags: ignoreversion
#endif
Source: {#FilesDir}\UDF\ib_udf.dll; DestDir: {app}\UDF; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: {#FilesDir}\UDF\fbudf.dll; DestDir: {app}\UDF; Components: ServerComponent; Flags: sharedfile ignoreversion;
Source: {#FilesDir}\UDF\*.sql; DestDir: {app}\UDF; Components: ServerComponent; Flags: ignoreversion;
Source: {#FilesDir}\UDF\*.txt; DestDir: {app}\UDF; Components: ServerComponent; Flags: ignoreversion;

Source: {#FilesDir}\plugins\*.dll; DestDir: {app}\plugins; Components: ServerComponent; Flags: ignoreversion;

Source: {#FilesDir}\misc\*.*; DestDir: {app}\misc; Components: ServerComponent; Flags: ignoreversion;
Source: {#FilesDir}\misc\upgrade\security\*.*; DestDir: {app}\misc\upgrade\security; Components: ServerComponent; Flags: ignoreversion;
Source: {#FilesDir}\misc\upgrade\ib_udf\*.*; DestDir: {app}\misc\upgrade\ib_udf; Components: ServerComponent; Flags: ignoreversion;
Source: {#FilesDir}\misc\upgrade\metadata\*.*; DestDir: {app}\misc\upgrade\metadata; Components: ServerComponent; Flags: ignoreversion;

;Note - Win9x requires 8.3 filenames for the uninsrestartdelete option to work
Source: {#FilesDir}\system32\Firebird2Control.cpl; DestDir: {sys}; Components: ServerComponent\SuperServerComponent; MinVersion: 0,4.0; Flags: sharedfile ignoreversion promptifolder restartreplace uninsrestartdelete; Check: InstallCPLApplet
Source: {#FilesDir}\system32\Firebird2Control.cpl; DestDir: {sys}; Destname: FIREBI~1.CPL; Components: ServerComponent\SuperServerComponent; MinVersion: 4.0,0; Flags: sharedfile ignoreversion promptifolder restartreplace uninsrestartdelete; Check: InstallCPLApplet
#endif /* files */

#ifdef examples
Source: {#FilesDir}\examples\*.*; DestDir: {app}\examples; Components: DevAdminComponent;  Flags: ignoreversion;
Source: {#FilesDir}\examples\api\*.*; DestDir: {app}\examples\api; Components: DevAdminComponent;  Flags: ignoreversion;
Source: {#FilesDir}\examples\build_win32\*.*; DestDir: {app}\examples\build_win32; Components: DevAdminComponent;  Flags: ignoreversion;
Source: {#FilesDir}\examples\dyn\*.*; DestDir: {app}\examples\dyn; Components: DevAdminComponent;  Flags: ignoreversion;
Source: {#FilesDir}\examples\empbuild\*.*; DestDir: {app}\examples\empbuild; Components: DevAdminComponent;  Flags: ignoreversion;
Source: {#FilesDir}\examples\include\*.*; DestDir: {app}\examples\include; Components: DevAdminComponent;  Flags: ignoreversion;
Source: {#FilesDir}\examples\stat\*.*; DestDir: {app}\examples\stat; Components: DevAdminComponent;  Flags: ignoreversion;
Source: {#FilesDir}\examples\udf\*.*; DestDir: {app}\examples\udf; Components: DevAdminComponent;  Flags: ignoreversion;
#endif

#ifdef ship_pdb
Source: {#FilesDir}\bin\fbclient.pdb; DestDir: {app}\bin; Components: ClientComponent;
Source: {#FilesDir}\bin\fb_inet_server.pdb; DestDir: {app}\bin; Components: ServerComponent\ClassicServerComponent;
Source: {#FilesDir}\bin\fbserver.pdb; DestDir: {app}\bin; Components: ServerComponent\SuperServerComponent;
;Source: {#FilesDir}\bin\fbembed.pdb; DestDir: {app}\bin; Components: ClientComponent;
#if PlatformTarget == "x64"
Source: {#WOW64Dir}\bin\fbclient.pdb; DestDir: {app}\WOW64; Components: ClientComponent;
#endif
#endif

[UninstallRun]
Filename: {app}\bin\instsvc.exe; Parameters: "stop {code:ServiceName} "; StatusMsg: {cm:instsvcStopMsg}; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask; RunOnceId: StopService
Filename: {app}\bin\instsvc.exe; Parameters: "remove {code:ServiceName} "; StatusMsg: {cm:instsvcRemove}; MinVersion: 0,4.0; Components: ServerComponent; Flags: runminimized; Tasks: UseServiceTask; RunOnceId: RemoveService
Filename: {app}\bin\instclient.exe; Parameters: " remove gds32"; StatusMsg: {cm:instclientDecLibCountGds32}; MinVersion: 4.0,4.0; Flags: runminimized;
Filename: {app}\bin\instclient.exe; Parameters: " remove fbclient"; StatusMsg: {cm:instclientDecLibCountFbClient}; MinVersion: 4.0,4.0; Flags: runminimized;
#if PlatformTarget == "x64"
Filename: {app}\wow64\instclient.exe; Parameters: " remove gds32"; StatusMsg: {cm:instclientDecLibCountGds32}; MinVersion: 4.0,4.0; Flags: runminimized 32bit;
Filename: {app}\wow64\instclient.exe; Parameters: " remove fbclient"; StatusMsg: {cm:instclientDecLibCountFbClient}; MinVersion: 4.0,4.0; Flags: runminimized 32bit;
#endif
Filename: {app}\bin\instreg.exe; Parameters: " remove"; StatusMsg: {cm:instreg}; MinVersion: 4.0,4.0; Flags: runminimized; RunOnceId: RemoveRegistryEntry

[UninstallDelete]
Type: files; Name: {app}\*.lck
Type: files; Name: {app}\*.evn
Type: dirifempty; Name: {app}

[_ISTool]
EnableISX=true

[Code]
program Setup;

//Var
//  ProductVersion = '1.5.0';

// Some global variables are also in FirebirdInstallEnvironmentChecks.inc
// This is not ideal, but then this scripting environment is not ideal, either.
// The basic point of the include files is to isolate chunks of code that are
// a) Form a module or have common functionality
// b) Debugged.
// This hopefully keeps the main script simpler to follow.

Var
  InstallRootDir: String;
  FirebirdConfSaved: String;
  ForceInstall: Boolean;        // If /force set on command-line we install _and_
                                // configure. Default is to install and configure only if
                                // no other working installation is found (unless we are installing
                                // over the same version)

  //These three command-line options change the default behaviour
  // during a scripted install
  // They also control whether their associated task checkboxes are displayed
  // during an interactive install
  NoCPL: Boolean;               // pass /nocpl on command-line.
  NoLegacyClient: Boolean;      // pass /nogds32 on command line.
  CopyFbClient: Boolean;        // pass /copyfbclient on command line.

  // Options for scripted uninstall.
  CleanUninstall: Boolean;      // If /clean is passed to the uninstaller it will delete
                                // user config files - firebird.conf, firebird.log,
                                // aliases.conf, fbtrace.conf and the security database.

//  UseSuperClassic: Boolean;     // /UseSuperClassic can be passed on the command-line


#ifdef setuplogging
  OkToCopyLog : Boolean;        // Set when installation is complete.
#endif

#include "FirebirdInstallSupportFunctions.inc"

#include "FirebirdInstallEnvironmentChecks.inc"


function SummarizeInstalledProducts: String;
var
  InstallSummaryArray: TArrayofString;
  product: Integer;
  i: Integer;
  StatusDescription: String;
  InstallSummary: String;
  prodstr: String;
begin

//do nothing gracefully if we are called by accident.
if ProductsInstalledCount = 0 then
  exit;

SetArrayLength(InstallSummaryArray,ProductsInstalledCount);
for product := 0 to MaxProdInstalled -1 do begin
  if (ProductsInstalledArray[product].InstallType <> NotInstalled) then begin
      InstallSummaryArray[i] := Format(ProductsInstalledArray[product].Description, [ProductsInstalledArray[product].ActualVersion]);

    if (ProductsInstalledArray[product].ServerVersion <> '') then begin
      if ((ProductsInstalledArray[product].InstallType AND ClassicServerInstall) = ClassicServerInstall) then
        InstallSummaryArray[i] := InstallSummaryArray[i] + ' '+ExpandConstant('{cm:ClassicServerInstall}')
      else
        InstallSummaryArray[i] := InstallSummaryArray[i] + ' '+ExpandConstant('{cm:SuperServerInstall}')
      end
    else begin
      if (ProductsInstalledArray[product].GBAKVersion <> '') then
        InstallSummaryArray[i] := InstallSummaryArray[i] + ' '+ExpandConstant('{cm:DeveloperInstall}')
      else
        InstallSummaryArray[i] := InstallSummaryArray[i] + ' '+ExpandConstant('{cm:ClientInstall}')
    end;

    if ((ProductsInstalledArray[product].InstallType AND BrokenInstall) = BrokenInstall) then
      InstallSummaryArray[i] := InstallSummaryArray[i]
      + #13 + ExpandConstant('{cm:PreviousInstallBroken}')
    else
      InstallSummaryArray[i] := InstallSummaryArray[i]
      + #13 + ExpandConstant('{cm:PreviousInstallGood}')
    ;

    i:= i+1;
  end;
end;

for i:=0 to ProductsInstalledCount-1 do
  InstallSummary := InstallSummary + InstallSummaryArray[i] + #13;

// If FB21 is installed and installed platform does not match current platform
// then notify user.
#if PlatformTarget == "x64"
If ((ProductsInstalled AND FB21_x64 ) = FB21_x64 ) then
#else
If ((ProductsInstalled AND FB21 ) = FB21 ) then
#endif
      InstallSummary := InstallSummary
      +#13 + ExpandConstant('{cm:InstallSummarySuffix1}')
      +#13 + ExpandConstant('{cm:InstallSummarySuffix2}')
      +#13 + ExpandConstant('{cm:InstallSummarySuffix3}')
      +#13 + ExpandConstant('{cm:InstallSummarySuffix4}')
      +#13
;

if ProductsInstalledCount = 1 then
  StatusDescription := Format(ExpandConstant('{cm:InstalledProducts}'), [IntToStr(ProductsInstalledCount), ExpandConstant('{cm:InstalledProdCountSingular}')])
else
  StatusDescription := Format(ExpandConstant('{cm:InstalledProducts}'), [IntToStr(ProductsInstalledCount), ExpandConstant('{cm:InstalledProdCountPlural}')]);

  Result := StatusDescription
    +#13
    +#13 + InstallSummary
    +#13 + ExpandConstant('{cm:InstallSummaryCancel1}')
    +#13 + ExpandConstant('{cm:InstallSummaryCancel2}')
    +#13
    +#13 + ExpandConstant('{cm:InstallSummaryCancel3}')
    +#13
end;

function AnalysisAssessment: boolean;
var
  MsgText: String;
  MsgResult: Integer;
  VerString: String;
begin
  result := false;

  //We've got all this information. What do we do with it?

  if ProductsInstalledCount = 0 then begin
      result := true;
      exit;
  end;


  //If existing install of the same majorver.minorver is
  //found then we can upgrade it.
  if ( (ProductsInstalledCount = 1) AND
#if PlatformTarget == "x64"
    ((ProductsInstalled AND FB21_x64 ) = FB21_x64 ) ) then begin
#else
    ((ProductsInstalled AND FB21 ) = FB21 ) ) then begin
#endif
        result := true;
        exit;
    end
  ;

  if ForceInstall then begin
    result := true;
    exit;
  end;

  //Otherwise, show user the analysis report.
  MsgText := SummarizeInstalledProducts;
  MsgResult := MsgBox(MsgText,mbConfirmation,MB_YESNO);
  if (MsgResult = IDNO ) then
    result := true;
    //but we don't configure
    if ((InstallAndConfigure AND Configure) = Configure) then
      InstallAndConfigure := InstallAndConfigure - Configure;
end;


function InitializeSetup(): Boolean;
var
  i: Integer;
  CommandLine: String;
begin

  result := true;

  CommandLine:=GetCmdTail;

  if ((pos('HELP',Uppercase(CommandLine)) > 0) or
    (pos('/?',Uppercase(CommandLine)) > 0) or
    (pos('/H',Uppercase(CommandLine)) > 0) ) then begin
    CreateHelpDlg;
    ShowHelpDlg;
    CloseHelpDlg;
    result := False;
    Exit;

  end;

  if pos('FORCE',Uppercase(CommandLine)) > 0 then
    ForceInstall:=True;

  if pos('NOCPL', Uppercase(CommandLine)) > 0 then
    NoCPL := True;

  if pos('NOGDS32', Uppercase(CommandLine)) > 0 then
    NoLegacyClient := True;

  if pos('COPYFBCLIENT', Uppercase(CommandLine)) > 0 then
    CopyFbClient := True;

// To be completed after RC1
//  if pos('USESUPERCLASSIC', Uppercase(CommandLine)) > 0 then
//    UseSuperClassic := True;

  // Check if a server is running - we cannot continue if it is.
  if FirebirdDefaultServerRunning then begin
    result := false;
    exit;
  end;

  if not CheckWinsock2 then begin
    result := False;
    exit;
  end;

  //By default we want to install and confugure,
  //unless subsequent analysis suggests otherwise.
  InstallAndConfigure := Install + Configure;

  InstallRootDir := '';

  InitExistingInstallRecords;
  AnalyzeEnvironment;
  result := AnalysisAssessment;

end;


procedure DeInitializeSetup();
var
  ErrCode: Integer;
begin
  // Did the install fail because winsock 2 was not installed?
  if Winsock2Failure then
    // Ask user if they want to visit the Winsock2 update web page.
    if MsgBox(ExpandConstant('{cm:Winsock2Web1}')+#13#13+ExpandConstant('{cm:Winsock2Web2}'), mbInformation, MB_YESNO) = idYes then
      // User wants to visit the web page
      ShellExec('open', sMSWinsock2Update, '', '', SW_SHOWNORMAL, ewNoWait, ErrCode);

  if RunningServerVerString <> '' then
        MsgBox( #13+Format(ExpandConstant('{cm:FbRunning1}'), [RunningServerVerString])
      +#13
      +#13+ExpandConstant('{cm:FbRunning2}')
      +#13+ExpandConstant('{cm:FbRunning3}')
      +#13, mbError, MB_OK);

#ifdef setuplogging
  if OkToCopyLog then
    FileCopy (ExpandConstant ('{log}'), ExpandConstant ('{app}\InstallationLogFile.log'), FALSE);

  RestartReplace (ExpandConstant ('{log}'), '');
#endif /* setuplogging */

end;

//This function tries to find an existing install of Firebird 2.n
//If it succeeds it suggests that directory for the install
//Otherwise it suggests the default for Fb 2.n
function ChooseInstallDir(Default: String): String;
var
  InterBaseRootDir,
  FirebirdRootDir: String;
begin
  //The installer likes to call this FOUR times, which makes debugging a pain,
  //so let's test for a previous call.
  if (InstallRootDir = '') then begin

    // Try to find the value of "RootDirectory" in the Firebird
    // registry settings. This is either where Fb 1.0 exists or Fb 1.5
    InterBaseRootDir := GetInterBaseDir;
    FirebirdRootDir := GetFirebirdDir;

    if (FirebirdRootDir <> '') and ( FirebirdRootDir = InterBaseRootDir ) then  //Fb 1.0 must be installed so don't overwrite it.
      InstallRootDir := Default;

    if (( InstallRootDir = '' ) and
        ( FirebirdRootDir = Default )) then // Fb 2.0 is already installed,
      InstallRootDir := Default;             // so we offer to install over it

    if (( InstallRootDir = '') and
        ( FirebirdVer[0] = 1 ) and ( FirebirdVer[1] = 5 ) ) then   // Firebird 1.5 is installed
      InstallRootDir := Default;                                   // but the user has changed the default

    if (( InstallRootDir = '') and
        ( FirebirdVer[0] = {#MajorVer} ) and ( FirebirdVer[1] = {#MinorVer} ) ) then   // Firebird 2.n is installed
      InstallRootDir := FirebirdRootDir;                            // but the user has changed the default

    // if we haven't found anything then try the FIREBIRD env var
    // User may have preferred location for Firebird, but has possibly
    // uninstalled previous version
    if (InstallRootDir = '') then
      InstallRootDir:=getenv('FIREBIRD');

    //if no existing locations found make sure we default to the default.
    if (InstallRootDir = '') then
      InstallRootDir := Default;

  end; // if InstallRootDir = '' then begin

  Result := ExpandConstant(InstallRootDir);

end;


function ServiceStartFlags(Default: String): String;
var
  ServerType: String;
  SvcParams: String;
  InstanceName: String;
begin
  servertype := '';
  SvcParams := '';
  if ClassicInstallChosen then
    ServerType := ' -classic '
  else
    ServerType := ' -superserver ';

  if IsComponentSelected('ServerComponent') and IsTaskSelected('AutoStartTask') then
    SvcParams := ' -auto '
  else
    SvcParams := ' -demand ';

  if IsComponentSelected('ServerComponent') and IsTaskSelected('UseGuardianTask') then
    SvcParams := SvcParams + ServerType + ' -guardian'
  else
    SvcParams := SvcParams + ServerType;

  if IsComponentSelected('ServerComponent') and IsTaskSelected('SuperClassicTask') then
    SvcParams := ' -multithreaded ';

  InstanceName := ' -n DefaultInstance'

  SvcParams := SvcParams + InstanceName;

  Result := SvcParams;
end;


function ServiceName(Default: String): String;
begin
    Result := ' -n DefaultInstance' ;
end;




function InstallGuardianIcon(): Boolean;
begin
  result := false;
  if IsComponentSelected('ServerComponent') and IsTaskSelected('UseApplicationTask') then
    if IsComponentSelected('ServerComponent') and IsTaskSelected('UseGuardianTask') then
      result := true;
end;

function InstallSuperServerIcon(): Boolean;
begin
  result := false;
  if IsComponentSelected('ServerComponent') and IsTaskSelected('UseApplicationTask') then
    if NOT (IsComponentSelected('ServerComponent') and IsTaskSelected('UseGuardianTask')) then
      result := true;
end;

function InstallSuperClassicIcon(): Boolean;
begin
  result := false;
  if IsComponentSelected('ServerComponent') and IsTaskSelected('UseApplicationTask') then
    if IsTaskSelected('SuperClassicTask') then
      result := true;
end;

function InstallClassicServerIcon(): Boolean;
begin
  result := false;
  if IsComponentSelected('ServerComponent') and IsTaskSelected('UseApplicationTask') then
    if NOT IsTaskSelected('SuperClassicTask') then
      result := true;
end;



function StartApp(Default: String): String;
begin
  if IsComponentSelected('ServerComponent') and IsTaskSelected('UseGuardianTask') then
    Result := GetAppPath+'\bin\fbguard.exe'
  else
    if ClassicInstallChosen then
      Result := GetAppPath+'\bin\fb_inet_server.exe'
    else
      Result := GetAppPath+'\bin\fbserver.exe';
end;

function StartAppParams(Default: String): String;
begin
  Result := ' -a ';
  if IsTaskSelected('SuperClassicTask') then
    Result := Result + ' -m ';
end;

function IsNotAutoStartApp: boolean;
//Support function to help remove unwanted registry entry.
begin
  result := true;
  if ( IsComponentSelected('ServerComponent') and IsTaskSelected('AutoStartTask') ) and
    ( IsComponentSelected('ServerComponent') and IsTaskSelected('UseApplicationTask') ) then
    result := false;
end;


procedure RemoveSavedConfIfNoDiff;
//Compare firebird.conf with the one we just saved.
//If they match then we can remove the saved one.
var
  FirebirdConfStr: String;
  SavedConfStr: String;
begin
  LoadStringFromFile( GetAppPath+'\firebird.conf', FirebirdConfStr );
  LoadStringFromFile( FirebirdConfSaved, SavedConfStr );

  if CompareStr( FirebirdConfStr, SavedConfStr ) = 0 then
    DeleteFile( FirebirdConfSaved );
end;

procedure UpdateFirebirdConf;
// Update firebird conf. If user has deselected the guardian we should update
// firebird.conf accordingly. Otherwise we leave the file unchanged.
var
  fbconf: TArrayOfString;
  i, position: Integer;
  ArraySize: Integer;
  FileChanged: boolean;
begin
  //There is no simple, foolproof and futureproof way to check whether
  //we are doing a server install, so the easiest way is to see if a
  //firebird.conf exists. If it doesn't then we don't care.
  if FileExists(GetAppPath+'\firebird.conf') then begin
    if NOT (IsComponentSelected('ServerComponent') and IsTaskSelected('UseGuardianTask')) then
      ReplaceLine(GetAppPath+'\firebird.conf','GuardianOption','GuardianOption = 0','#');
  end;
end;


function NonDefaultLanguage: boolean;
//return true if language other than default is chosen
begin
  result := (ActiveLanguage <> 'en');
end;


procedure CurPageChanged(CurPage: Integer);
begin
  case CurPage of
    wpInfoBefore:   WizardForm.INFOBEFOREMEMO.font.name:='Courier New';
    wpInfoAfter:    WizardForm.INFOAFTERMEMO.font.name:='Courier New';
    wpSelectTasks:  WizardForm.TASKSLIST.height := WizardForm.TASKSLIST.height+30;
    wpFinished:     ; // Create some links to Firebird and IBP here?.
  end;
end;


procedure CurStepChanged(CurStep: TSetupStep);
var
  AppStr: String;
  ReadMeFileStr: String;
begin
   case CurStep of
    ssInstall: begin
              SetupSharedFilesArray;
              GetSharedLibCountBeforeCopy;
      end;

    ssPostInstall: begin
      //Manually set the sharedfile count of these files.
      IncrementSharedCount(Is64BitInstallMode, GetAppPath+'\firebird.conf', false);
      IncrementSharedCount(Is64BitInstallMode, GetAppPath+'\firebird.log', false);
      IncrementSharedCount(Is64BitInstallMode, GetAppPath+'\aliases.conf', false);
      IncrementSharedCount(Is64BitInstallMode, GetAppPath+'\fbtrace.conf', false);
      IncrementSharedCount(Is64BitInstallMode, GetAppPath+'\security2.fdb', false);
      end;

    ssDone: begin
      //If user has chosen to install an app and run it automatically set up the registry accordingly
      //so that the server or guardian starts evertime they login.
      if (IsComponentSelected('ServerComponent') and IsTaskSelected('AutoStartTask') ) and
              ( IsComponentSelected('ServerComponent') and IsTaskSelected('UseApplicationTask') ) then begin
        AppStr := StartApp('') + StartAppParams(' -a ');
        RegWriteStringValue (HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Run', 'Firebird', AppStr);
      end;

      //Fix up conf file
      RemoveSavedConfIfNoDiff;
      UpdateFirebirdConf;

      //Reset shared library count if necessary
      CheckSharedLibCountAtEnd;

      //Move lang specific readme from doc dir to root of install.
      if NonDefaultLanguage then begin
        ReadMeFileStr := ExpandConstant('{cm:ReadMeFile}');
        if FileCopy(GetAppPath+'\doc\'+ReadMeFileStr, GetAppPath+'\'+ReadMeFileStr, false) then
          DeleteFile(GetAppPath+'\doc\'+ReadMeFileStr);
      end;

#ifdef setuplogging
      OkToCopyLog := True;
#endif

    end;
  end;
end;

function StartEngine: boolean;
begin
  if ConfigureFirebird then
    result := not FirebirdDefaultServerRunning;
end;


function ChooseUninstallIcon(Default: String): String;
begin
  if ClassicInstallChosen then
    result := GetAppPath+'\bin\fb_inet_server.exe'
  else
    result := GetAppPath+'\bin\fbserver.exe';

end;

//InnoSetup has a Check Parameter that allows installation if the function returns true.
//For firebird.conf we want to do two things:
// o if firebird.conf already exists then install firebird.conf.default
// o if firebird.conf does not exist then install firebird.conf
//
// This double test is also needed because the uninstallation rules are different for each file.
// We never uninstall firebird.conf. We always uninstall firebird.conf.default.

function FirebirdConfExists: boolean;
begin
  Result := fileexists(GetAppPath+'\firebird.conf');
end;

function NoFirebirdConfExists: boolean;
begin
  Result := not fileexists(GetAppPath+'\firebird.conf');
end;

function fbtraceConfExists: boolean;
begin
  Result := fileexists(GetAppPath+'\fbtrace.conf');
end;

function NofbtraceConfExists: boolean;
begin
  Result := not fileexists(GetAppPath+'\fbtrace.conf');
end;

function InitializeUninstall(): Boolean;
var
  CommandLine: String;
begin
  CommandLine:=GetCmdTail;
  if pos('CLEAN',Uppercase(CommandLine))>0 then
    CleanUninstall:=True
  else
    CleanUninstall:=False;

  //MUST return a result of true, otherwise uninstall will abort!
  result := true;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin

  case CurUninstallStep of

//    usAppMutexCheck :

//    usUninstall :

    usPostUninstall : begin
      // We are manually handling the share count of these files, so we must
      // a) Decrement shared count of each one and
      // b) If Decrement reaches 0 (ie, function returns true) then we
      //    test if CleanUninstall has been passed.
      if DecrementSharedCount(Is64BitInstallMode, GetAppPath+'\firebird.conf') then
        if CleanUninstall then
          DeleteFile(GetAppPath+'\firebird.conf');

      if DecrementSharedCount(Is64BitInstallMode, GetAppPath+'\firebird.log') then
        if CleanUninstall then
          DeleteFile(GetAppPath+'\firebird.log');

      if DecrementSharedCount(Is64BitInstallMode, GetAppPath+'\aliases.conf') then
        if CleanUninstall then
          DeleteFile(GetAppPath+'\aliases.conf');

      if DecrementSharedCount(Is64BitInstallMode, GetAppPath+'\fbtrace.conf') then
        if CleanUninstall then
          DeleteFile(GetAppPath+'\fbtrace.conf');

      if DecrementSharedCount(Is64BitInstallMode, GetAppPath+'\security2.fdb') then
        if CleanUninstall then
          DeleteFile(GetAppPath+'\security2.fdb');

      end;

//    usDone :

  end;


end;


begin
end.
