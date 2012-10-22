#ifndef ATSNODE_PROTOTYPE_H
#define ATSNODE_PROTOTYPE_H

#include <string>

#include <ASTNode.h>
#include <Block.h>



class ATSNode_Prototype : public ASTNode
{
    public:
        ATSNode_Prototype();
        virtual ~ATSNode_Prototype();
        Block* block;

    protected:
    private:
};

#endif // ATSNODE_PROTOTYPE_H
