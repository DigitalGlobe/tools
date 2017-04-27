@ECHO OFF

IF (%1) EQU (x86) (
  xcopy /Y /Q /D ..\..\bin\FileGDBAPI*.*
  xcopy /Y /Q /D ..\..\bin\Esri.FileGDB*.*
  GOTO :EOF
)

IF (%1) EQU (x64) (
  xcopy /Y /Q /D ..\..\bin64\FileGDBAPI*.*
  xcopy /Y /Q /D ..\..\bin64\Esri.FileGDB*.*
  GOTO :EOF
)

ECHO Unknown platform (%1).  Failed to copy files
