#ifndef StringReader_H
#define StringReader_H

#include <vector>
#include <string>
#include <iostream>

class StringReader
{
    public:
        StringReader();
        StringReader(std::string inputString);
        virtual ~StringReader();
        void setString(std::string inputString);
        std::string word(bool truncateEnd = true);
        std::string line(bool truncateEnd = true);
        std::string getTokens(std::vector<std::string> get_chars, bool truncateEnd = true);
        std::string truncateEnd(std::string to_truncate);
    protected:
    private:
        std::string rd_string;
        int str_pos;
        bool end_reached;
};

#endif // StringReader_H
