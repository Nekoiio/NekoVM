g++ -std=c++17 -Wall -Wextra `
    main.cpp `
    Emulator/emulator.cpp `
    Emulator/CPU/CPU.cpp `
    Emulator/Memory/mem.cpp `
    Debugger/Debugger.cpp `
    -I. `
    -o NekoVM.exe


if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful!"
}