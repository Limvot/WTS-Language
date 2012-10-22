#include "ASTNode_Call.h"

ASTNode_Call::ASTNode_Call()
{
    type = call;
    function = NULL;
}

ASTNode_Call::~ASTNode_Call()
{
    //dtor
}
