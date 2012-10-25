#ifndef ASTNODE_PROTOTYPE_FUNCTION_H
#define ASTNODE_PROTOTYPE_FUNCTION_H

#include <vector>

#include <ASTNode_Prototype.h>
#include <Value.h>

class Value;	//Because functions can both be and use values. Fix the circular dependency, close the loop.

class ASTNode_Prototype_Function : public ASTNode_Prototype
{
    public:
        ASTNode_Prototype_Function();
        virtual ~ASTNode_Prototype_Function();
        std::vector<Value*> parameters;
        unsigned int num_parameters;
        Value* return_value;						//Will set its type, but not its data, so can figure out what this thing returns
    protected:
    private:
};

#endif // ASTNODE_PROTOTYPE_FUNCTION_H
