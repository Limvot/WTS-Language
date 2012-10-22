#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>

#include <Node.h>


class ASTNode : public Node
{
    public:
        ASTNode();
        virtual ~ASTNode();
        std::string name;
        enum ATSType {      call,
                            variable,
                            statement,
                            block,
                            prototype,
                            prototype_function,
                            prototype_object,
                            basic
                            } type;
    protected:
    private:
};

#endif // ASTNODE_H
