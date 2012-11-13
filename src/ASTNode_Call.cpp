#include "ASTNode_Call.h"

ASTNode_Call::ASTNode_Call()
{
    type = call;
    function = NULL;
}

ASTNode_Call::ASTNode_Call(ASTNode_Prototype_Function* incoming_function)
{
	type = call;
	function = incoming_function;
}

ASTNode_Call::~ASTNode_Call()
{
    //dtor
}
