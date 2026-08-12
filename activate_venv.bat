@echo off

:: 1. Set REPO_ROOT (if not already defined externally)
if not defined REPO_ROOT (
    set "REPO_ROOT=%~dp0"
    if "%REPO_ROOT:~-1%"=="\" set "REPO_ROOT=%REPO_ROOT:~0,-1%"
)

echo %REPO_ROOT%\tools\build_tools\%

:: 2. Prepend local build tools (MinGW, CMake, Ninja, etc.) to PATH
set "PATH=%REPO_ROOT%\tools\build_tools\MinGW\bin;%REPO_ROOT%\tools\build_tools\CMake\bin;%REPO_ROOT%\tools\build_tools\ninja-win;%PATH%"

:: Activate the virtual environment
call %REPO_ROOT%\tools\build_tools\Python312\.venv\Scripts\activate.bat

:: Run your work or keep the prompt open
cmd /k

