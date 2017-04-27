textreplace -std -t bin\@package@.bat
textreplace -std -t bin\@package@-browser.bat
textreplace -std -t bin\@package@-designer.bat
textreplace -std -t bin\python-@package@.bat

REM get short path without blanks
for %%i in ("%OSGEO4W_ROOT%") do set O4W_ROOT=%%~fsi
if "%OSGEO4W_DESKTOP%"=="" set OSGEO4W_DESKTOP=~$folder.common_desktop$

if not %OSGEO4W_MENU_LINKS%==0 mkdir "%OSGEO4W_STARTMENU%"
if not %OSGEO4W_MENU_LINKS%==0 nircmd shortcut "%O4W_ROOT%\bin\nircmd.exe" "%OSGEO4W_STARTMENU%" "QGIS Desktop @version@" "exec hide %O4W_ROOT%\bin\@package@.bat" "%O4W_ROOT%\apps\@package@\icons\QGIS.ico"
if not %OSGEO4W_MENU_LINKS%==0 nircmd shortcut "%O4W_ROOT%\bin\nircmd.exe" "%OSGEO4W_STARTMENU%" "QGIS Browser @version@" "exec hide %O4W_ROOT%\bin\@package@-browser.bat" "%O4W_ROOT%\apps\@package@\icons\browser.ico"
if not %OSGEO4W_MENU_LINKS%==0 nircmd shortcut "%O4W_ROOT%\bin\nircmd.exe" "%OSGEO4W_STARTMENU%" "Qt Designer with QGIS @version@ custom widgets" "exec hide """%OSGEO4W_ROOT%\bin\@package@-designer.bat"" "%O4W_ROOT%\apps\@package@\icons\QGIS.ico"

if not %OSGEO4W_DESKTOP_LINKS%==0 mkdir "%OSGEO4W_DESKTOP%"
if not %OSGEO4W_DESKTOP_LINKS%==0 nircmd shortcut "%O4W_ROOT%\bin\nircmd.exe" "%OSGEO4W_DESKTOP%" "QGIS Desktop @version@" "exec hide %O4W_ROOT%\bin\@package@.bat" "%O4W_ROOT%\apps\@package@\icons\QGIS.ico"
if not %OSGEO4W_DESKTOP_LINKS%==0 nircmd shortcut "%O4W_ROOT%\bin\nircmd.exe" "%OSGEO4W_DESKTOP%" "QGIS Browser @version@" "exec hide %O4W_ROOT%\bin\@package@-browser.bat" "%O4W_ROOT%\apps\@package@\icons\browser.ico"
if not %OSGEO4W_DESKTOP_LINKS%==0 nircmd shortcut "%O4W_ROOT%\bin\nircmd.exe" "%OSGEO4W_DESKTOP%" "Qt Designer with QGIS @version@ custom widgets" "exec hide %O4W_ROOT%\bin\@package@-designer.bat" "%O4W_ROOT%\apps\@package@\icons\QGIS.ico"

set OSGEO4W_ROOT=%OSGEO4W_ROOT:\=\\%
textreplace -std -t "%O4W_ROOT%\apps\@package@\bin\qgis.reg"
nircmd elevate "%WINDIR%\regedit" /s "%O4W_ROOT%\apps\@package@\bin\qgis.reg"
