# PBL2 - Auto setup GLFW 64-bit library
if (-not (Test-Path "Libraries\glfw-3.4.bin.WIN64\lib-mingw-w64\libglfw3.a")) {
    Write-Host "[INFO] Dang tu dong tai thu vien GLFW 64-bit (vui long doi vai giay)..." -ForegroundColor Yellow
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
    Invoke-WebRequest -Uri "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip" -OutFile "glfw.zip"
    Expand-Archive -Path "glfw.zip" -DestinationPath "temp_glfw" -Force
    if (Test-Path "Libraries\glfw-3.4.bin.WIN64\lib-mingw-w64") {
        Remove-Item -Recurse -Force "Libraries\glfw-3.4.bin.WIN64\lib-mingw-w64"
    }
    Move-Item -Path "temp_glfw\glfw-3.4.bin.WIN64\lib-mingw-w64" -Destination "Libraries\glfw-3.4.bin.WIN64\" -Force
    if (-not (Test-Path "bin")) {
        New-Item -ItemType Directory -Path "bin" -Force | Out-Null
    }
    Copy-Item "Libraries\glfw-3.4.bin.WIN64\lib-mingw-w64\glfw3.dll" -Destination "Libraries\" -Force
    Copy-Item "Libraries\glfw-3.4.bin.WIN64\lib-mingw-w64\glfw3.dll" -Destination "bin\" -Force
    Remove-Item -Recurse -Force "temp_glfw", "glfw.zip"
    Write-Host "[SUCCESS] Da cai dat xong thu vien GLFW thanh cong!" -ForegroundColor Green
}

