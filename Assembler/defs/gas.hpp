#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <sstream>
#include "Instructs/instructions.hpp"

struct func
{
    std::string name;
    uint16_t offset;
    uint16_t bsize;
};

class GAS
{
    private:
        func functionStartLocs[100];
        uint8_t functionCount = 0;

        uint16_t labelStartLocs[100];
        uint8_t labelCount = 0;

        uint16_t gradualOff = 0;

    public:

        std::string load(const std::string& filename); 
        void parse(const std::string& filename);
        void readInstructions(std::ifstream& file, std::ofstream& outFile);
};