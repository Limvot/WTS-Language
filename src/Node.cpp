#include "Node.h"

Node::Node()
{
    parent = NULL;
    num_children = 0;
}

Node::Node(Node* incoming_parent)
{
    parent = incoming_parent;
    num_children = 0;
}

Node::~Node()
{
    children.clear();                               //Delete all our children
}

int Node::setParent(Node* incoming_parent)
{
    if (parent)
        parent->removeChild(this);                  //Remove ourselves from parent, if it exists
    parent = incoming_parent;
    return 0;
}

int Node::addChild(Node* incoming_child)
{
    children.push_back(incoming_child);
    incoming_child->setParent(this);
    num_children++;
    return 0;
}

int Node::removeChild(Node* remove_child)
{
    int found_child = findChild(remove_child);
    if (found_child >= 0)
    {
        children[found_child] = NULL;               //Can't erase, would delete the object
        num_children--;
    }
    else
    {
        return -1;                                  //Error if we didn't find the child
    }
    return 0;                                       //If no error, return sucess
}

int Node::deleteChild(Node* delete_child)
{
    int found_child = findChild(delete_child);
    if (found_child >= 0)
    {
        children.erase(children.begin()+found_child);               //Erase and delete the child (the child at index from begining iterator
        num_children--;
    }
    else
    {
        return -1;                                  //Error if we didn't find the child
    }
    return 0;                                       //If no error, return sucess
}

int Node::findChild(Node* find_child)                     //Replace with an unorderd_map lookup in the future?
{
    for (unsigned int i = 0; i < num_children; i++)
    {
        if (children[i] == find_child)
            return i;
    }
    return -1;                                      //Couldn't find child, return error
}
