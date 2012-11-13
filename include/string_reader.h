#ifndef STRING_READER_H
#define STRING_READER_H

#include <vector>
#include <string>
#include <iostream>

class string_reader
{
    public:
        string_reader();
        string_reader(std::string input_string);
        virtual ~string_reader();
        void set_string(std::string input_string);
        std::string word(bool truncate_end = true);
        std::string line(bool truncate_end = true);
        std::string get_tokens(std::vector<std::string> get_chars, bool truncate_end = true);
        std::string truncate_end(std::string to_truncate);
    protected:
    private:
        std::string rd_string;
        int str_pos;
        bool end_reached;
};

#endif // STRING_READER_H
