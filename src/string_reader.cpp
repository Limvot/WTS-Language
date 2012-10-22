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
    return get_tokens(" ", "\n", truncate_end);
}

std::string string_reader::line(bool truncate_end)
{
    return get_tokens("\n", truncate_end);
}

std::string string_reader::get_tokens(std::string stop_char1, bool truncate_end)
{
    return get_tokens(stop_char1, "", truncate_end);
}

std::string string_reader::get_tokens(std::string stop_char1, std::string stop_char2, bool truncate_end)
{
    int found_pos;
    std::string stop_char;
    if (stop_char2 == "")                                   //If we only have one argument, do it for that
    {
        stop_char = stop_char1;
        found_pos = rd_string.find(stop_char1, str_pos);

    } else {                                                //If we have two, search for both and use the positing and character that occurs sooner
        int found_pos1, found_pos2;
        found_pos1 = rd_string.find(stop_char1, str_pos);
        found_pos2 = rd_string.find(stop_char2, str_pos);

        if (found_pos1 <= found_pos2)
        {
            found_pos = found_pos1;
            stop_char = stop_char1;
        } else {
            found_pos = found_pos2;
            stop_char = stop_char2;
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
