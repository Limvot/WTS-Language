#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "ASTNode.h"
#include "ASTNode_Variable.h"
#include "ASTNode_Prototype_Function.h"

#ifndef NULL
#define NULL 0
#endif



class AbstractSyntaxTree
{
    public:
        AbstractSyntaxTree();
        ~AbstractSyntaxTree();

        bool ascend();
        bool ascendToUpperBlock();
        bool setCurrentNode(ASTNode* setNode);

        ASTNode* root;
        ASTNode* currentNode;
        std::map<std::string, ASTNode_Prototype_Function*> functions;
        std::map<std::string, ASTNode_Variable*> variables;
        
    protected:
    private:
};

#endif // ABSTRACTSYNTAXTREE_H