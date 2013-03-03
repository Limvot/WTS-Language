#include "Block.h"

Block::Block()
{
    type = block;
    name = "block";
}

Block::Block(ASTNode* incomingParent)
{
	setParent(incomingParent);
    type = block;
    name = "block";
}

Block::~Block()
{
    //dtor
}
