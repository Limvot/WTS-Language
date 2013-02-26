#include "ASTNode_Prototype_Variable.h"

ASTNode_Prototype_Variable::ASTNode_Prototype_Variable(ASTNode_Variable* variable)
{
    type = prototype_variable;
    name = "prototype_variable";
    this->variable = variable;
    variable->setParent(this);
}

ASTNode_Prototype_Variable::~ASTNode_Prototype_Variable()
{
    //dtor
}
