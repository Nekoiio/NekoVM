#pragma once

enum class Instruction : uint8_t
{
    movRV = 0x00,
    movRR = 0x09,

    addRR = 0x01,
    addRV = 0x02,

    subRR = 0x03,
    subRV = 0x04,

    cmpRR = 0x05,
    cmpRV = 0x06,

    jmpA  = 0x07,
    jzA   = 0x08,

    pushR = 0x10,
    pushV = 0x11,
    popR  = 0x12,
    
    stp   = 0xFF
};