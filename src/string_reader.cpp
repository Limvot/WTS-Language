#include "string_reader.h"

string_reader::string_reader()
{
    str_pos = 0;
}

string_reader::string_reader(std::string input_string)
{
    str_pos = 0;
    end_reached = false;
    set_string(input_string);
}

string_reader::~string_reader()
{
    //dtor
}

void string_reader::set_string(std::string input_string)
{
    rd_string = input_string;
    end_reached = false;
}

std::string string_reader::word()
{
    return "";
}

std::string string_reader::line()
{
    int found_pos = 0;
    found_pos = rd_string.find("\n", str_pos);
    std::cout << "Position is " << found_pos << std::endl;

    if (found_pos == str_pos)                                   //We are at the endline
    {
        str_pos++;
        return "\n";
    }

    if (found_pos == std::string::npos)                         //We are at the end of the file
    {
        //End of String
        end_reached = true;
        std::cout << "Reached end of file!\n";
        return "";
    } else {
        std::string string_section;
        for (; str_pos < found_pos; str_pos++)
        {
            string_section = string_section + rd_string[str_pos];
        }
        return string_section;
    }
}
