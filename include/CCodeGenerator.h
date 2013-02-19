#ifndef C_CODE_GENERATOR_H
#define C_CODE_GENERATOR_H

#include <iostream>
#include <string>
#include <sstream>

#include "CodeGenerator.h"
#include "Value.h"
#include "ASTNode_Variable.h"
#include "ASTNode_Prototype_Function.h"
#include "ASTNode_Prototype_Function_Builtin.h"

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
        void do_node(ASTNode* currentNode, std::string prefix = std::string(""), std::string ending_statement = std::string(""));
        
        void doCallNode(ASTNode* currentNode, std::string prefix, std::string ending_statement);
        void doVariableNode(ASTNode* currentNode, std::string prefix, std::string ending_statement);
        void doStatementNode(ASTNode* currentNode, std::string prefix, std::string ending_statement);
        void doBlockNode(ASTNode* currentNode, std::string prefix, std::string ending_statement);
        void doPrototypeFunctionNode(ASTNode* currentNode, std::string prefix, std::string ending_statement);
        void doValueNode(ASTNode* currentNode, std::string prefix, std::string ending_statement);

        std::string getOutput();

        std::string doOperatorSymbol(ASTNode_Prototype_Function_Builtin::builtin_type);
        std::string doValueType(Value::value_type type_in);


        
        //ASTNode*
    protected:
    	std::string output_c;
    private:
};

#endif // C_CODE_GENERATOR_H

//