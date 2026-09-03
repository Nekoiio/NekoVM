#pragma once

//* <- means currently working on
//! <- means not implemented

#include <stdint.h>

namespace ISA
{
    enum class Instruction : uint8_t
    {
        movRV = 0x00,
        movRR = 0x09, 
        movXV = 0x19,//*
        movXR = 0x20,//*
        movXX = 0x21,

        addRR = 0x01,
        addRV = 0x02,

        subRR = 0x03,
        subRV = 0x04,

        cmpRR = 0x05,
        cmpRV = 0x06,

        jmpA  = 0x07,
        jg    = 0x17,//!
        jl    = 0x18,//!
        jzA   = 0x08,
        

        pushR = 0x10,
        pushV = 0x11,
        pushA = 0x12,
        popR  = 0x13,
        pop16  = 0x14,

        callA = 0x15,
        ret   = 0x16,

        stp   = 0xFF
    };

    constexpr Instruction decode(uint8_t opcode)
    {
        return static_cast<Instruction>(opcode);
    }
    

    constexpr uint16_t instructionLength(Instruction instruction)
    {
        switch (instruction)
        {
            // 1 byte
            case Instruction::stp:
            case Instruction::ret:
                return 1;

            // 2 bytes
            case Instruction::pushR:
            case Instruction::pushV:
            case Instruction::popR:
            case Instruction::pop16:
                return 2;

            // 3 bytes
            case Instruction::movRV:
            case Instruction::movRR:
            case Instruction::movXR:
            case Instruction::addRR:
            case Instruction::addRV:
            case Instruction::subRR:
            case Instruction::subRV:
            case Instruction::cmpRR:
            case Instruction::cmpRV:
            case Instruction::jmpA:
            case Instruction::jzA:
            case Instruction::callA:
            case Instruction::pushA:
                return 3;
            
            
            case Instruction::movXV:
                return 4;
            // Unknown instruction
            default:
                return 1;
        }
    }
}