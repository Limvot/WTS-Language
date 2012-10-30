#ifndef C_CODE_GENERATOR_H
#define C_CODE_GENERATOR_H

#include <vector>
#include <map>
#include <string>

#include "CodeGenerator.h"

#ifndef NULL
#define NULL 0
#endif



class CCodeGenerator : public CodeGenerator
{
    public:
        CCodeGenerator();
        ~CCodeGenerator();

        void generate(AbstractSyntaxTree* tree);
        void do_node(ASTNode* current_node);
        std::string getOutput();
        
        //ASTNode*
    protected:
    	std::string output_c;
    private:
};

#endif // C_CODE_GENERATOR_H