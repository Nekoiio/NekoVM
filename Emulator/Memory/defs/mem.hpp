#pragma once

#include <stdint.h>
#include <fstream>
#include <string>
#include <stdexcept>

#define MAX_SIZE 65536

class Memory
{

    private:
        uint8_t data[MAX_SIZE] = {0};
    public:
        uint8_t  read(const uint16_t address);
        uint16_t readU16(const uint16_t address);

        void    load(const std::string& name);
        void    write(const uint16_t address);
};