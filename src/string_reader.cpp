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

std::string string_reader::word(bool truncate_end)
{
    std::vector<std::string> stop_chars;
    stop_chars.push_back(" ");
    stop_chars.push_back("\n");
    stop_chars.push_back("\t");
    return get_tokens(stop_chars, truncate_end);
}

std::string string_reader::line(bool truncate_end)
{
    std::vector<std::string> stop_chars;
    stop_chars.push_back("\n");
    return get_tokens(stop_chars, truncate_end);
}

std::string string_reader::get_tokens(std::vector<std::string> stop_chars, bool truncate_end)
{
    int found_pos, new_found_pos;
    std::string stop_char;

    found_pos = rd_string.find(stop_chars[0], str_pos);
    stop_char = stop_chars[0];

    for (unsigned int i = 1; i < stop_chars.size(); i++)
    {
        new_found_pos = rd_string.find(stop_chars[i], str_pos);
        
        if ((new_found_pos <= found_pos) & (new_found_pos != std::string::npos))
        {
            found_pos = new_found_pos;
            stop_char = stop_chars[i];
        }
    }

    if (found_pos == str_pos)                                   //We are at the endline
    {
        str_pos++;
        return stop_char;
    }

    if (found_pos == std::string::npos)                         //We are at the end of the file
    {
        //End of String
        end_reached = true;
        std::cout << "Reached end of file!\n";
        return "";
    } else {

        std::string string_section;

        if (truncate_end)                                       //If we want to get rid of the delimiting character, which is the default, don't add the last char. Note we have to increase str_pos by one manually later
            found_pos -= 1;

        for (; str_pos <= found_pos; str_pos++)
        {
            string_section = string_section + rd_string[str_pos];
        }

        if (truncate_end)                                       //Ok, we didn't add the last char, but str_pos now points at that char. So we move it one ahead.
            str_pos++;
        return string_section;
    }
}

std::string string_reader::truncate_end(std::string to_truncate)
{
    std::string to_return = "";
    for (unsigned int i = 0; i < to_truncate.length()-1; i++)
        to_return = to_return + to_truncate[i];
    return to_return;
}
