@echo off
setlocal

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%

cl /nologo /W4 /WX /std:c11 /TC lubtype_regression_tests.c /Fe:lubtype_regression_tests.exe
if errorlevel 1 exit /b %errorlevel%

lubtype_regression_tests.exe
exit /b %errorlevel%
