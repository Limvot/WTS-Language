#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

#include <ATSNode.h>


class Block : public ATSNode
{
    public:
        Block();
        virtual ~Block();
        std::vector<ATSNode*> ATSNodes;
    protected:
    private:
};

#endif // BLOCK_H
