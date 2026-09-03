#include "Assembler/defs/gas.hpp"


std::string GAS::load(const std::string& filename)
{
    std::ifstream filestream(filename);

    if (!filestream.is_open()) throw std::runtime_error("Error opening file");

    std::ostringstream stringBuffer;

    stringBuffer << filestream.rdbuf();

    return stringBuffer.str();
}



void GAS::parse(const std::string& filename)
{
    std::ifstream file(filename);
    std::ofstream outFile("TEST.nvm");

    std::string line;
    uint16_t count = 0;
    while(std::getline(file, line))
    {
        if (line.empty()) continue;


        switch (line[0])
        {
            case '@':
                functionCount++;
                functionStartLocs[count].offset = gradualOff;
                functionStartLocs[count].name   = line.substr(1, line.find(':')); // Strips away (@)func(:) leaving func
                readInstructions(file, outFile);
                break;
        }
        count++;
    }
}

void GAS::readInstructions(std::ifstream& file, std::ofstream& outFile)
{
    std::string line;
    while(std::getline(file, line))
    {
        if (line.empty()) continue;

        lstrip(line);

        if (line.find("ret") != std::string::npos) 
        {
            outFile.put(ISA::encode(ISA::Instruction::ret));
            gradualOff += ISA::instructionLength(ISA::Instruction::ret);
            break;
        }
        



    }
}


std::string hMov(std::string& line);
std::string hAdd(std::string& line);
std::string hCmp(std::string& line);
std::string hPush(std::string& line);
std::string hPop(std::string& line);




void lstrip(std::string& line)
{
    size_t pos = line.find_first_not_of(" \t");

    if (pos == std::string::npos)
        line.clear();
    else
        line.erase(0, pos);
}

