@echo off
setlocal

set "START_DIR=%~dp0"
set "SEARCH_DIR=%START_DIR%"

:find_root
if exist "%SEARCH_DIR%tools\psx\mod-builder\main.py" goto found_root
for %%I in ("%SEARCH_DIR%..") do set "PARENT_DIR=%%~fI\"
if /I "%PARENT_DIR%"=="%SEARCH_DIR%" goto missing_root
set "SEARCH_DIR=%PARENT_DIR%"
goto find_root

:found_root
pushd "%START_DIR%" >nul
set "CTR_MODSDK_ROOT=%SEARCH_DIR:~0,-1%"
uv run --active --project "%CTR_MODSDK_ROOT%" python "%CTR_MODSDK_ROOT%\tools\psx\mod-builder\main.py" %*
set "EXIT_CODE=%ERRORLEVEL%"
popd >nul
exit /b %EXIT_CODE%

:missing_root
echo Could not find CTR-ModSDK root from "%START_DIR%".
echo Expected tools\psx\mod-builder\main.py somewhere above this build.bat.
exit /b 1
