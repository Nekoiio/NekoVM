#include "Debugger/defs/Debugger.hpp"

#define x) static_cast<uint8_t>(x)


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
std::string U16ToHex(uint16_t value)
{
    std::stringstream ss;

    ss << "0x"
       << std::uppercase
       << std::hex
       << std::setw(2)
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
    ISA::Instruction op = ISA::decode(mem.read(current));

    std::cout << "PC-> ";
    interpret(current);

    // Next instruction
    current += ISA::instructionLength(op);
    interpret(current);

    // Next instruction
    current += ISA::instructionLength(op);
    interpret(current);

    // Next instruction
    current += ISA::instructionLength(op);
    interpret(current);


    std::cout << "------------- REGISTERS ------------" << std::endl;
    for (int i = 0; i < regState.registers.size(); i++)
    {
        std::cout << "r" << i << " -> " << static_cast<int>(regState.registers[i]) << std::endl; 
    }
    for (int i = 0; i < regState.registersU16.size(); i++)
    {
        std::cout << "x" << i << " -> " << U16ToHex(static_cast<uint16_t>(regState.registersU16[i])) << std::endl; 
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
    switch (ISA::decode(mem.read(address)))
    {
        case ISA::Instruction::movRV: printInstruct("mov", "rv", address); break;
        case ISA::Instruction::movRR: printInstruct("mov", "rr", address); break;
        case ISA::Instruction::movXA: printInstruct("mov", "xa", address); break;
        case ISA::Instruction::movXR: printInstruct("mov", "xr", address); break;
        case ISA::Instruction::movXV: printInstruct("mov", "xv", address); break;
        case ISA::Instruction::movXX: printInstruct("mov", "xx", address); break;

        case ISA::Instruction::addRR: printInstruct("add", "rr", address); break;
        case ISA::Instruction::addRV: printInstruct("add", "rv", address); break;

        case ISA::Instruction::subRR: printInstruct("sub", "rr", address); break;
        case ISA::Instruction::subRV: printInstruct("sub", "rv", address); break;

        case ISA::Instruction::cmpRR: printInstruct("cmp", "rr", address); break;
        case ISA::Instruction::cmpRV: printInstruct("cmp", "rv", address); break; 

        case ISA::Instruction::jmpA:  printInstruct("jmp", "a", address); break;
        case ISA::Instruction::jzA:   printInstruct("jz", "a", address); break;

        case ISA::Instruction::stp:   printInstruct("stp", "s", address); break;

        case ISA::Instruction::pushR: printInstruct("push", "r", address); break;
        case ISA::Instruction::pushV: printInstruct("push", "v", address); break;
        case ISA::Instruction::pushA: printInstruct("push", "a", address); break;
        case ISA::Instruction::pushX: printInstruct("push", "x", address); break;

        case ISA::Instruction::callA: printInstruct("call", "a", address); break;
        case ISA::Instruction::ret:   printInstruct("ret", "", address); break;

        case ISA::Instruction::pop16: printInstruct("pop", "x", address); break;
        case ISA::Instruction::popR:  printInstruct("pop", "r", address); break;

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
    else if (type == "")
    {
        std::cout << op;
    }
    else if (type == "x")
    {
        std::cout << op << " x" << static_cast<int>(mem.read(address + 1));
    }
    else if (type == "xa")
    {
            std::cout 
            << op 
            <<  " x" 
            << static_cast<int>(mem.read(address + 1)) 
            << ", " 
            <<  static_cast<int>(mem.readU16(address + 2));   
    }
    else if (type == "xr")
    {
            std::cout 
            << op 
            <<  " x" 
            << static_cast<int>(mem.read(address + 1)) 
            << ", " 
            << "r"
            << static_cast<int>(mem.read(address + 2));
    }
    else if (type == "xx")
    {
            std::cout 
            << op 
            <<  " x" 
            << static_cast<int>(mem.read(address + 1)) 
            << ", " 
            << "x"
            << static_cast<int>(mem.read(address + 2));
    }
    else if(type == "xv")
    {
            std::cout 
            << op 
            <<  " x" 
            << static_cast<int>(mem.read(address + 1)) 
            << ", " 
            <<  static_cast<int>(mem.read(address + 2));
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
    if (MemoryMap::STACK_START == regState.SP)
    {
        std::cout << "NO VARIABLES ON THE STACK AT THE MOMENT" << std::endl;
        return;
    }

    uint16_t counter = regState.SP + entries - 1;

    for (;; counter--)
    {
        if (counter <= regState.SP)
            break;

        std::cout << toHex(counter)
                  << " | "
                  << U8ToHex(mem.read(counter))
                  << std::endl;

        if (counter == 0x0000)
            break;
    }

    std::cout << "SP-> "
              << toHex(regState.SP)
              << " | "
              << U8ToHex(mem.read(regState.SP))
              << std::endl;
}