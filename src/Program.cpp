#include "Program.h"

Program::Program()
{
    //ctor
}

Program::~Program()
{
    //dtor
}

int Program::run(char* input_file_name_in, char* output_file_name_in)
{
    std::string input_file_name;
    std::string output_file_name;
    std::string input_file_string;
    std::string output_CPP;

    std::ifstream input_file;
    std::ofstream output_file;

    WTS_Parser parser;
    CCodeGenerator ccode_gen;
    GenInfoCodeGenerator info_gen;

    //Get the file!
    input_file_name = input_file_name_in;
    std::cout << "Hello! Welcome to the WTS (Way Too Simple) Language compiler!" << std::endl;
    std::cout << "You have chosen to compile the file: " << input_file_name << "\n";

    input_file.open(input_file_name.c_str());
    if (!input_file.is_open())
    {
        std::cout << "Something bad happened opening the input file, returning";
        return 1;
    }

    output_file_name = output_file_name_in;
    std::cout << "You have chosen to output the compiled code to: " << output_file_name << "\n";
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
    try
    {
        parser.parse();
    }
    catch (SyntaxErrorException* syntax_error)
    {
        std::cout << "!!!!!!|||||||||A syntax error has occured, quiting...!!!!!!|||||||||\n";
        std::cout << parser.getCPP();
        return -1;
    }

    output_CPP = "";
    /*
    output_CPP = parser.getCPP();

    output_CPP = output_CPP + "\n\n//INFO SECTION!!!!\n\n";
    info_gen.generate(parser.getTree());
    output_CPP = output_CPP + info_gen.getOutput();
*/
    output_CPP = output_CPP + "\n\n//C code!!!!!\n\n";
    ccode_gen.generate(parser.getTree());
    output_CPP = output_CPP + ccode_gen.getOutput();


    output_file << output_CPP;
    output_file.close();

    std::cout << "Program has been sucessfuly compiled into C/C++ and saved!\n";

    return 0;
}
