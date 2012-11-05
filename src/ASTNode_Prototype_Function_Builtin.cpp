#include "ASTNode_Prototype_Function_Builtin.h"



ASTNode_Prototype_Function_Builtin::ASTNode_Prototype_Function_Builtin(builtin_type incoming_type, std::string incoming_name)
{
	operator_type = incoming_type;
	name = incoming_name;
	func_type = func_builtin;

	if (operator_type == print)
		is_binary = false;
	else
		is_binary = true;

}

ASTNode_Prototype_Function_Builtin::~ASTNode_Prototype_Function_Builtin()
{
	//dtor
}
