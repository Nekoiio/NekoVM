#include "Emulator/Memory/defs/mem.hpp"

#define minm(a,b) ((a <= b) ? a : b)

uint8_t Memory::read(const uint16_t address)
{
    if (address >= MAX_SIZE)
    {
        throw std::runtime_error("Out of bounds read in memory");
    }
    return data[address];
}

uint16_t Memory::readU16(const uint16_t address)
{
    if (address >= MAX_SIZE)
    {
        throw std::runtime_error("Out of bounds read in memory");
    }

    uint16_t res = static_cast<uint16_t>(data[address]) | (static_cast<uint16_t>(data[address + 1]) << 8);
    return res;
}


void Memory::load(const std::string& name)
{
    std::ifstream file(name, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        throw std::runtime_error("error opening binary file");
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    size_t bytesToRead = minm(static_cast<size_t>(fileSize), MAX_SIZE);

    // 3. Read the block directly into your pre-existing array
    if (!file.read(reinterpret_cast<char*>(data), bytesToRead)) {
        throw std::runtime_error("Error reading binary file");
    }

    file.close();
} 
