#include "ASTNode_Prototype_Function.h"

ASTNode_Prototype_Function::ASTNode_Prototype_Function()
{
    type = prototype_function;
    name = "prototype_function";
    num_parameters = 0;
    func_type = func_normal;
    function_body = new Value();
    function_body->setParent(this);
    function_body->data.dat_block = new Block(function_body);
    function_body->data.dat_block->setParent(function_body);
    function_body->val_type = Value::typ_block;
    
    return_value = new Value;
    return_value->setParent(this);
    return_value->val_type = Value::typ_void;
}

ASTNode_Prototype_Function::~ASTNode_Prototype_Function()
{
    delete return_value;
}

void ASTNode_Prototype_Function::setBody(Value* body) {
    function_body = body;
    body->setParent(this);
}
