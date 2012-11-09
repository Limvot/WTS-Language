#ifndef INTERPRETER_H
#define INTERPRETER_H

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



class Interpreter : public CodeGenerator
{
    public:
        Interpreter();
        ~Interpreter();

        std::string toString(int in);

        void interpret(AbstractSyntaxTree* tree);
        void do_node(ASTNode* current_node, std::string prefix = std::string(""), std::string ending_statement = std::string(""));
        std::string getOutput();

        std::string doOperatorSymbol(ASTNode_Prototype_Function_Builtin::builtin_type);
        std::string doValueType(Value::value_type type_in);
        
        //ASTNode*
    protected:
    	std::string output_c;
    private:
};

#endif // INTERPRETER_H

//