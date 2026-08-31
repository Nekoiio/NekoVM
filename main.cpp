#include "Emulator/defs/emulator.hpp"
#include "Debugger/defs/Debugger.hpp"

int main()
{

    Emulator test;
    Debugger debg = Debugger(test);

    std::string inp;
    std::cout << "Mode> ";
    std::cin >> inp;


    std::string filename;
    std::cout << "Filename> ";
    std::cin >> filename;

    if (inp == "debug")
    {
        debg.start(filename);
    } 
    else if (inp == "normal")
    {
        test.run(filename);
    }

    return 0;
}

