#include "Program.h"

Program::Program()
{
    //ctor
}

Program::~Program()
{
    //dtor
}

int Program::run()
{
    std::string input_file_name;
    std::string output_file_name;
    std::string input_file_string;
    std::string output_CPP;

    std::ifstream input_file;
    std::ofstream output_file;

    WTS_Parser parser;

    //Get the file!

    std::cout << "Hello! Welcome to the WTS (Way Too Simple) Language compiler!" << std::endl;
    std::cout << "Please input the name of the file you wish to compile:";
    std::cin >> input_file_name;

    input_file.open(input_file_name.c_str());
    if (!input_file.is_open())
    {
        std::cout << "Something bad happened opening the input file, returning";
        return 1;
    }

    std::cout << "Please input the name of the file you wish to which you want to save the compiled code:";
    std::cin >> output_file_name;

    output_file.open(output_file_name.c_str());
    if (!output_file.is_open())
    {
        std::cout << "Something bad happened with opening the output file, returning";
        return 1;
    }

    //Read the file
    std::string line;
    while(input_file.good())
    {
        getline(input_file, line);
        input_file_string.append(line + "\n");
    }

    input_file.close();

    //Parse this thing!
    parser.set_string(input_file_string);
    parser.parse();
    output_CPP = parser.getCPP();

    //Output created C++ to file

    output_file << output_CPP;
    output_file.close();

    return 0;
}
