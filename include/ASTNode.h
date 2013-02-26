#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <string>

#ifndef NULL
#define NULL 0
#endif



class ASTNode
{
    public:
        ASTNode();
        ASTNode(ASTNode* incoming_parent);
        virtual ~ASTNode();

        int setParent(ASTNode* incoming_parent);
        int addChild(ASTNode* incoming_child);
        int removeChild(ASTNode* remove_child);
        int deleteChild(ASTNode* delete_child);
        int findChild(ASTNode* find_child);

        bool hasParent();

        void setName(std::string name);
        std::string getLineage();

        ASTNode* parent;
        std::vector<ASTNode*> children;
        unsigned int num_children;

        std::string name;
        enum ASTType {      call,
                            variable,
                            statement,
                            block,
                            prototype,
                            prototype_function,
                            prototype_object,
                            prototype_variable,
                            value,
                            basic
                            } type;
    protected:
    private:
};

#endif // ASTNODE_H