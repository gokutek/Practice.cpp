@echo off

cmake -S . -B build -A Win32
cmake --build build --config Debug

pause