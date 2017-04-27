[Setup]
AppName=ossimplanet
AppVerName=ossimplanet version 1.8.4
DefaultDirName={pf}\ossimplanet
DefaultGroupName=ossimplanet
PrivilegesRequired=none

[Files]

Source: C:\Program Files\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\*; DestDir: "{app}"; Flags: replacesameversion
;Source: ..\..\ossim\bin\*.dll; DestDir: "{app}"; Flags:  replacesameversion  recursesubdirs
Source: ..\..\ossim\projects\vs\Release\bin\*.exe; DestDir: "{app}"; Flags:  replacesameversion  recursesubdirs
Source: ..\..\ossim\projects\vs\Release\bin\*.dll; DestDir: "{app}"; Flags:  replacesameversion  recursesubdirs
Source: ..\..\ossim\share\ossim\*.csv; DestDir: "{app}\data"; Flags:  replacesameversion  recursesubdirs
Source: ..\..\ossim\etc\templates\ossim_preferences_template; DestDir: "{app}\templates\"; Flags: replacesameversion
Source: ..\..\ossim_qt4\bin\*.exe; DestDir: "{app}"; Flags:  replacesameversion  recursesubdirs
Source: ..\..\ossim_qt4\lib\*.dll; DestDir: "{app}"; Flags:  replacesameversion  recursesubdirs
Source: ..\..\ossimPlanetQt\lib\*.dll; DestDir: "{app}"; Flags:  replacesameversion  recursesubdirs
Source: ..\..\ossimPlanetQt\apps\ossimplanet\release\*.exe; DestDir: "{app}"; Flags:  replacesameversion  recursesubdirs
Source: ..\ossimPlanetUsers.pdf; DestDir: "{app}\docs"; Flags: replacesameversion
Source: ..\icons\ossimPlanet.ico; DestDir: "{app}\icons"; Flags: replacesameversion
Source: ..\fonts\*.ttf; DestDir: "{app}\fonts"; Flags: replacesameversion
Source: ..\geoids\geoid1996\*.grd; DestDir: "{app}\geoids\geoid1996"; Flags: replacesameversion
Source: ..\images\*.png; DestDir: "{app}\images"; Flags: replacesameversion
Source: ..\images\reference\earth*; DestDir: "{app}\images\reference"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\bin\*.dll; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\bin\*.exe; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\bin\osgplugins\*; DestDir: "{app}\osgplugins"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\src\qt-4.5.1\bin\Qt3Support4.dll; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\src\qt-4.5.1\bin\QtCore4.dll; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\src\qt-4.5.1\bin\QtGui4.dll; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\src\qt-4.5.1\bin\QtNetwork4.dll; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\src\qt-4.5.1\bin\QtOpenGL4.dll; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\src\qt-4.5.1\bin\QtSvg4.dll; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\src\qt-4.5.1\bin\QtSql4.dll; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\src\qt-4.5.1\bin\QtXml4.dll; DestDir: "{app}"; Flags: replacesameversion
Source: ..\..\ossim_dependencies\src\qt-4.5.1\bin\QtXmlPatterns4.dll; DestDir: "{app}"; Flags: replacesameversion

[Icons]
Name: "{group}\{cm:UninstallProgram, ossimplanet}"; Filename: "{uninstallexe}"
Name: "{group}\ossimplanet"; Filename: "{app}\ossimplanet.exe"; WorkingDir: "{app}"; IconFilename: "{app}\icons\ossimPlanet.ico"
Name: "{group}\imagelinker"; Filename: "{app}\imagelinker.exe"; WorkingDir: "{app}"; IconFilename: "{app}\icons\ossimPlanet.ico"
Name: "{group}\iview"; Filename: "{app}\iview.exe"; WorkingDir: "{app}"; IconFilename: "{app}\icons\ossimPlanet.ico"
Name: "{group}\Users Guide"; Filename: "{app}\docs\ossimPlanetUsers.pdf"


[_ISTool]
UseAbsolutePaths=true
