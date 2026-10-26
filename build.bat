@echo off
chcp 65001 > nul
title PBL2 - He Thong Quan Ly Cua Hang & Ban Hang (ImGui GUI)
color 0A

echo ============================================================
echo   [BUILD] DANG BIEN DICH DU AN PBL2 (C++17 + ImGui + GLFW)
echo ============================================================

:: 1. Kiem tra trinh bien dich g++
where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    color 0C
    echo [ERROR] Khong tim thay trinh bien dich 'g++' trong PATH!
    echo Vui long cai dat MinGW-w64 hoac them g++ vao bien moi truong.
    pause
    exit /b 1
)

:: 2. Tao thu muc bin va sao chep runtime dependencies
if not exist "bin" mkdir "bin"

if exist "Libraries\glfw3.dll" (
    copy /y "Libraries\glfw3.dll" "bin\" > nul
    copy /y "Libraries\glfw3.dll" ".\" > nul
)
if exist "Libraries\tahoma.ttf" (
    copy /y "Libraries\tahoma.ttf" "bin\" > nul
)

:: 3. Thuc hien bien dich toan bo ma nguon va thu vien do hoa
echo [INFO] Dang bien dich cac tep ma nguon...
g++ -std=c++17 -finput-charset=utf-8 -fexec-charset=utf-8 ^
    src/UI.cpp ^
    Libraries/Cart.cpp ^
    Libraries/DLinkedList.cpp ^
    Libraries/DataManager.cpp ^
    Libraries/HashTable.cpp ^
    Libraries/AccountStorage.cpp ^
    Libraries/AuthManager.cpp ^
    Libraries/ProductStorage.cpp ^
    Libraries/OrderManager.cpp ^
    Libraries/imgui-master/imgui.cpp ^
    Libraries/imgui-master/imgui_draw.cpp ^
    Libraries/imgui-master/imgui_tables.cpp ^
    Libraries/imgui-master/imgui_widgets.cpp ^
    Libraries/imgui-master/backends/imgui_impl_glfw.cpp ^
    Libraries/imgui-master/backends/imgui_impl_opengl3.cpp ^
    -I Libraries ^
    -I Libraries/imgui-master ^
    -I Libraries/imgui-master/backends ^
    -I Libraries/glfw-3.4.bin.WIN64/include ^
    -L Libraries/glfw-3.4.bin.WIN64/lib-mingw-w64 ^
    -lglfw3 -lopengl32 -lgdi32 ^
    -o bin/app.exe

:: 4. Kiem tra ket qua bien dich
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
    echo [ERROR] Bien dich that bai! Vui long kiem tra lai thong bao loi o tren.
)

echo.
echo ============================================================
echo Nhan phim bat ky de thoat...
pause > nul
