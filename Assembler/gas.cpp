#include "Assembler/defs/gas.hpp"


std::string GAS::load(const std::string& filename)
{
    std::ifstream filestream(filename);

    if (!filestream.is_open()) throw std::runtime_error("Error opening file");

    std::ostringstream stringBuffer;

    stringBuffer << filestream.rdbuf();

    return stringBuffer.str();
}



void GAS::parse(const std::string& filename, const std::string outfilename)
{
    std::ifstream file(filename);
    std::ofstream outFile(outfilename);

    //uint16_t count = 0; <- Replaced for functionCount to see if it works
    readInstructions(file, outFile);
}

void GAS::readInstructions(std::ifstream& file, std::ofstream& outFile)
{
    std::string line;
    while(std::getline(file, line))
    {
        if (line.empty()) continue;

        std::replace(line.begin(), line.end(), ',', ' ');

        std::string instruction;
        std::string op1;
        std::string op2;
        std::istringstream iss(line);

        iss >> instruction;
        iss >> op1;
        iss >> op2;

        //! DEBUG std::cout << instruction << std::endl << op1 << std::endl << op2 << std::endl;

        if (instruction.empty()) continue;
        // Exceptions
        if (instruction.front() == '@')
        {
            functionStartLocs[instruction.substr(1, line.size() - 2)] = gradualOff;
            continue;
        }
        if (instruction.front() == '-')
        {
            labelStartLocs[instruction.substr(1,line.size() - 2)] = gradualOff;
            continue;
        }
        if (instruction == "stp")
        {
            outFile.put(static_cast<char>(ISA::Instruction::stp));
            gradualOff += ISA::instructionLength(ISA::Instruction::stp);
            continue;
        }
        if (instruction == "ret")
        {
            outFile.put(static_cast<char>(ISA::Instruction::ret));
            gradualOff += ISA::instructionLength(ISA::Instruction::ret);
            continue;
        }
        if (instruction == "call")
        {
            outFile.put(static_cast<char>(ISA::Instruction::callA));
            outFile.put(0x00);
            outFile.put(0x00);
            gradualOff += ISA::instructionLength(ISA::Instruction::callA);
            continue;
        }
        if (instruction.front() == 'j')
        {
            auto it = ISA::instructionMap.find(instruction + 'a');

            if (it == ISA::instructionMap.end())
            {
                throw std::runtime_error("Unknown jump instruction");
            }


            outFile.put(static_cast<char>(it->second));
            outFile.put(0x00);
            outFile.put(0x00);
            gradualOff += ISA::instructionLength(it->second);
            continue;
        }




        b2w res = hVariants(instruction, op1, op2);

        if (res.active == -1)
        {
            throw std::runtime_error("Error, encountered an unknown instruction while reading");
        }


        for (int i = 0; i <= res.active-1; i++)
        {
            outFile.put(res.bytes[i]);
        }
    }
}

b2w GAS::hVariants(std::string& inst, std::string& op1, std::string& op2)
{
    b2w res ={
        {0, 0, 0, 0},
        1
    };
    std::string hashAccess = inst;
    //! DEBUG std::cout << hashAccess << std::endl;
    //! DEBUG std::cout << inst << std::endl << op1 << std::endl << op2 << std::endl;
    if (op1[0] == 'r')
    {
        hashAccess += 'r';
        res.bytes[1] = op1[1] - '0';
        res.active++;
    }

    if (op1[0] == 'x')
    {
        hashAccess += 'x';
        res.bytes[1] = op1[1] - '0';
        res.active++;
    }


    if (op2[0] == 'r')
    {
        hashAccess += 'r';
        res.bytes[2] = op2[1] - '0';
        res.active++;
    }

    if (op2[0] == 'x')
    {
        hashAccess += 'x';
        res.bytes[2] = op2[1] - '0';
        res.active++;
    }

    size_t numlen = 0;
    char valtype;
    //movR|X_V_A
    if (res.active == 2 && inst[0] != 'p') // Eliminate possible -> PushX|R  popX|R 
    {
        int num = numParse(op2, &numlen, valtype);

        res.bytes[2] = static_cast<char>(num); // putting it in little endian beacuse thats what memory funcs expect.
        res.bytes[3] = static_cast<char>(num >> 8);
        if (valtype == 'a')
        {
            res.active += 2;
        }
        else
        {
            res.active += 1;
        }
        hashAccess += valtype;
    }
    // pushAV  popAV
    if (res.active == 1) 
    {
        int num = numParse(op1, &numlen, valtype);
        

        res.bytes[1] = static_cast<char>(num); // putting it in little endian beacuse thats what memory funcs expect.
        res.bytes[2] = static_cast<char>(num >> 8);
        if (valtype == 'a')
        {
            res.active += 2;
        }
        else
        {
            res.active += 1;
        }
        hashAccess += valtype;
    }

    //!DEBUG std::cout << "hashAccess: " << hashAccess << std::endl;
    //!DEBUG std::cout << "type: " << valtype << std::endl;
    auto ins = ISA::instructionMap.find(hashAccess);
    if (ins == ISA::instructionMap.end())
    {
        res.active = -1;
        return res;
    }
    res.bytes[0] = ISA::encode(ins->second);
    gradualOff += ISA::instructionLength(ins->second);

    return res;
}
/*
add instruction to the string,
 then add op1[0] if r or x
 then add op2[0] if r or x
  if not neither
    read number and its length
        check for h hex interpretation
    if the number is > 254 determine mov_A


*/




int GAS::numParse(const std::string& op, size_t* numlen, char& valtype)
{
    if (op.empty())
        return -1;

    int num;

    if (op.back() == 'h')
    {
        std::string hexPart = op.substr(0, op.length() - 1);
        num = std::stoi(hexPart, numlen, 16);
    }
    else
    {
        num = std::stoi(op, numlen, 10);
    }

    if (num > 255)
        valtype = 'a';
    else
        valtype = 'v';

    return num;
}





char numvaltype(std::string& op2)
{
    if (op2.back() == 'h')
    {
        return 'a'; 
    }
    else
    {
        return 'v';
    }
}





void lstrip(std::string& line)
{
    size_t pos = line.find_first_not_of(" \t");

    if (pos == std::string::npos)
        line.clear();
    else
        line.erase(0, pos);
}

