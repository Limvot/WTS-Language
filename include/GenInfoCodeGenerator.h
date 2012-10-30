#ifndef GEN_INFO_CODE_GENERATOR_H
#define GEN_INFO_CODE_GENERATOR_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "CodeGenerator.h"
#include "Value.h"
#include "ASTNode_Prototype_Function.h"
#include "ASTNode_Statement.h"

#ifndef NULL
#define NULL 0
#endif



class GenInfoCodeGenerator : public CodeGenerator
{
    public:
        GenInfoCodeGenerator();
        ~GenInfoCodeGenerator();

        std::string toString(int in);

        void generate(AbstractSyntaxTree* tree);
        void do_node(ASTNode* current_node, std::string preface = std::string(""));
        std::string getOutput();
        
        //ASTNode*
    protected:
    	std::string output_info;
    private:
};

#endif // GEN_INFO_CODE_GENERATOR_H