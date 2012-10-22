#ifndef ASTNODE_CALL_H
#define ASTNODE_CALL_H

#include <vector>

#include <ASTNode.h>
#include <ATSNode_Prototype_Function.h>
#include <Value.h>


class ASTNode_Call : public ASTNode
{
    public:
        ASTNode_Call();
        virtual ~ASTNode_Call();
        ATSNode_Prototype_Function* function;
        std::vector<Value*> parameters;
    protected:
    private:
};

#endif // ASTNODE_CALL_H
