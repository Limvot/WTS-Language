#ifndef ASTNODE_CALL_H
#define ASTNODE_CALL_H

#include <vector>

#include <ASTNode.h>
#include <ASTNode_Prototype_Function.h>
#include <Value.h>

// I hate circular dependencies
class ASTNode_Prototype_Function;
class Value;


class ASTNode_Call : public ASTNode
{
    public:
        ASTNode_Call();
        ASTNode_Call(ASTNode_Prototype_Function* incoming_function);
        virtual ~ASTNode_Call();
        ASTNode_Prototype_Function* function;
        std::vector<Value*> parameters;
    protected:
    private:
};

#endif // ASTNODE_CALL_H
