#include "Debugger/defs/Debugger.hpp"

#define sci(x) static_cast<uint8_t>(x)


Debugger::Debugger(Emulator& e) : emu(e), cpu(e.cpu), mem(e.mem) {}

//*  ------------------------------ Helpers ----------------------------------
std::string toHex(uint16_t address)
{
    std::stringstream ss;

    ss  << "0x"
        << std::uppercase
        << std::hex
        << std::setw(4)
        << std::setfill('0')
        << address;

    std::string hex = ss.str();
    return hex;
}

std::string U8ToHex(uint16_t value)
{
    std::stringstream ss;

    ss << "0x"
       << std::uppercase
       << std::hex
       << std::setw(1)
       << std::setfill('0')
       << value;

    return ss.str();
}

uint16_t fromHex(std::string& hex)
{
    return static_cast<uint16_t>(std::stoul(hex, nullptr, 0));
} 

void helper()
{
    std::cout <<  "Registers = r" << std::endl;
    std::cout << "Continue   = c" << std::endl;
    std::cout << "Step       = s" << std::endl;
    std::cout << "Dumo Stack = d" << std::endl;
}
//* -------------------------------------------------------------------------

uint16_t Debugger::instructionLength(uint16_t address)
{
    switch (mem.read(address))
    {
        case sci(Instruction::stp):
            return 1;

        case sci(Instruction::pushR):
        case sci(Instruction::pushV):
        case sci(Instruction::popR):
            return 2;

        case sci(Instruction::movRV):
        case sci(Instruction::movRR):
        case sci(Instruction::addRR):
        case sci(Instruction::addRV):
        case sci(Instruction::subRR):
        case sci(Instruction::subRV):
        case sci(Instruction::cmpRR):
        case sci(Instruction::cmpRV):
        case sci(Instruction::jmpA):
        case sci(Instruction::jzA):
            return 3;

        default:
            return 1;
    }
}



void Debugger::start(const std::string& filename)
{
    regState = cpu.getRegisters();
    mem.load(filename);

    while (true)
    {
        char input;
        std::cout << "> ";
        std::cin >> input;

        switch (input)
        {
            case 'r':
            {

                printRegs();
                break;
            }
            case 's':
            {
                cpu.step(mem);
                regState = cpu.getRegisters();
                printRegs();
                break;
            }
            case 'c':
            {
                cpu.start(mem);
                break;
            }
            case 'd':
            {
                std::string leftover;
                std::cin >> leftover;

                std::cout << "------------- STACK ------------" << std::endl;
                dump_stack(static_cast<uint16_t>(std::stoi(leftover)));
                break;
            }
            case 'h':
            {
                helper();
            }
        }
    }
}



void Debugger::printRegs()
{
    std::cout << "------------- INSTRUCTIONS ------------" << std::endl;

    uint16_t current = regState.PC;

    std::cout << "PC-> ";
    interpret(current);

    // Next instruction
    current += instructionLength(current);
    interpret(current);

    // Next instruction
    current += instructionLength(current);
    interpret(current);

    // Next instruction
    current += instructionLength(current);
    interpret(current);


    std::cout << "------------- REGISTERS ------------" << std::endl;
    for (int i = 0; i < regState.registers.size(); i++)
    {
        std::cout << "R" << i << " -> " << static_cast<int>(regState.registers[i]) << std::endl; 
    }
    
    std::cout << std::endl << "PC -> " << toHex(regState.PC) << std::endl;
    std::cout << "SP -> " << toHex(regState.SP) << std::endl << std::endl;

    std::cout << "------------- FLAGS ------------"<< std::endl;
    
    std::cout << "ZF -> " << (regState.FLAGS & 0x01)        << std::endl;
    std::cout << "CF -> " << ((regState.FLAGS & 0x02) >> 1) << std::endl;
    std::cout << "NF -> " << ((regState.FLAGS & 0x04) >> 2) << std::endl;
    std::cout << "OF -> " << ((regState.FLAGS & 0x08) >> 3) << std::endl;
    
}

void Debugger::interpret(const uint16_t address)
{
    switch (mem.read(address))
    {
        case sci(Instruction::movRV): printInstruct("mov", "rv", address); break;
        case sci(Instruction::movRR): printInstruct("mov", "rr", address); break;
        case sci(Instruction::addRR): printInstruct("add", "rr", address); break;
        case sci(Instruction::addRV): printInstruct("add", "rv", address); break;
        case sci(Instruction::subRR): printInstruct("sub", "rr", address); break;
        case sci(Instruction::subRV): printInstruct("sub", "rv", address); break;
        case sci(Instruction::cmpRR): printInstruct("cmp", "rr", address); break;
        case sci(Instruction::cmpRV): printInstruct("cmp", "rv", address); break; 
        case sci(Instruction::jmpA):  printInstruct("jmp", "a", address); break;
        case sci(Instruction::jzA):   printInstruct("jz", "a", address); break;
        case sci(Instruction::stp):   printInstruct("stp", "s", address); break;
        case sci(Instruction::pushR): printInstruct("push", "r", address); break;
        case sci(Instruction::pushV): printInstruct("push", "v", address); break; 
        case sci(Instruction::popR):  printInstruct("pop", "r", address); break;

        default: break;
    }
}

void Debugger::printInstruct(const std::string& op, const std::string& type, const uint16_t address)
{
    std::cout << toHex(address) << " | ";
    if (type == "rr")
    {
        std::cout 
        << op 
        << " r" 
        << static_cast<int>(mem.read(address + 1)) 
        << ", " 
        << "r" 
        <<  static_cast<int>(mem.read(address + 2));
    } 
    else if (type == "rv")
    {
        std::cout 
            << op 
            <<  " r" 
            << static_cast<int>(mem.read(address + 1)) 
            << ", " 
            <<  static_cast<int>(mem.read(address + 2));
    }
    else if (type == "s")
    {
        std::cout << op;
    }
    else if (type == "a")
    {
        std::cout << op << " " << toHex(mem.readU16(address + 1));;
    }
    else if (type == "r")
    {
        std::cout << op << " r" << static_cast<int>(mem.read(address + 1));
    }
    else if (type == "v")
    {
        std::cout << op << " " << toHex(mem.read(address + 1));
    }
    else
    {
        std::cout << "UNKNOWN";
    }
    std::cout << std::endl;
}

/*
void Debugger::dump_stack(const uint16_t entries)
{
    for (uint16_t i = 0; i < entries; i++)
    {
        std::cout << toHex(regState.SP - i) << " | " << U8ToHex(mem.read(regState.SP - i)) << std::endl;
    }
}
*/

void Debugger::dump_stack(const uint16_t entries)
{
    uint16_t counter;
    if (MemoryMap::STACK_START - regState.SP == 0)
    {
        std::cout << "NO VARIABLES ON THE STACK AT THE MOMENT" << std::endl;
    } 
    else
    {
        counter = regState.SP + entries;
    }
    
    for (; counter > regState.SP; counter--)
    {
        std::cout << toHex(counter) << " | " << U8ToHex(mem.read(counter)) << std::endl;
    }
    std::cout << "SP-> " << toHex(regState.SP) << " | " << U8ToHex(mem.read(regState.SP)) << std::endl;

}