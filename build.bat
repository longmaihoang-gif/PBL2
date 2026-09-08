@echo off
chcp 65001 > nul
title PBL2 - He Thong Quan Ly Cua Hang & Ban Hang
color 0A

echo ============================================================
echo   [BUILD] DANG BIEN DICH DU AN PBL2 (C++17)...
echo ============================================================

:: Tao thu muc bin neu chua co
if not exist "bin" mkdir "bin"

:: Lenh bien dich gop toan bo src/*.cpp va Libraries/*.cpp
g++ -std=c++17 src/*.cpp Libraries/*.cpp -I Libraries -o bin/app.exe

:: Kiem tra trang thai bien dich
if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Bien dich thanh cong ruc ro!
    echo ============================================================
    echo   [RUNNING] KHOI CHAY CHUONG TRINH...
    echo ============================================================
    echo.
    bin\app.exe
) else (
    echo.
    color 0C
    echo [ERROR] Bien dich that bai! Vui long kiem tra lai loi o tren.
)

echo.
echo ============================================================
echo Nhan phim bat ky de thoat...
pause > nul
