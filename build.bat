@echo off
echo Compiling SFort95 Interpreter...
g++ -std=c++17 -Iinclude src\prog3.cpp src\parserInterp.cpp src\lex.cpp src\val.cpp -o sfort95.exe
if %errorlevel% neq 0 (
    echo Build failed.
    exit /b %errorlevel%
)
echo Build successful! Run sfort95.exe with an input file.
