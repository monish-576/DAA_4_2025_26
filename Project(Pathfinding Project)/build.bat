@echo off
echo Building Campus Route ^& Navigation System...

if not exist bin (
    mkdir bin
)

g++ -std=c++11 src\*.cpp -o bin\navigation_system.exe
if %ERRORLEVEL% equ 0 (
    echo Build successful. Executable created at bin\navigation_system.exe
) else (
    echo Build failed!
)
pause
