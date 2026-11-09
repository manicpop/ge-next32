@echo off
setlocal
rem Called by each MSG custom-build item, before C compilation.
if "%~1"=="" exit /b 1
if not exist "%~2" (
    echo Message compiler not found: %~2
    exit /b 1
)
pushd "%~dp0Dist" || exit /b 1
if not exist "%~1.MSG" goto failed
rem Require fresh outputs, not files left behind by a failed compiler run.
if exist "%~1.h" del /q "%~1.h"
if exist "%~1.MCV" del /q "%~1.MCV"
if exist "%~1.h" goto failed
if exist "%~1.MCV" goto failed
"%~2" "%~1"
if errorlevel 1 goto failed
if not exist "%~1.h" goto failed
if not exist "%~1.MCV" goto failed
move /y "%~1.h" "%~dp0%~1.h" >nul
if errorlevel 1 goto failed
popd
exit /b 0

:failed
echo Header generation failed for %~1.MSG
popd
exit /b 1
