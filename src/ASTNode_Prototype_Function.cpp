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
    
    returnType = new Value;
    returnType->setParent(this);
    returnType->val_type = Value::typ_void;

    returnValue = NULL;
}

ASTNode_Prototype_Function::~ASTNode_Prototype_Function()
{
    delete returnType;
    delete returnValue;
    delete function_body;
}

void ASTNode_Prototype_Function::setBody(Value* body) {
    function_body = body;
    body->setParent(this);
}

void ASTNode_Prototype_Function::setReturnValue(Value* returnValue) {
    this->returnValue = returnValue;

    if (returnValue)
        this->returnValue->setParent(this);
    else
        std::cout << "Return value attempted to be set for " << name << " is null." << std::endl;
}

