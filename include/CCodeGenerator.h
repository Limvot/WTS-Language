#ifndef C_CODE_GENERATOR_H
#define C_CODE_GENERATOR_H

#include <string>
#include <sstream>

#include "CodeGenerator.h"
#include "Value.h"
#include "ASTNode_Prototype_Function.h"
#include "ASTNode_Statement.h"

#ifndef NULL
#define NULL 0
#endif



class CCodeGenerator : public CodeGenerator
{
    public:
        CCodeGenerator();
        ~CCodeGenerator();

        std::string toString(int in);

        void generate(AbstractSyntaxTree* tree);
        void do_node(ASTNode* current_node, std::string prefix = std::string(""));
        std::string getOutput();
        
        //ASTNode*
    protected:
    	std::string output_c;
    private:
};

#endif // C_CODE_GENERATOR_H

