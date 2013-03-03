#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <sstream>

#include "Value.h"
#include "ASTNode_Variable.h"
#include "ASTNode_Prototype_Function.h"
#include "ASTNode_Prototype_Variable.h"
#include "ASTNode_Prototype_Function_Builtin.h"
#include "ASTNode_Call.h"
#include "AbstractSyntaxTree.h"
#include "ASTNode_Statement.h"

#ifndef NULL
#define NULL 0
#endif



class Interpreter
{
    public:
        Interpreter();
        ~Interpreter();

        std::string toString(int in);

        void interpret(AbstractSyntaxTree* tree);
        Value* doNode(ASTNode* currentNode);
        Value* doCallNode(ASTNode* currentNode);
        std::string getOutput();

        std::string doOperatorSymbol(ASTNode_Prototype_Function_Builtin::builtin_type);
        std::string doValueType(Value::value_type type_in);
        
        //ASTNode*
    protected:
    	std::string output_info;
    private:
};

#endif // INTERPRETER_H

//