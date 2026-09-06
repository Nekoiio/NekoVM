#include "Assembler/defs/gas.hpp"


int main(int argc, char* argv[])
{
    GAS g;
    g.parse(argv[1], argv[2]);
    return 0;
}