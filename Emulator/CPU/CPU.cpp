#include "Emulator/CPU/defs/CPU.hpp"


#define sci(x) static_cast<uint8_t>(x)

constexpr uint8_t FLAG_ZERO    = 0x01;
constexpr uint8_t FLAG_CARRY   = 0x02;
constexpr uint8_t FLAG_NEGATIVE = 0x04;
constexpr uint8_t FLAG_OVERFLOW = 0x08;

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
    uint8_t opcode = mem.read(PC);

    switch (opcode)
    {
        case sci(Instruction::movRV):
        {    
            uint8_t val = mem.read(PC + 2);
            registers[mem.read(PC + 1)] = val;

            PC += 3;
            break;
        }

        case sci(Instruction::movRR):
        {   
            uint8_t val = registers[mem.read(PC + 2)];
            registers[mem.read(PC + 1)] = val;

            PC += 3;
            break;
        }

        case sci(Instruction::addRR):
        {    
            registers[mem.read(PC+1)] += registers[mem.read(PC+2)];

            PC += 3;
            break;
        }
        case sci(Instruction::addRV):
        {    
            registers[mem.read(PC+1)] += mem.read(PC+2);

            PC += 3;
            break;
        }

        case sci(Instruction::subRR):
        {
            registers[mem.read(PC+1)] -= registers[mem.read(PC+2)];

            PC += 3;
            break;
        }

        case sci(Instruction::subRV):
        {    
            registers[mem.read(PC+1)] -= mem.read(PC+2);

            PC += 3;
            break;
        }
        case sci(Instruction::jmpA):
        {    
            PC = mem.readU16(PC + 1);
            break;
        }
        case sci(Instruction::jzA):
        {    
            if (FLAGS & FLAG_ZERO)
            {
                PC = mem.readU16(PC + 1);
            } else {
                PC += 3;
            }
            break;
        }
        

        case sci(Instruction::cmpRR):
        {    
            int16_t res = registers[mem.read(PC + 1)] - registers[mem.read(PC + 2)];
            cmp(res);
            PC += 3;
            break;
        }
        
        case sci(Instruction::cmpRV):
        {    
            int16_t res = registers[mem.read(PC + 1)] - mem.read(PC + 2);
            cmp(res);
            PC += 3;
            break;
        }


        case sci(Instruction::pushR):
        {
            if (SP <= MemoryMap::STACK_END) throw std::runtime_error("Stack overflow");

            SP -= 1;
            mem.writeU8(SP, registers[mem.read(PC + 1)]);
            PC += 2;
            break;
        }

        case sci(Instruction::pushV):
        {
            if (SP <= MemoryMap::STACK_END) throw std::runtime_error("Stack overflow");

            SP -= 1;
            mem.writeU8(SP, mem.read(PC + 1));
            PC += 2;
            break;
        }

        case sci(Instruction::popR):
        {
            if (SP + 1 == 0x0000) throw std::runtime_error("Stack underflow");
            registers[mem.read(PC + 1)] = mem.read(SP);
            SP += 1;
            PC += 2;
            break;
        }

        case sci(Instruction::stp):
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
        std::begin(registers),
        std::end(registers),
        current_state.registers.begin()
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