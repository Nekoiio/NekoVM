#pragma once

#include "Emulator/defs/emulator.hpp"
#include "Instructs/instructions.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>


class Debugger
{
    public:
        Emulator& emu;

        Debugger(Emulator& e);

        Registers regState;

        void start(const std::string& filename);
        void printRegs();
        void interpret(const uint16_t address);
        void printInstruct(const std::string& op, const std::string& type, const uint16_t address);
        void dump_stack(const uint16_t entries);
        uint16_t instructionLength(uint16_t address);

    private:
        CPU& cpu;
        Memory& mem;
};