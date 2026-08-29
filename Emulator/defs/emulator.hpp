#pragma once

#include "Emulator/CPU/defs/CPU.hpp"
#include "Emulator/Memory/defs/mem.hpp"

class Emulator
{
    private:
        CPU cpu;
        Memory mem;
    
    public:
        void run(const std::string& filename);
};