#include "ASTNode_Prototype_Function.h"

ASTNode_Prototype_Function::ASTNode_Prototype_Function()
{
    type = prototype_function;
    num_parameters = 0;
    return_value->val_type = Value::typ_void;
}

ASTNode_Prototype_Function::~ASTNode_Prototype_Function()
{
    //dtor
}
