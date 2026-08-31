#include "Assembler/defs/gas.hpp"


std::string GAS::load(const std::string& filename)
{
    std::ifstream filestream(filename);

    if (!filestream.is_open()) throw std::runtime_error("Error opening file");

    std::ostringstream stringBuffer;

    stringBuffer << filestream.rdbuf();

    return stringBuffer.str();
}
