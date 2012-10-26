#include "ASTNode_Prototype_Function.h"

ASTNode_Prototype_Function::ASTNode_Prototype_Function()
{
    type = prototype_function;
    num_parameters = 0;
    function_body = new Block;
    function_body->parent = this;
    return_value = new Value;
    return_value->parent = this;
    return_value->val_type = Value::typ_void;
}

ASTNode_Prototype_Function::~ASTNode_Prototype_Function()
{
    delete return_value;
}
