#include "ASTNode_Variable.h"

ASTNode_Variable::ASTNode_Variable()
{
    type = variable;
    value = NULL;
}

ASTNode_Variable::~ASTNode_Variable()
{
    //dtor
}
