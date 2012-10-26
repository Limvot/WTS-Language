#include "ASTNode_Variable.h"

ASTNode_Variable::ASTNode_Variable()
{
    type = variable;
    value = new Value;
    value->parent = this;
}

ASTNode_Variable::~ASTNode_Variable()
{
    delete value;
}
