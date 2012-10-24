#ifndef WTS_PARSER_H
#define WTS_PARSER_H

#include <map>
#include <string>
#include <iostream>
#include "string_reader.h"
#include "ASTNode.h"

class WTS_Parser
{
    public:
        WTS_Parser();
        virtual ~WTS_Parser();
        int set_string(std::string input_string);
        int parse();
        std::string getCPP();
        void do_token(std::string token);

        //typedef void(*WTS_Parser()::token_handle_function)(std::string);
        //std::map<std::string, token_handle_function> wts_KeyWordsMap;
        std::map<std::string, void(WTS_Parser::*)(std::string)> wts_KeyWordsMap;
        void initialize_map();

    protected:
        bool reached_end;
        string_reader reader;
        std::string cpp_output;
        std::string next_token;

void wts_begin_function(std::string token);
void wts_end_function(std::string token);
void wts_go_function(std::string token);
void wts_print(std::string token);
void wts_integer(std::string token);
void wts_unsigned_integer(std::string token);
void wts_floating_point(std::string token);
void wts_boolean(std::string token);
void wts_end_statement(std::string token);
void wts_simple_token_replacement(std::string token);
void wts_equals(std::string token);
void wts_begin_if(std::string token);
void wts_end_if(std::string token);
void wts_begin_while(std::string token);
void wts_end_while(std::string token);

    private:
};

#endif // WTS_PARSER_H
