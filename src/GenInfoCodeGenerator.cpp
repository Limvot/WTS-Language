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

	for(std::map<std::string, ASTNode_Variable*>::iterator it = tree->variables.begin(); it != tree->variables.end(); ++it)
	{
		std::cout << "variable: " << it->first <<  " value " << it->second << std::endl;
	}

	for(std::map<std::string, ASTNode_Prototype_Function*>::iterator it = tree->functions.begin(); it != tree->functions.end(); ++it)
	{
		std::cout << "function: " << it->first <<  " value " << it->second << std::endl;
	}

	if (tree->root->num_children)
	{
		output_info = output_info + "Root node has " + toString(tree->root->num_children) + " children.\n";
		std::cout << "Root node has " << tree->root->num_children << " children." << std::endl;
		for (unsigned int i = 0; i < tree->root->num_children; i++)
		{
			doNode(tree->root->children[i]);							//Go through each child of the root node
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

void GenInfoCodeGenerator::doNode(ASTNode* currentNode, std::string prefix)				//The prefix is added to each line added to the output, so that we can have multiple indents through recursive functions
{
	//std::cout << "doing node named " << currentNode->name << std::endl;
	switch (currentNode->type)
	{
		case ASTNode::call:
			doCallNode(currentNode, prefix);
			break;

		case ASTNode::prototype_variable:
			output_info += prefix + "variable:" + static_cast<ASTNode_Prototype_Variable*>(currentNode)->variable->name + "\n";
			break;

		case ASTNode::statement:
			doStatementNode(currentNode, prefix);
			break;

		case ASTNode::block:
			doBlockNode(currentNode, prefix);
			break;

		case ASTNode::prototype:
			output_info += prefix + "Plain prototype\n";
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
	ASTNode_Call* currentCallNode = static_cast<ASTNode_Call*>(currentNode);
	output_info += prefix + "call:" + currentCallNode->function->name + "\n";
	for (unsigned int i = 0; i < currentCallNode->parameters.size(); i++)
	{
		Value* currentValueNode = currentCallNode->parameters[i];
		if (currentValueNode)
		{
			output_info += prefix + "\tParameter " + toString(i) + ": type = " + toString(currentValueNode->valType) + "\n";
			if (currentValueNode->valType == Value::typCall)
				doNode(currentValueNode->data.datCall, prefix+"\t");
		}
		else
			output_info += prefix + "\tParameter " + toString(i) + " is NULL.\n";
	}
}

void GenInfoCodeGenerator::doStatementNode(ASTNode* currentNode, std::string prefix)
{
	ASTNode_Statement* statementNode = dynamic_cast<ASTNode_Statement*> (currentNode);
	output_info += prefix + "statement:" + currentNode->name + " Type:" + toString(statementNode->statementType) + ", ";
		switch (statementNode->statementType)
		{
			case ASTNode_Statement::if_statement:
			{
				output_info += "If statement\n";
				output_info += prefix + "\tCondition:\n";
				doNode(statementNode->condition, prefix+"\t\t");
				output_info += prefix + "\tExecutable Block:\n";
				doNode(statementNode->firstOption, prefix+"\t\t");
				break;
			}

			case ASTNode_Statement::while_statement:
				output_info += "While statement\n";
				output_info += prefix + "\tCondition:\n";
				doNode(statementNode->condition, prefix+"\t\t");
				output_info += prefix + "\tExecutable Block:\n";
				doNode(statementNode->firstOption, prefix+"\t\t");
				break;

			default:
				break;
	}
}

void GenInfoCodeGenerator::doBlockNode(ASTNode* currentNode, std::string prefix)
{
	output_info += prefix + "block: " + toString(currentNode->children.size()) + " children.\n";
				for (unsigned int i = 0; i < currentNode->children.size(); i++)
						doNode(currentNode->children[i], prefix+"\t");
}

void GenInfoCodeGenerator::doPrototypeFunctionNode(ASTNode* currentNode, std::string prefix)
{
	output_info += prefix + "Function prototype: " + currentNode->name + "\n";
	ASTNode_Prototype_Function* function_prototype = static_cast<ASTNode_Prototype_Function*> (currentNode);
	output_info += "\tParameters:\n";
	for (int i = 0; i < function_prototype->parameters.size(); i++) {
		doNode(function_prototype->parameters[i], prefix);
	}
	doNode(function_prototype->function_body, prefix);																//Do the body
	output_info += prefix + "\tFunction returns a type:" + toString(function_prototype->returnType->valType) + "\n";
}

void GenInfoCodeGenerator::doValueNode(ASTNode* currentNode, std::string prefix)
{
	Value* currentValueNode = dynamic_cast<Value*>(currentNode);
	output_info += prefix + "Value: This could get complicated\n";
	output_info += prefix + "\tValue type: " + toString(currentValueNode->valType) +"\n";
	if (currentValueNode->valType == Value::typPrototype)
	{
		output_info += prefix + "\tPrototype's name: " + currentValueNode->data.datPrototype->name + "\n";
		doNode(currentValueNode->data.datPrototype, prefix+"\t");
	} else if (currentValueNode->valType == Value::typCall)
	{
		output_info += prefix + "\tFunction called's name: " + currentValueNode->data.datCall->function->name + "\n";
		doNode(currentValueNode->data.datCall, prefix+"\t");
	} else if (currentValueNode->valType == Value::typBlock)
	{
		output_info += prefix + "\tValue contained block\n";
		doNode(currentValueNode->data.datBlock, prefix+"\t");
	}
}
