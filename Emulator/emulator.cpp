#include "Emulator/defs/emulator.hpp"

void Emulator::run(const std::string& filename)
{
    mem.load(filename);
    cpu.start(mem);
}