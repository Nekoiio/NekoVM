#pragma once

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <sstream>
#include <algorithm>
#include "Instructs/instructions.hpp"

struct func
{
    std::string name;
    uint16_t offset;
    uint16_t bsize;
};

struct b2w
{
    char bytes[4];
    int active;
};

class GAS
{
    private:
        std::unordered_map<std::string, uint16_t> functionStartLocs;
        uint8_t functionCount = 0;

        std::unordered_map<std::string, uint16_t> labelStartLocs;
        uint8_t labelCount = 0;

        uint16_t gradualOff = 0;

        b2w hVariants(
            std::string& instruction,
            std::string& operand1,
            std::string& operand2
        );
        void hJmps(std::string& HOI); // Hex Or Int
        void hCalls(std::string& HOI);
        int numParse(const std::string& op2, size_t* numlen, char& type);
        char numType(std::string& op2);
    public:

        std::string load(const std::string& filename); 
        void parse(const std::string& filename, const std::string outfilename);
        void readInstructions(std::ifstream& file, std::ofstream& outFile);
};
