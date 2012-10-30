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

void GenInfoCodeGenerator::do_node(ASTNode* current_node, std::string preface)				//The preface is added to each line added to the output, so that we can have multiple indents through recursive functions
{
	switch (current_node->type)
	{
		case ASTNode::call:
		{
			ASTNode_Call* current_call_node = static_cast<ASTNode_Call*>(current_node);
			output_info += preface + "call:" + current_call_node->function->name + "\n";
			for (unsigned int i = 0; i < current_call_node->parameters.size(); i++)
			{
				Value* current_value_node = current_call_node->parameters[i];
				output_info += preface + "\tParameter " + toString(i) + ": type = " + toString(current_value_node->val_type) + "\n";
			}
			break;
		}

		case ASTNode::variable:
			output_info += preface + "variable:" + current_node->name + "\n";
			break;

		case ASTNode::statement:
		{
			ASTNode_Statement* statement_node = dynamic_cast<ASTNode_Statement*> (current_node);
			output_info += preface + "statement:" + current_node->name + "\n";
				switch (statement_node->statement_type)
				{
					case ASTNode_Statement::if_statement:
					{
						output_info += preface +"If statement\n";
						output_info += preface + "\tCondition: ";
						do_node(statement_node->condition, preface+"\t");
						output_info += preface + "\tExecutable Block: ";
						do_node(statement_node->first_option, preface+"\t");
						break;
					}

					case ASTNode_Statement::while_statement:
						output_info += preface +"While statement\n";
						output_info += preface + "\tCondition: ";
						do_node(statement_node->condition, preface+"\t");
						output_info += preface + "\tExecutable Block: ";
						do_node(statement_node->first_option, preface+"\t");
						break;

					default:
						break;
				}
			break;
		}

		case ASTNode::block:
			output_info += preface + "block: Why are we here?\n";
			break;

		case ASTNode::prototype:
			output_info += preface + "Plain prototype: Again, why are we here?\n";
			break;

		case ASTNode::prototype_function:
		{
			output_info += preface + "Function prototype: " + current_node->name + "\n";
			ASTNode_Prototype_Function* function_prototype = static_cast<ASTNode_Prototype_Function*> (current_node);
			unsigned int num_children = function_prototype->function_body->children.size();
			output_info += preface + "\tFunction returns a type:" + toString(function_prototype->return_value->val_type) + " and has " + toString(num_children) + " children.\n";
			for (unsigned int i = 0; i < num_children; i++)
			{
				do_node(function_prototype->function_body->children[i], preface+"\t");
			}
			break;
		}

		case ASTNode::prototype_object:
			output_info += preface + "Object prototype: Again, why are we here?\n";
			break;

		case ASTNode::value:
		{
			Value* current_value_node = dynamic_cast<Value*>(current_node);
			output_info += preface + "Value: This could get complicated\n";
			output_info += preface + "\tValue type: " + toString(current_value_node->val_type) +"\n";
			if (current_value_node->val_type == Value::typ_call)
			{
				output_info += preface + "\tFunction called's name: " + current_value_node->data.dat_call->function->name + "\n";
			}
			break;
		}

		case ASTNode::basic:
			output_info += preface + "Basic node: Can we get here? I don't even know anymore...\n";
			break;
	}
}
