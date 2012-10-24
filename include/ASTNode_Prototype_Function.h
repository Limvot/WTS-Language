#ifndef ASTNODE_PROTOTYPE_FUNCTION_H
#define ASTNODE_PROTOTYPE_FUNCTION_H

#include <vector>

#include <ASTNode_Prototype.h>
#include <Value.h>


class ASTNode_Prototype_Function : public ASTNode_Prototype
{
    public:
        ASTNode_Prototype_Function();
        virtual ~ASTNode_Prototype_Function();
        std::vector<Value*> parameters;
        unsigned int num_parameters;
        Value::value_type return_type;
    protected:
    private:
};

#endif // ASTNODE_PROTOTYPE_FUNCTION_H
