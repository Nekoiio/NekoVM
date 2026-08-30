#pragma once

#include <stdint.h>
#include "Emulator/Memory/defs/mem.hpp"
#include "Emulator/CPU/defs/instructions.hpp"
#include <array>
#include <algorithm>

struct Registers
{
    std::array<uint8_t, 4> registers;
    uint16_t PC;
    bool running;
    uint16_t FLAGS;
    uint8_t instruction[3];
    uint16_t SP;
};

class CPU
{
    private:
        uint8_t registers[4];

        uint16_t PC;
        uint16_t SP;

        uint16_t FLAGS;
        
        void cmp(const int16_t res);

    public:
        bool running;

        void reset();

        void step(Memory& mem);

        void start(Memory& mem);

        Registers getRegisters();

};

