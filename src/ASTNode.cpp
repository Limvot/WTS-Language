#include "ASTNode.h"

ASTNode::ASTNode()
{
    parent = NULL;
    num_children = 0;
    name = "node";
    type = basic;
}

ASTNode::ASTNode(ASTNode* incomingParent)
{
    parent = incomingParent;
    num_children = 0;
}

ASTNode::~ASTNode()
{
    children.clear();                               //Delete all our children
}

int ASTNode::setParent(ASTNode* incomingParent)
{
    if (parent)
        parent->removeChild(this);                  //Remove ourselves from parent, if it exists
    parent = incomingParent;
    return 0;
}

int ASTNode::addChild(ASTNode* incoming_child)
{
    children.push_back(incoming_child);
    incoming_child->setParent(this);
    num_children++;
    return 0;
}

int ASTNode::removeChild(ASTNode* remove_child)
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

int ASTNode::deleteChild(ASTNode* delete_child)
{
    int found_child = findChild(delete_child);
    if (found_child >= 0)
    {
        children.erase(children.begin()+found_child);//Erase and delete the child (the child at index from begining iterator
        num_children--;
    }
    else
    {
        return -1;                                  //Error if we didn't find the child
    }
    return 0;                                       //If no error, return sucess
}

int ASTNode::findChild(ASTNode* find_child)         //Replace with an unorderd_map lookup in the future?
{
    for (unsigned int i = 0; i < num_children; i++)
    {
        if (children[i] == find_child)
            return i;
    }
    return -1;                                      //Couldn't find child, return error
}

bool ASTNode::hasParent() {
    return (parent != NULL);
}

void ASTNode::setName(std::string name) {
    this->name = name;
}

std::string ASTNode::getLineage() {
    std::string lineage = "";
    if (hasParent())
        lineage = parent->getLineage() + "->";
    return(lineage + name);
}


