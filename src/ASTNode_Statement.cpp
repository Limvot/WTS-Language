#include "ASTNode_Statement.h"

ASTNode_Statement::ASTNode_Statement()
{
    type = statement;
    condition = NULL;
    first_option = NULL;
    second_option = NULL;
}

ASTNode_Statement::~ASTNode_Statement()
{
    if (condition)
    	delete condition;
    if (first_option)
    	delete condition;
    if (second_option)
    	delete condition;
}
