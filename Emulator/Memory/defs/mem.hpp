#pragma once

#include <stdint.h>
#include <fstream>
#include <string>
#include <stdexcept>

#define MAX_SIZE 65536

class Memory
{
    #define minm(a,b) (b >= a) : a ? b
    private:
        uint8_t data[MAX_SIZE] = {0};
    public:
        uint8_t  read(const uint16_t address);
        uint16_t readU16(const uint16_t address);

        void    load(const std::string& name);
        void    write(uint16_t address, const uint8_t* byte2write, const uint16_t byte_count);
        void    writeU8(uint16_t address, const uint8_t byte);
};

namespace MemoryMap
{
    constexpr uint16_t PROGRAM_START = 0x0000;
    constexpr uint16_t PROGRAM_END   = 0x7FFF;

    constexpr uint16_t STACK_START   = 0xFFFF;
    constexpr uint16_t STACK_END     = 0x8000;
}

namespace PermissionMap
{
    constexpr uint8_t CODE_REG_PERM   = 0b0000011; // X R 
    constexpr uint8_t STACK_REG_PERM  = 0b0000101; // W R
    //constexpr uint8_t GOT_REG_PERM    = 0b0000001; // R
}