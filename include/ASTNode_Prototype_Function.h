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
        Value* function_body;                       //Function bodies are values, which can be blocks
        Value* returnType;
        Value* returnValue;

        enum function_type {
        						func_normal,
        						func_builtin } func_type;	//May add stuff like inline, lambda, pure, etc
        void setBody(Value* body);
        void setReturnValue(Value* returnValue);
    protected:
    private:
};

#endif // ASTNODE_PROTOTYPE_FUNCTION_H
