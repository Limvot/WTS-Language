#include <iostream>

#include <string>
#include "include/Node.h"
#include "include/Program.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Need both an input and an output file!\n";
        return 0;
    }
    Program program;
    program.run(argv[1],argv[2]);
    return 0;
}
