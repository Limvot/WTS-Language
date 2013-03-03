#include "ASTNode_Statement.h"

ASTNode_Statement::ASTNode_Statement()
{
    type = statement;
    name = "statement";
    condition = NULL;
    firstOption = NULL;
    secondOption = NULL;
}

ASTNode_Statement::~ASTNode_Statement()
{
    if (condition)
    	delete condition;
    if (firstOption)
    	delete condition;
    if (secondOption)
    	delete condition;
}
