#pragma once

#include "Emulator/CPU/defs/CPU.hpp"
#include "Emulator/Memory/defs/mem.hpp"

class Emulator
{

    
    public:
        void run(const std::string& filename);
        CPU cpu;
        Memory mem;
};