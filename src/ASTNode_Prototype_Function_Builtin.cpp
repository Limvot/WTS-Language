#include "ASTNode_Prototype_Function_Builtin.h"



ASTNode_Prototype_Function_Builtin::ASTNode_Prototype_Function_Builtin(builtin_type incoming_type, std::string incoming_name)
{
	operatorType = incoming_type;
	name = incoming_name;
	funcType = func_builtin;

	if (operatorType == print)
		isBinary = false;
	else
		isBinary = true;

}

ASTNode_Prototype_Function_Builtin::~ASTNode_Prototype_Function_Builtin()
{
	//dtor
}
