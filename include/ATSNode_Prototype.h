#ifndef ATSNODE_PROTOTYPE_H
#define ATSNODE_PROTOTYPE_H

#include <string>

#include <ATSNode.h>
#include <Block.h>



class ATSNode_Prototype : public ATSNode
{
    public:
        ATSNode_Prototype();
        virtual ~ATSNode_Prototype();
        Block* block;

    protected:
    private:
};

#endif // ATSNODE_PROTOTYPE_H
