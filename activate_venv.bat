@echo off
setlocal EnableExtensions

:: 1. Set REPO_ROOT to the directory containing this script (without trailing backslash)
if not defined REPO_ROOT (
    for %%I in ("%~dp0.") do set "REPO_ROOT=%%~fI"
)

echo ===================================================
echo Repo Root   : %REPO_ROOT%
echo Build Tools : %REPO_ROOT%\tools\build_tools
echo ===================================================

:: 2. Prepend local build tools (MinGW, CMake, Ninja, etc.) to PATH
set "PATH=%REPO_ROOT%\tools\build_tools\MinGW\bin;%REPO_ROOT%\tools\build_tools\CMake\bin;%REPO_ROOT%\tools\build_tools\ninja-win;%PATH%"

:: 3. Activate the virtual environment
set "VENV_ACTIVATE=%REPO_ROOT%\tools\build_tools\Python312\.venv\Scripts\activate.bat"

if exist "%VENV_ACTIVATE%" (
    call "%VENV_ACTIVATE%"
) else (
    echo [ERROR] Virtual environment not found at:
    echo "%VENV_ACTIVATE%"
)

:: 4. Keep the command prompt open
cmd /k
