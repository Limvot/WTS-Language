#include "ATSNode_Prototype_Function.h"

ATSNode_Prototype_Function::ATSNode_Prototype_Function()
{
    type = prototype_function;
    num_parameters = 0;
    return_type = Value::typ_void;
}

ATSNode_Prototype_Function::~ATSNode_Prototype_Function()
{
    //dtor
}
