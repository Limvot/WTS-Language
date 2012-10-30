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
	if (tree->root.num_children)
	{
		for (unsigned int i = 0; i < tree->root.num_children; i++)
		{
			do_node(tree->root.children[i]);							//Go through each child of the root node
		}
	}
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

void GenInfoCodeGenerator::do_node(ASTNode* current_node, std::string prefix)				//The prefix is added to each line added to the output, so that we can have multiple indents through recursive functions
{
	switch (current_node->type)
	{
		case ASTNode::call:
		{
			ASTNode_Call* current_call_node = static_cast<ASTNode_Call*>(current_node);
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
			break;
		}

		case ASTNode::variable:
			output_info += prefix + "variable:" + current_node->name + "\n";
			break;

		case ASTNode::statement:
		{
			ASTNode_Statement* statement_node = dynamic_cast<ASTNode_Statement*> (current_node);
			output_info += prefix + "statement:" + current_node->name + " Type:" + toString(statement_node->statement_type) + ", ";
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
						output_info += prefix +"While statement\n";
						output_info += prefix + "\tCondition:\n";
						do_node(statement_node->condition, prefix+"\t\t");
						output_info += prefix + "\tExecutable Block:\n";
						do_node(statement_node->first_option, prefix+"\t\t");
						break;

					default:
						break;
				}
			break;
		}

		case ASTNode::block:
			output_info += prefix + "block: " + toString(current_node->children.size()) + " children.\n";
			for (unsigned int i = 0; i < current_node->children.size(); i++)
					do_node(current_node->children[i], prefix+"\t");
			break;

		case ASTNode::prototype:
			output_info += prefix + "Plain prototype: Again, why are we here?\n";
			break;

		case ASTNode::prototype_function:
		{
			output_info += prefix + "Function prototype: " + current_node->name + "\n";
			ASTNode_Prototype_Function* function_prototype = static_cast<ASTNode_Prototype_Function*> (current_node);
			unsigned int num_children = function_prototype->function_body->children.size();
			output_info += prefix + "\tFunction returns a type:" + toString(function_prototype->return_value->val_type) + " and has " + toString(num_children) + " children.\n";
			for (unsigned int i = 0; i < num_children; i++)
			{
				do_node(function_prototype->function_body->children[i], prefix+"\t");
			}
			break;
		}

		case ASTNode::prototype_object:
			output_info += prefix + "Object prototype: Again, why are we here?\n";
			break;

		case ASTNode::value:
		{
			Value* current_value_node = dynamic_cast<Value*>(current_node);
			output_info += prefix + "Value: This could get complicated\n";
			output_info += prefix + "\tValue type: " + toString(current_value_node->val_type) +"\n";
			if (current_value_node->val_type == Value::typ_call)
			{
				output_info += prefix + "\tFunction called's name: " + current_value_node->data.dat_call->function->name + "\n";
				do_node(current_value_node->data.dat_call, prefix+"\t");
			}
			break;
		}

		case ASTNode::basic:
			output_info += prefix + "Basic node: Can we get here? I don't even know anymore...\n";
			break;
	}
}
