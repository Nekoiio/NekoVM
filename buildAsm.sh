#!/bin/bash

g++ -std=c++17 -Wall -Wextra \
    asmMain.cpp \
    Assembler/gas.cpp \
    -I. \
    -o asmNekoVm

if [ $? -eq 0 ]; then
    echo "Build successful!"
fi
