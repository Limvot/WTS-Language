#ifndef ATSNODE_VARIABLE_H
#define ATSNODE_VARIABLE_H

#include <ASTNode.h>
#include <Value.h>


class ATSNode_Variable : public ASTNode
{
    public:
        ATSNode_Variable();
        virtual ~ATSNode_Variable();
        Value* value;
    protected:
    private:
};

#endif // ATSNODE_VARIABLE_H
