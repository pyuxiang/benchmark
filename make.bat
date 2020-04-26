@echo off
rem CS2040C AY19/20 Sem 2 Lab Group B08
rem Peh Yu Xiang, National University of Singapore
rem 2020-04-25
rem
rem Batch file for automated compilation and testing
rem
rem To sniff actual process resources: https://stackoverflow.com/q/63166
rem To limit stack size: https://stackoverflow.com/q/2275550
rem - Windows: -Wl,--stack=<size_in_bytes> as g++ flags

set SRC_DIR=implementations
set TEST_DIR=tester
set TARGET=main

set CATCH_FLAGS=-x 5
set CCFLAGS=-std=c++11 -Wall -I$(CATCH_SINGLE_INCLUDE)
set CLASSES=
set TESTS=
for %%f in ( %SRC_DIR%\*.cpp ) do (
    call :concat %%~nf %TEST_DIR%\test-%%~nf.cpp
)

if [%1]==[clean] (
    if exist %TARGET%.exe del %TARGET%.exe
    goto :eof
)
if [%1]==[release] goto release
if [%1]==[run] goto run
if not [%1]==[] (
    if not [%1]==[all] (
        if not [%1]==[build] (
            goto invalid
        )
    )
)

if not exist %TEST_DIR%\catch-windows.o (
    echo [make] Compiling test framework...
    g++ -c %TEST_DIR%\catch.cpp -o %TEST_DIR%\catch-windows.o
)
echo [make] Compiling tests for:%CLASSES%
g++ %CCFLAGS% -o %TARGET% %TESTS% %TEST_DIR%\catch-windows.o
echo [make] Success, compiled to %TARGET%.exe
if [%1]==[build] goto :eof

:run
if exist %TARGET%.exe (
    echo [make] Running: ".\%TARGET%.exe %CATCH_FLAGS%"
    .\%TARGET%.exe %CATCH_FLAGS%
) else (
    echo [make] Error, %TARGET%.exe does not exist
)
goto :eof

:invalid
echo make: *** No rule to make target '%1'.  Stop.
goto :eof

:concat
set CLASSES=%CLASSES% %1
set TESTS=%TESTS% %2
goto :eof

:release
rem System-specific date
set NOW=%date:~10%%date:~7,2%%date:~4,2%
if exist %TEST_DIR%\catch-windows.o del %TEST_DIR%\catch-windows.o
if exist %TEST_DIR%\catch-unix.o del %TEST_DIR%\catch-unix.o
set FILES=
for %%a in (*) do call :concatfiles "%%a"
tar -cvf release/tester-%NOW%.tar %SRC_DIR% %TEST_DIR% %FILES%
goto :eof

:concatfiles
set FILES=%FILES% %1
goto :eof
