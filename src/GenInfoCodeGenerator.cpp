#include "GenInfoCodeGenerator.h"


GenInfoCodeGenerator::GenInfoCodeGenerator()
{
    //ctor
}

GenInfoCodeGenerator::~GenInfoCodeGenerator()
{
    //dtor
}

void GenInfoCodeGenerator::generate(AbstractSyntaxTree* tree)
{
	output_info = "/*\n";
	if (tree->root->num_children)
	{
		output_info = output_info + "Root node has " + toString(tree->root->num_children) + " children.\n";
		std::cout << "Root node has " << tree->root->num_children << " children." << std::endl;
		for (unsigned int i = 0; i < tree->root->num_children; i++)
		{
			do_node(tree->root->children[i]);							//Go through each child of the root node
		}
	}
	output_info += "*/\n";
}

std::string GenInfoCodeGenerator::toString(int in)
{
	std::stringstream out_ss;
	out_ss << in;
	return out_ss.str();
}

std::string GenInfoCodeGenerator::getOutput()
{
	return output_info;
}

void GenInfoCodeGenerator::do_node(ASTNode* currentNode, std::string prefix)				//The prefix is added to each line added to the output, so that we can have multiple indents through recursive functions
{
	//std::cout << "doing node named " << currentNode->name << std::endl;
	switch (currentNode->type)
	{
		case ASTNode::call:
			doCallNode(currentNode, prefix);
			break;

		case ASTNode::variable:
			output_info += prefix + "variable:" + currentNode->name + "\n";
			break;

		case ASTNode::statement:
			doStatementNode(currentNode, prefix);
			break;

		case ASTNode::block:
			doBlockNode(currentNode, prefix);
			break;

		case ASTNode::prototype:
			output_info += prefix + "Plain prototype: Again, why are we here?\n";
			break;

		case ASTNode::prototype_function:
			doPrototypeFunctionNode(currentNode, prefix);
			break;

		case ASTNode::prototype_object:
			output_info += prefix + "Object prototype: Again, why are we here?\n";
			break;

		case ASTNode::value:
			doValueNode(currentNode, prefix);
			break;

		case ASTNode::basic:
			output_info += prefix + "Basic node: Can we get here? I don't even know anymore...\n";
			break;
	}
}

void GenInfoCodeGenerator::doCallNode(ASTNode* currentNode, std::string prefix) 
{
	ASTNode_Call* current_call_node = static_cast<ASTNode_Call*>(currentNode);
	output_info += prefix + "call:" + current_call_node->function->name + "\n";
	for (unsigned int i = 0; i < current_call_node->parameters.size(); i++)
	{
		Value* current_value_node = current_call_node->parameters[i];
		if (current_value_node)
		{
			output_info += prefix + "\tParameter " + toString(i) + ": type = " + toString(current_value_node->val_type) + "\n";
			if (current_value_node->val_type == Value::typ_call)
				do_node(current_value_node->data.dat_call, prefix+"\t");
		}
		else
			output_info += prefix + "\tParameter " + toString(i) + " is NULL.\n";
	}
}

void GenInfoCodeGenerator::doStatementNode(ASTNode* currentNode, std::string prefix)
{
	ASTNode_Statement* statement_node = dynamic_cast<ASTNode_Statement*> (currentNode);
	output_info += prefix + "statement:" + currentNode->name + " Type:" + toString(statement_node->statement_type) + ", ";
		switch (statement_node->statement_type)
		{
			case ASTNode_Statement::if_statement:
			{
				output_info += "If statement\n";
				output_info += prefix + "\tCondition:\n";
				do_node(statement_node->condition, prefix+"\t\t");
				output_info += prefix + "\tExecutable Block:\n";
				do_node(statement_node->first_option, prefix+"\t\t");
				break;
			}

			case ASTNode_Statement::while_statement:
				output_info += "While statement\n";
				output_info += prefix + "\tCondition:\n";
				do_node(statement_node->condition, prefix+"\t\t");
				output_info += prefix + "\tExecutable Block:\n";
				do_node(statement_node->first_option, prefix+"\t\t");
				break;

			default:
				break;
	}
}

void GenInfoCodeGenerator::doBlockNode(ASTNode* currentNode, std::string prefix)
{
	output_info += prefix + "block: " + toString(currentNode->children.size()) + " children.\n";
				for (unsigned int i = 0; i < currentNode->children.size(); i++)
						do_node(currentNode->children[i], prefix+"\t");
}

void GenInfoCodeGenerator::doPrototypeFunctionNode(ASTNode* currentNode, std::string prefix)
{
	output_info += prefix + "Function prototype: " + currentNode->name + "\n";
	ASTNode_Prototype_Function* function_prototype = static_cast<ASTNode_Prototype_Function*> (currentNode);
	do_node(function_prototype->function_body);																//Do the body
	output_info += prefix + "\tFunction returns a type:" + toString(function_prototype->returnType->val_type) + "\n";
}

void GenInfoCodeGenerator::doValueNode(ASTNode* currentNode, std::string prefix)
{
	Value* current_value_node = dynamic_cast<Value*>(currentNode);
	output_info += prefix + "Value: This could get complicated\n";
	output_info += prefix + "\tValue type: " + toString(current_value_node->val_type) +"\n";
	if (current_value_node->val_type == Value::typ_call)
	{
		output_info += prefix + "\tFunction called's name: " + current_value_node->data.dat_call->function->name + "\n";
		do_node(current_value_node->data.dat_call, prefix+"\t");
	} else if (current_value_node->val_type == Value::typ_block)
	{
		output_info += prefix + "\tValue contained block\n";
		do_node(current_value_node->data.dat_block, prefix+"\t");
	}
}
