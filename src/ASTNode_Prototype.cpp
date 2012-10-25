#include "ASTNode_Prototype.h"

ASTNode_Prototype::ASTNode_Prototype()
{
    type = prototype;
    block = new Block();
}

ASTNode_Prototype::~ASTNode_Prototype()
{
    //dtor
}
