#include "Block.h"

Block::Block()
{
    type = block;
    name = "block";
}

Block::Block(ASTNode* incoming_parent)
{
	setParent(incoming_parent);
    type = block;
    name = "block";
}

Block::~Block()
{
    //dtor
}
