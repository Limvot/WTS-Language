#include "ASTNode_Variable.h"

ASTNode_Variable::ASTNode_Variable()
{
    type = variable;
    name = "variable";
    value = new Value;
    value->setParent(this);
}

ASTNode_Variable::~ASTNode_Variable()
{
    delete value;
}
