#ifndef ASTNODE_VARIABLE_H
#define ASTNODE_VARIABLE_H

#include <ASTNode.h>
#include <Value.h>

class Value;				//I really hate circular dependencies. I have a lot of them too, maybe re-design this part

class ASTNode_Variable : public ASTNode
{
    public:
        ASTNode_Variable();
        virtual ~ASTNode_Variable();
        Value* value;
    protected:
    private:
};

#endif // ASTNODE_VARIABLE_H
