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

void Memory::write(uint16_t address, const uint8_t* bytes2write, const uint16_t byte_count)
{
    if (address + byte_count >= MAX_SIZE)
    {
        throw std::runtime_error("Out of bounds write in memory");
    }

    for (int i = 0; i < byte_count; i++)
    {
        data[address + i] = bytes2write[i];
    }
}

void Memory::writeU8(uint16_t address, const uint8_t byte)
{
    if (address >= MAX_SIZE)
    {
        throw std::runtime_error("Out of bounds write in memory");
    }

    data[address] = byte;
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

    size_t bytesToRead = minm(static_cast<size_t>(fileSize), MemoryMap::PROGRAM_END);

    // 3. Read the block directly into your pre-existing array
    if (!file.read(reinterpret_cast<char*>(data), bytesToRead)) {
        throw std::runtime_error("Error reading binary file");
    }

    file.close();
}


