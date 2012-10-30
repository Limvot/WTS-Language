#ifndef ASTNODE_STATEMENT_H
#define ASTNODE_STATEMENT_H

#include <ASTNode.h>
#include <Value.h>
#include <Block.h>

class ASTNode_Statement : public ASTNode
{
    public:
        ASTNode_Statement();
        virtual ~ASTNode_Statement();
        enum state_type	{	if_statement,
        					while_statement,
        					for_statement,
        					do_while_statement} statement_type;
        Value* condition;
        Block* first_option;
        Block* second_option;
    protected:
    private:
};

#endif // ASTNODE_STATEMENT_H
