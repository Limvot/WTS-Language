#include "AbstractSyntaxTree.h"


AbstractSyntaxTree::AbstractSyntaxTree()
{
	root = static_cast<ASTNode*>(new Block);
    currentNode = root;
    root->setName("root");
}

AbstractSyntaxTree::~AbstractSyntaxTree()
{
    delete root;
    root = NULL;
    currentNode = NULL;
}

bool AbstractSyntaxTree::ascend() {

	if (currentNode->hasParent()) {
		currentNode = currentNode->parent;
		return(true);
	}

	return(false);
	
}

ASTNode* AbstractSyntaxTree::findAbove(ASTNode::ASTType searchNodeType) {

	ASTNode* currentSearchNode = NULL;

	if (currentNode->hasParent())
		currentSearchNode = currentNode->parent;
	else {
		std::cout << "currentNode does not have parent" << std::endl;
		return(NULL);
	}

	while (currentSearchNode->hasParent() && currentSearchNode->type != searchNodeType)
		currentSearchNode = currentSearchNode->parent;

	if (currentSearchNode->type == searchNodeType)
		return(currentSearchNode);

	std::cout << "Could not find above node of type " << searchNodeType << std::endl;
	return(NULL);
}

bool AbstractSyntaxTree::ascendToUpperBlock() {		//This class does not use the ascend() function because it only should change currentNode if there is indeed a block above it, i.e. after we've found it
	ASTNode* searchNode = findAbove(ASTNode::block);
	if (searchNode) {
		currentNode = searchNode;
		return(true);
	}
	return(false);
}
bool AbstractSyntaxTree::setCurrentNode(ASTNode* setNode) {
	currentNode = setNode;
	return true;
}

