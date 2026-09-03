#pragma once

#include <stdint.h>
#include "Emulator/Memory/defs/mem.hpp"
#include "Instructs/instructions.hpp"
#include <array>
#include <algorithm>

struct Registers
{
    std::array<uint8_t, 4> registers;
    std::array<uint16_t, 4> registersU16;
    uint16_t PC;
    bool running;
    uint16_t FLAGS;
    uint8_t instruction[3];
    uint16_t SP;
};

class CPU
{
    private:
        std::array<uint8_t, 4> registers = {0};
        std::array<uint16_t, 4> registersU16 = {0};

        uint16_t PC = MemoryMap::PROGRAM_START;
        uint16_t SP = MemoryMap::STACK_START;

        uint16_t FLAGS = 0x0000;
        
        void cmp(const int16_t res);
        
    public:
        bool running;

        void reset();

        void step(Memory& mem);

        void start(Memory& mem);

        Registers getRegisters();

        uint16_t getPC();

};

