#ifndef ASTNODE_PROTOTYPE_H
#define ASTNODE_PROTOTYPE_H

#include <string>

#include <ASTNode.h>
#include <Block.h>



class ASTNode_Prototype : public ASTNode
{
    public:
        ASTNode_Prototype();
        virtual ~ASTNode_Prototype();
        Block* block;

    protected:
    private:
};

#endif // ASTNODE_PROTOTYPE_H
