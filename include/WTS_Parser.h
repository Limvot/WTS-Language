#ifndef WTS_PARSER_H
#define WTS_PARSER_H

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
    protected:
        bool reached_end;
        string_reader reader;
        std::string cpp_output;

    private:
};

#endif // WTS_PARSER_H
