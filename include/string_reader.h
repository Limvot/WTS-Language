#ifndef STRING_READER_H
#define STRING_READER_H

#include <string>
#include <iostream>

class string_reader
{
    public:
        string_reader();
        string_reader(std::string input_string);
        virtual ~string_reader();
        void set_string(std::string input_string);
        std::string word();
        std::string line();
    protected:
    private:
        std::string rd_string;
        int str_pos;
        bool end_reached;
};

#endif // STRING_READER_H
