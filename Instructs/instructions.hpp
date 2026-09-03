#pragma once

//* <- means currently working on
//! <- means not implemented
//? <- means not added to md file yet / dont know if it works
#include <stdint.h>
#include <unordered_map>
namespace ISA
{ //TODO: UPDATE THE NUMBERS ON THE TABLE all  +1
    enum class Instruction : uint8_t
    {
        //* Curr Num: 24
        movRV = 0x01,
        movRR = 0x10, 
        movXV = 0x20,//?
        movXR = 0x21,//?
        movXX = 0x22,//?
        movXA = 0x24,//?

        addRR = 0x02,
        addRV = 0x03,

        subRR = 0x04,
        subRV = 0x05,

        cmpRR = 0x06,
        cmpRV = 0x07,

        jmpA  = 0x08,
        jgA   = 0x18,//*?
        jlA   = 0x19,//*?
        jzA   = 0x09,
        

        pushR = 0x11,
        pushV = 0x12,
        pushA = 0x13,
        pushX = 0x23, //?
        popR  = 0x14,
        pop16  = 0x15,

        callA = 0x16,
        ret   = 0x17,

        stp   = 0xFF
    };

    std::unordered_map<std::string, ISA::Instruction> instructionMap =
    {
        {"movrv", ISA::Instruction::movRV},
        {"movrr", ISA::Instruction::movRR},

        {"movxv", ISA::Instruction::movXV},
        {"movxr", ISA::Instruction::movXR},
        {"movxx", ISA::Instruction::movXX},
        {"movxa", ISA::Instruction::movXA},

        {"addrr", ISA::Instruction::addRR},
        {"addrv", ISA::Instruction::addRV},

        {"subrr", ISA::Instruction::subRR},
        {"subrv", ISA::Instruction::subRV},

        {"cmprr", ISA::Instruction::cmpRR},
        {"cmprv", ISA::Instruction::cmpRV},

        {"jmpa", ISA::Instruction::jmpA},
        {"jga",  ISA::Instruction::jgA},
        {"jla",  ISA::Instruction::jlA},
        {"jza",  ISA::Instruction::jzA},

        {"pushr", ISA::Instruction::pushR},
        {"pushv", ISA::Instruction::pushV},
        {"pusha", ISA::Instruction::pushA},
        {"pushx", ISA::Instruction::pushX},

        {"popr",  ISA::Instruction::popR},
        {"pop16", ISA::Instruction::pop16},

        {"calla", ISA::Instruction::callA},
        {"ret",   ISA::Instruction::ret},
        {"stp",   ISA::Instruction::stp}
    };

    constexpr Instruction decode(uint8_t opcode)
    {
        return static_cast<Instruction>(opcode);
    }

    constexpr uint8_t encode(Instruction ins)
    {
        return static_cast<uint8_t>(ins);
    }
    
    constexpr Instruction strToInstruction(const std::string& ins)
    {
        
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
            case Instruction::pushX:
            case Instruction::popR:
            case Instruction::pop16:
                return 2;

            // 3 bytes
            case Instruction::movRV:
            case Instruction::movRR:
            case Instruction::movXR:
            case Instruction::movXV:
            case Instruction::movXX:
            case Instruction::addRR:
            case Instruction::addRV:
            case Instruction::subRR:
            case Instruction::subRV:
            case Instruction::cmpRR:
            case Instruction::cmpRV:
            case Instruction::jmpA:
            case Instruction::jzA:
            case Instruction::jlA:
            case Instruction::jgA:
            case Instruction::callA:
            case Instruction::pushA:
                return 3;
            
            
            case Instruction::movXA:
                return 4;
            // Unknown instruction
            default:
                return 1;
        }
    }
}