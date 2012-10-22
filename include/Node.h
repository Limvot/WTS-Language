#ifndef NODE_H
#define NODE_H

#include <vector>

#ifndef NULL
#define NULL 0
#endif

class Node
{
    public:
        Node();
        Node(Node* incoming_parent);
        virtual ~Node();

        int setParent(Node* incoming_parent);
        int addChild(Node* incoming_child);
        int removeChild(Node* remove_child);
        int deleteChild(Node* delete_child);
        int findChild(Node* find_child);

        Node* parent;
        std::vector<Node*> children;
        unsigned int num_children;
    protected:
    private:
};

#endif // NODE_H
