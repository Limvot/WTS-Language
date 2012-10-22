#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

#include <ASTNode.h>


class Block : public ASTNode
{
    public:
        Block();
        virtual ~Block();
        std::vector<ASTNode*> ASTNodes;
    protected:
    private:
};

#endif // BLOCK_H
