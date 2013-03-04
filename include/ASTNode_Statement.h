#ifndef ASTNODE_STATEMENT_H
#define ASTNODE_STATEMENT_H

#include <ASTNode.h>
#include <Value.h>
#include <Block.h>

class Value;

class ASTNode_Statement : public ASTNode
{
    public:
        ASTNode_Statement();
        virtual ~ASTNode_Statement();
        enum state_type	{	if_statement,
        					while_statement,
        					for_statement,
        					do_while_statement} statementType;
        Value* condition;
        Value* firstOption;    //Values can be blocks
        Value* secondOption;
    protected:
    private:
};

#endif // ASTNODE_STATEMENT_H
