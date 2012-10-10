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
    //Look up the word in the keywordmap, call assoicated function pointer with argument token
    void(WTS_Parser::* function_pointer)(std::string) = wts_KeyWordsMap[token];
    if (function_pointer != 0)
    {
        (this->*function_pointer)(token);
    }
    else
    {
        cpp_output += token;    //CHANGE THIS! If we don't know what it is, we output it. This is for varyables, but should be replaced with lookup table
    }
}


void WTS_Parser::wts_begin_function(std::string token)
{
    cpp_output += "int " + reader.word() + "() {\n";
}
void WTS_Parser::wts_end_function(std::string token)
{
    cpp_output += "return 0;\n}\n";
}
void WTS_Parser::wts_go_function(std::string token)
{
    cpp_output += reader.word() + "()";
}
void WTS_Parser::wts_print(std::string token)
{
    cpp_output += "std::cout << " + reader.line();
}
void WTS_Parser::wts_integer(std::string token)
{
    cpp_output += "int " + reader.word();
}
void WTS_Parser::wts_unsigned_integer(std::string token)
{
    cpp_output += "unsigned int " + reader.word();
}
void WTS_Parser::wts_floating_point(std::string token)
{
    cpp_output += "float " + reader.word();
}
void WTS_Parser::wts_boolean(std::string token)
{
    cpp_output += "bool " + reader.word();
}
void WTS_Parser::wts_end_statement(std::string token)
{
    cpp_output += token + "\n";
}
void WTS_Parser::wts_simple_token_replacement(std::string token)
{
    cpp_output += "( ";
    do_token(reader.word());
    cpp_output += " " + token + " ";
    do_token(reader.word());
    cpp_output += " )";
}
void WTS_Parser::wts_equals(std::string token)
{
    cpp_output += reader.word() + " = ";
    do_token(reader.word());
}
void WTS_Parser::wts_begin_if(std::string token)
{
    cpp_output += "if ( ";
    do_token(reader.word());
    cpp_output += ")\n{\n";
}
void WTS_Parser::wts_end_if(std::string token)
{
    cpp_output += "}\n";
}
void WTS_Parser::wts_begin_while(std::string token)
{
    cpp_output += "while ( ";
    do_token(reader.word());
    cpp_output += ")\n{\n";
}
void WTS_Parser::wts_end_while(std::string token)
{
    cpp_output += "}\n";
}

void WTS_Parser::initialize_map()
{
    wts_KeyWordsMap["begin"] = &WTS_Parser::wts_begin_function;
    wts_KeyWordsMap["end"] = &WTS_Parser::wts_end_function;
    wts_KeyWordsMap["go"] = &WTS_Parser::wts_go_function;
    wts_KeyWordsMap["print"] = &WTS_Parser::wts_print;
    wts_KeyWordsMap["int"] = &WTS_Parser::wts_integer;
    wts_KeyWordsMap["uint"] = &WTS_Parser::wts_unsigned_integer;
    wts_KeyWordsMap["float"] = &WTS_Parser::wts_floating_point;
    wts_KeyWordsMap["bool"] = &WTS_Parser::wts_boolean;
    wts_KeyWordsMap[";"] = &WTS_Parser::wts_end_statement;
    wts_KeyWordsMap["="] = &WTS_Parser::wts_equals;
    wts_KeyWordsMap["+"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["-"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["*"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["/"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["^"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["if"] = &WTS_Parser::wts_begin_if;
    wts_KeyWordsMap["endif"] = &WTS_Parser::wts_end_if;
    wts_KeyWordsMap["while"] = &WTS_Parser::wts_begin_while;
    wts_KeyWordsMap["endwhile"] = &WTS_Parser::wts_end_while;
    wts_KeyWordsMap["=="] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["!="] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["<"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap[">"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["<="] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap[">="] = &WTS_Parser::wts_simple_token_replacement;

    std::cout << "wts_KeyWordMap initilized, now contains " << wts_KeyWordsMap.size() << " entries.\n";
}

