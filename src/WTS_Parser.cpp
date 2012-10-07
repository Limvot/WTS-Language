#include "WTS_Parser.h"

WTS_Parser::WTS_Parser()
{
    reached_end = false;
}

WTS_Parser::~WTS_Parser()
{
    //dtor
}

int WTS_Parser::set_string(std::string input_string)
{
    reached_end = false;
    reader.set_string(input_string);
    return 0;
}

int WTS_Parser::parse()
{
    //Do the parse work!
    //The standard opening
    cpp_output = "#include <iostream>\nusing namespace std;\n";

    std::string current_line;
    while (!reached_end)
    {
        current_line = reader.line();
        if (current_line == "")                                     //Reached end-of-file
        {
            reached_end = true;
        } else {
            cpp_output.append(current_line);
            std::cout << cpp_output << std::endl;
        }
    }

    return 0;
}

std::string WTS_Parser::getCPP()
{
    return cpp_output;
}

