#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <vector>
#include <map>
#include <string>

#include "AbstractSyntaxTree.h"
#include "ASTNode.h"
#include "ASTNode_Variable.h"
#include "ASTNode_Prototype_Function.h"

#ifndef NULL
#define NULL 0
#endif



class CodeGenerator
{
    public:
        CodeGenerator();
        virtual ~CodeGenerator();
        virtual void generate(AbstractSyntaxTree*) = 0;
        virtual std::string getOutput() = 0;

        //ASTNode*
    protected:
    private:
};

#endif // CODE_GENERATOR_H