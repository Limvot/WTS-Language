#ifndef GEN_INFO_CODE_GENERATOR_H
#define GEN_INFO_CODE_GENERATOR_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "CodeGenerator.h"
#include "Value.h"
#include "ASTNode_Prototype_Variable.h"
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
        void doNode(ASTNode* currentNode, std::string prefix = std::string(""));

        void doCallNode(ASTNode* currentNode, std::string prefix);
        void doStatementNode(ASTNode* currentNode, std::string prefix);
        void doBlockNode(ASTNode* currentNode, std::string prefix);
        void doPrototypeFunctionNode(ASTNode* currentNode, std::string prefix);
        void doValueNode(ASTNode* currentNode, std::string prefix);
        
        std::string getOutput();
        
        //ASTNode*
    protected:
    	std::string output_info;
    private:
};

#endif // GEN_INFO_CODE_GENERATOR_H