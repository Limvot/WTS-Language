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

bool AbstractSyntaxTree::ascendToUpperBlock() {		//This class does not use the ascend() function because it only should change currentNode if there is indeed a block above it, i.e. after we've found it

	ASTNode* currentSearchNode = NULL;

	if (currentNode->hasParent())
		currentSearchNode = currentNode->parent;
	else
		return(false);

	std::cout << "Current search node had parent" << std::endl;

	while (currentSearchNode->hasParent() && currentSearchNode->type != ASTNode::block) {
		currentSearchNode = currentSearchNode->parent;
		std::cout << "while condition true, going up a level" << std::endl;
	}

	if (currentSearchNode->type == ASTNode::block) {
		currentNode = currentSearchNode;
		std::cout << "found block node, suceeded!" << std::endl;
		return(true);
	}

	std::cout << "top node not block, failed" << std::endl;

	return(false);
}
bool AbstractSyntaxTree::setCurrentNode(ASTNode* setNode) {
	currentNode = setNode;  
}

