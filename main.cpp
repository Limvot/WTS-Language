#include <iostream>

#include <string>
#include "include/ASTNode.h"
#include "include/Program.h"

using namespace std;

int main(int argc, char* argv[])
{
    ASTNode* root = new ASTNode;
    std::cout << "Type is " << root->type << "!\n";
    if (argc < 3)
    {
        std::cout << "Need both an input and an output file!\n";
        return 0;
    }
    Program program;
    program.run(argv[1],argv[2]);
    return 0;
}
