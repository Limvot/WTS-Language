#ifndef ASTNODE_PROTOTYPE_VARIABLE_H
#define ASTNODE_PROTOTYPE_VARIABLE_H

#include <ASTNode_Prototype.h>
#include <ASTNode_Variable.h>


class ASTNode_Prototype_Variable : public ASTNode_Prototype
{
    public:
        ASTNode_Prototype_Variable(ASTNode_Variable* variable);
        virtual ~ASTNode_Prototype_Variable();
        ASTNode_Variable* variable;
    protected:
    private:
};

#endif // ASTNODE_PROTOTYPE_OBJECT_H
