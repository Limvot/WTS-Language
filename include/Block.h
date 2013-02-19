#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

#include <ASTNode.h>


class Block : public ASTNode
{
    public:
        Block();
        Block(ASTNode*);
        virtual ~Block();
    protected:
    private:
};

#endif // BLOCK_H
