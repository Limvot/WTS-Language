#ifndef WTS_PARSER_H
#define WTS_PARSER_H

#include <map>
#include <string>
#include <iostream>
#include "string_reader.h"

class WTS_Parser
{
    public:
        WTS_Parser();
        virtual ~WTS_Parser();
        int set_string(std::string input_string);
        int parse();
        std::string getCPP();
        void do_token(std::string token);

        enum KeyWords {  wts_not_defined,
                                wts_begin_function,
                                wts_end_function,
                                wts_go_function,
                                wts_print,
                                wts_integer,
                                wts_unsigned_integer,
                                wts_floating_point,
                                wts_boolean,
                                wts_equals,
                                wts_plus,
                                wts_minus,
                                wts_times,
                                wts_divide,
                                wts_exponent,
                                wts_begin_if,
                                wts_end_if,
                                wts_begin_while,
                                wts_end_while,
                                wts_is_equal,
                                wts_is_not_equal,
                                wts_less_than,
                                wts_greater_than,
                                wts_less_than_or_equal,
                                wts_greater_than_or_equal
                                };
        std::map<std::string, KeyWords> wts_KeyWordsMap;

        void initialize_map();

    protected:
        bool reached_end;
        string_reader reader;
        std::string cpp_output;
        std::string next_token;

    private:
};

#endif // WTS_PARSER_H
