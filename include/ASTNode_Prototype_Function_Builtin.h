#ifndef ASTNODE_PROTOTYPE_FUNCTION_BUILTIN_H
#define ASTNODE_PROTOTYPE_FUNCTION_BUILTIN_H

#include <vector>

#include <ASTNode_Prototype_Function.h>
#include <Value.h>

class Value;	//Because functions can both be and use values. Fix the circular dependency, close the loop.

class ASTNode_Prototype_Function_Builtin : public ASTNode_Prototype_Function
{
    public:
    	enum builtin_type {	none,
                            print,
                            assignment,
                            addition,
        					subtraction,
        					multiplication,
        					division,
        					exponent,
        					equal,
        					not_equal,
        					greater_than,
        					less_than,
        					greater_than_or_equal,
        					less_than_or_equal} operator_type;

        bool is_binary;

        ASTNode_Prototype_Function_Builtin(builtin_type incoming_type = none, std::string incoming_name = "unamed_builtin");
        virtual ~ASTNode_Prototype_Function_Builtin();

    protected:
    private:
};

#endif // ASTNODE_PROTOTYPE_FUNCTION_BUILTIN_H