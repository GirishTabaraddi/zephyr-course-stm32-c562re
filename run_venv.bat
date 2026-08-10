@echo off
:: Activate the virtual environment

REM echo %REPO_ROOT%\tools\build_tools\%

call %REPO_ROOT%\tools\build_tools\Python312\.env\Scripts\activate.bat

:: Run your work or keep the prompt open
cmd /k

