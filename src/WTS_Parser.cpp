#include "WTS_Parser.h"

WTS_Parser::WTS_Parser()
{
    reached_end = false;
    initialize_map();   //Setup out map of keywords
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
    cpp_output = "#include <iostream>\n";

    std::string current_word;
    while (!reached_end)
    {
        current_word = reader.word(false);                          //Do not truncate ending char
        std::cout << "Current WORD/LINE =||" << current_word << "||end of word\n";
        if (current_word == "")                                     //Reached end-of-file
        {
            reached_end = true;
        } else if (current_word[0] == '#')                          //If comment
        {
            if (current_word[current_word.length()-1] == '\n')      //If a one word comment (line ending right after word), just make a comment with the one word
                cpp_output.append("//" + current_word);
            else                                                    //If multiple word comment, add the rest of the line (which has newline in it, if we don't truncate)
                cpp_output.append("//" + current_word + reader.line(false));

        } else {
            do_token(reader.truncate_end(current_word));            //Evaluate the token, which is the word without the ending char
        }
    }

    std::cout << "Done parsing!\n";

    return 0;
}

std::string WTS_Parser::getCPP()
{
    return cpp_output;
}

void WTS_Parser::do_token(std::string token)
{
    switch (wts_KeyWordsMap[token]) //Look up the word in the keywordmap
    {
        case wts_begin_function:
            cpp_output += "int " + reader.word() + "() {\n";
            break;

        case wts_end_function:
            cpp_output += "return 0;\n}\n";
            break;

        case wts_go_function:
            cpp_output += reader.word() + "();\n";
            break;

        case wts_print:
            cpp_output += "std::cout << " + reader.line() + ";\n";
            break;

        case wts_integer:
            cpp_output += "int " + reader.word() + ";\n";
            break;

        case wts_unsigned_integer:
            cpp_output += "unsigned int " + reader.word() + ";\n";
            break;

        case wts_floating_point:
            cpp_output += "float " + reader.word() + ";\n";
            break;

        case wts_boolean:
            cpp_output += "bool " + reader.word() + ";\n";
            break;

        case wts_equals:
        {
            cpp_output += reader.word() + " = ";
            next_token = reader.word();
            switch (wts_KeyWordsMap[next_token]) {
                case wts_plus:
                    cpp_output += reader.word() + " + ";
                    cpp_output += reader.word() + ";\n";
                    break;

                case wts_minus:
                    cpp_output += reader.word() + " - ";
                    cpp_output += reader.word() + ";\n";
                    break;

                case wts_times:
                    cpp_output += reader.word() + " * ";
                    cpp_output += reader.word() + ";\n";
                    break;

                case wts_divide:
                    cpp_output += reader.word() + " / ";
                    cpp_output += reader.word() + ";\n";
                    break;

                case wts_exponent:
                    cpp_output += reader.word() + " ^ ";
                    cpp_output += reader.word() + ";\n";
                    break;

                default:
                    cpp_output += next_token + ";\n";
                    break;
            }
        }
            break;

        case wts_begin_if:
            {
            cpp_output += "if (";
            next_token = reader.word();
            switch (wts_KeyWordsMap[next_token])
            {
                case wts_is_equal:
                    cpp_output += reader.word() + " == ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_is_not_equal:
                    cpp_output += reader.word() + " != ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_less_than:
                    cpp_output += reader.word() + " < ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_greater_than:
                    cpp_output += reader.word() + " > ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_less_than_or_equal:
                    cpp_output += reader.word() + " <= ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_greater_than_or_equal:
                    cpp_output += reader.word() + " >= ";
                    cpp_output += reader.word() + ")\n";
                    break;

                default:
                    std::cout << "Error: Expected comparative operator\n";
            }
            cpp_output += "{\n";
            }
            break;

        case wts_end_if:
            cpp_output += "}\n";
            break;

        case wts_begin_while:
            {
            cpp_output += "while (";
            next_token = reader.word();
            switch (wts_KeyWordsMap[next_token])
            {
                case wts_is_equal:
                    cpp_output += reader.word() + " == ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_is_not_equal:
                    cpp_output += reader.word() + " != ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_less_than:
                    cpp_output += reader.word() + " < ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_greater_than:
                    cpp_output += reader.word() + " > ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_less_than_or_equal:
                    cpp_output += reader.word() + " <= ";
                    cpp_output += reader.word() + ")\n";
                    break;

                case wts_greater_than_or_equal:
                    cpp_output += reader.word() + " >= ";
                    cpp_output += reader.word() + ")\n";
                    break;

                default:
                    std::cout << "Error: Expected comparative operator\n";
            }
            cpp_output += "{\n";
            }
            break;

        case wts_end_while:
            cpp_output += "}\n";
            break;


        default:
            std::cout << "Error, unreacognised token: ||" + token + "||end_token\n";
    }
}

void WTS_Parser::initialize_map()
{
    wts_KeyWordsMap["begin"] = wts_begin_function;
    wts_KeyWordsMap["end"] = wts_end_function;
    wts_KeyWordsMap["go"] = wts_go_function;
    wts_KeyWordsMap["print"] = wts_print;
    wts_KeyWordsMap["int"] = wts_integer;
    wts_KeyWordsMap["uint"] = wts_unsigned_integer;
    wts_KeyWordsMap["float"] = wts_floating_point;
    wts_KeyWordsMap["bool"] = wts_boolean;
    wts_KeyWordsMap["="] = wts_equals;
    wts_KeyWordsMap["+"] = wts_plus;
    wts_KeyWordsMap["-"] = wts_minus;
    wts_KeyWordsMap["*"] = wts_times;
    wts_KeyWordsMap["/"] = wts_divide;
    wts_KeyWordsMap["^"] = wts_exponent;
    wts_KeyWordsMap["if"] = wts_begin_if;
    wts_KeyWordsMap["endif"] = wts_end_if;
    wts_KeyWordsMap["while"] = wts_begin_while;
    wts_KeyWordsMap["endwhile"] = wts_end_while;
    wts_KeyWordsMap["=="] = wts_is_equal;
    wts_KeyWordsMap["!="] = wts_is_not_equal;
    wts_KeyWordsMap["<"] = wts_less_than;
    wts_KeyWordsMap[">"] = wts_greater_than;
    wts_KeyWordsMap["<="] = wts_less_than_or_equal;
    wts_KeyWordsMap[">="] = wts_greater_than_or_equal;

    std::cout << "wts_KeyWordMap initilized, now contains " << wts_KeyWordsMap.size() << " entries.\n";
}

