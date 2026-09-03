#!/bin/bash


g++ -std=c++17 -Wall -Wextra \
    main.cpp \
    Emulator/emulator.cpp \
    Emulator/CPU/CPU.cpp \
    Emulator/Memory/mem.cpp \
    Debugger/Debugger.cpp \
    -I. \
    -o NekoVM


if [ $? -eq 0 ]; then
    echo "Build successful!"
fi
