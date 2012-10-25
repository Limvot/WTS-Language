#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

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

        ASTNode root;
        ASTNode* current_node;
        std::map<std::string, ASTNode_Prototype_Function*> functions;   //Be careful, I'm not sure but this could segfault if it deletes functiosn and the tree deletes the functions
        std::map<std::string, ASTNode_Variable*> variables;                        //Ditto for variables
    protected:
    private:
};

#endif // ABSTRACTSYNTAXTREE_H