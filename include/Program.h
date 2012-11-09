#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <fstream>
#include <string>

#include "SyntaxErrorException.h"
#include "WTS_Parser.h"
#include "CCodeGenerator.h"
#include "GenInfoCodeGenerator.h"
#include "Interpreter.h"

class Program
{
    public:
        Program();
        virtual ~Program();
        int run(char* input_file_name_in, char* output_file_name_in);
    protected:
    private:
};

#endif // PROGRAM_H
