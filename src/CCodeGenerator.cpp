#include "CCodeGenerator.h"


CCodeGenerator::CCodeGenerator()
{
    //ctor
}

CCodeGenerator::~CCodeGenerator()
{
    //dtor
}

void CCodeGenerator::generate(AbstractSyntaxTree* tree)
{
	output_c = "#include <stdio.h>\n\n";
	if (tree->root.num_children)
	{
		for (unsigned int i = 0; i < tree->root.num_children; i++)
		{
			do_node(tree->root.children[i]);							//Go through each child of the root node
		}
	}
}

std::string CCodeGenerator::getOutput()
{
	return output_c;
}

void CCodeGenerator::do_node(ASTNode* current_node)
{
	switch (current_node->type)
	{
		case ASTNode::call:
			output_c += "call:" + static_cast<ASTNode_Call*>(current_node)->function->name + "\n";
			break;

		case ASTNode::variable:
			output_c += "variable:" + current_node->name + "\n";
			break;

		case ASTNode::statement:
			output_c += "statement:" + current_node->name + "\n";
			break;

		case ASTNode::block:
			output_c += "block: Why are we here?\n";
			break;

		case ASTNode::prototype:
			output_c += "Plain prototype: Again, why are we here?\n";
			break;

		case ASTNode::prototype_function:
			output_c += "Function prototype: " + current_node->name + "\n";
			break;

		case ASTNode::prototype_object:
			output_c += "Object prototype: Again, why are we here?\n";
			break;

		case ASTNode::value:
			output_c += "Value: This could get complicated\n";
			break;

		case ASTNode::basic:
			output_c += "Basic node: Can we get here? I don't even know anymore...\n";
			break;
	}
}
