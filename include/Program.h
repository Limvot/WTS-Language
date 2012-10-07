#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <fstream>
#include <string>

#include "WTS_Parser.h"

class Program
{
    public:
        Program();
        virtual ~Program();
        int run();
    protected:
    private:
};

#endif // PROGRAM_H
