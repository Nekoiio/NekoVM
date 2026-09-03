#include "Emulator/CPU/defs/CPU.hpp"


constexpr uint8_t FLAG_ZERO    =  0b00000001;
constexpr uint8_t FLAG_CARRY   =  0b00000010;
constexpr uint8_t FLAG_NEGATIVE = 0b00000100;
constexpr uint8_t FLAG_OVERFLOW = 0b00001000;

void CPU::cmp(const int16_t res)
{
    if (res == 0)
    {
        FLAGS |= FLAG_ZERO;
        FLAGS &= ~FLAG_NEGATIVE;
    } 
    else if (res < 0)
    {
        FLAGS &= ~FLAG_ZERO;
        FLAGS |= FLAG_NEGATIVE;
    }
    else if (res > 0)
    {
        FLAGS &= ~FLAG_ZERO;
        FLAGS &= ~FLAG_NEGATIVE;
    }
}


void CPU::step(Memory& mem)
{
    ISA::Instruction opcode = ISA::decode(mem.read(PC));
    uint8_t ilen = ISA::instructionLength(opcode);
    
    switch (opcode)
    {
        case ISA::Instruction::movRV:
        {    
            uint8_t val = mem.read(PC + 2);
            registers[mem.read(PC + 1)] = val;

            PC += ilen;
            break;
        }

        case ISA::Instruction::movRR:
        {   
            uint8_t val = registers[mem.read(PC + 2)];
            registers[mem.read(PC + 1)] = val;

            PC += ilen;
            break;
        }

        case ISA::Instruction::movXR:
        {
            
        }
        case ISA::Instruction::movXV:
        {
            registersU16[mem.read(PC + 1)] = mem.readU16(PC + 2);
        }

        
        case ISA::Instruction::addRR:
        {    
            registers[mem.read(PC+1)] += registers[mem.read(PC+2)];

            PC += ilen;
            break;
        }
        case ISA::Instruction::addRV:
        {    
            registers[mem.read(PC+1)] += mem.read(PC+2);

            PC += ilen;
            break;
        }

        case ISA::Instruction::subRR:
        {
            registers[mem.read(PC+1)] -= registers[mem.read(PC+2)];

            PC += ilen;
            break;
        }

        case ISA::Instruction::subRV:
        {    
            registers[mem.read(PC+1)] -= mem.read(PC+2);

            PC += ilen;
            break;
        }
        case ISA::Instruction::jmpA:
        {    
            PC = mem.readU16(PC + 1);
            break;
        }
        case ISA::Instruction::jzA:
        {    
            if (FLAGS & FLAG_ZERO)
            {
                PC = mem.readU16(PC + 1);
            } else {
                PC += ilen;
            }
            break;
        }
        

        case ISA::Instruction::cmpRR:
        {    
            int16_t res = registers[mem.read(PC + 1)] - registers[mem.read(PC + 2)];
            cmp(res);
            PC += ilen;
            break;
        }
        
        case ISA::Instruction::cmpRV:
        {    
            int16_t res = registers[mem.read(PC + 1)] - mem.read(PC + 2);
            cmp(res);
            PC += ilen;
            break;
        }



        case ISA::Instruction::pushR:
        {
            if (SP <= MemoryMap::STACK_END) throw std::runtime_error("Stack overflow");

            SP -= 1;
            mem.writeU8(SP, registers[mem.read(PC + 1)]);
            PC += ilen;
            break;
        }

        case ISA::Instruction::pushV:
        {
            if (SP <= MemoryMap::STACK_END) throw std::runtime_error("Stack overflow");

            SP -= 1;
            mem.writeU8(SP, mem.read(PC + 1));
            PC += ilen;
            break;
        }

        case ISA::Instruction::pushA:
        {
            if (SP < MemoryMap::STACK_END + 2) throw std::runtime_error("Stack overflow");
            
            SP -= 2;

            uint16_t bytes = mem.readU16(PC+1);
            printf("PUSHED VALUE: %x", bytes);
            mem.writeU16(SP, bytes);

            PC += ilen;
            break;
        }


        case ISA::Instruction::pop16:
        {
            if (SP + 2 == 0x0000) throw std::runtime_error("Stack underflow");

            registersU16[mem.read(PC + 1)] = mem.readU16(SP);
            SP += 2;

            PC += ilen;
            break;
        }

        case ISA::Instruction::popR:
        {
            if (SP + 1 == 0x0000) throw std::runtime_error("Stack underflow");

            registers[mem.read(PC + 1)] = mem.read(SP);
            SP += 1;
            PC += ilen;
            break;
        }

        case ISA::Instruction::callA:
        {
            uint16_t returnAddress = PC + ilen;

            SP -= 2;
            mem.writeU16(SP, returnAddress);

            PC = mem.readU16(PC + 1);

            break;
        }

        case ISA::Instruction::ret:
        {
            if (SP == MemoryMap::STACK_START) throw std::runtime_error("Stack underflow");

            PC = mem.readU16(SP);
            SP += 2;

            break;
        }

        case ISA::Instruction::stp:
        {    
            running = false;
            break;
        }
        default:
        {
            running = false;
            break;
        }
    }
}



void CPU::reset()
{
    PC = MemoryMap::PROGRAM_START;
    SP = MemoryMap::STACK_START;
    registers = {0, 0, 0, 0};
    registersU16 = {0, 0, 0, 0};
    running = false;
    FLAGS &= 0;
    return;
}

void CPU::start(Memory& mem)
{
    running = true;

    while (running)
    {
        step(mem);
        //printf("%d\n", registers[0]);
    }
    reset();
}


Registers CPU::getRegisters()
{
    Registers current_state;

    std::copy(
        registers.begin(),
        registers.end(),
        current_state.registers.begin()
    );

    std::copy(
        registersU16.begin(),
        registersU16.end(),
        current_state.registersU16.begin()
    );

    current_state.PC = PC;
    current_state.FLAGS = FLAGS;
    current_state.running = running;
    current_state.SP    = SP;


    return current_state;
}

uint16_t CPU::getPC()
{
    return PC;
}