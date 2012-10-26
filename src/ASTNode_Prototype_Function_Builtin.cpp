#include "ASTNode_Prototype_Function_Builtin.h"

ASTNode_Prototype_Function_Builtin::ASTNode_Prototype_Function_Builtin()
{
	//ctor
}

ASTNode_Prototype_Function_Builtin::ASTNode_Prototype_Function_Builtin(builtin_type incoming_type)
{
	operator_type = incoming_type;
}

ASTNode_Prototype_Function_Builtin::~ASTNode_Prototype_Function_Builtin()
{
	//dtor
}
