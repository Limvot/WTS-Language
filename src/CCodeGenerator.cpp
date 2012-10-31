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

std::string CCodeGenerator::toString(int in)
{
	std::stringstream out_ss;
	out_ss << in;
	return out_ss.str();
}

std::string CCodeGenerator::getOutput()
{
	return output_c;
}


void CCodeGenerator::do_node(ASTNode* current_node, std::string prefix)				//The prefix is added to each line added to the output, so that we can have multiple indents through recursive functions
{
	switch (current_node->type)
	{
		case ASTNode::call:
		{
			ASTNode_Call* current_call_node = static_cast<ASTNode_Call*>(current_node);
			output_c += prefix + current_call_node->function->name + "(";
			for (unsigned int i = 0; i < current_call_node->parameters.size(); i++)
			{
				Value* current_value_node = current_call_node->parameters[i];
				if (current_value_node)
				{
					//output_c += prefix + "\tParameter " + toString(i) + ": type = " + toString(current_value_node->val_type) + "\n";
					if (current_value_node->val_type == Value::typ_call)
					{
						do_node(current_value_node->data.dat_call, "");
					} else
						output_c += prefix + current_value_node->name + ", ";
				}
				else
					//output_c += prefix + "\tParameter " + toString(i) + " is NULL.\n";
					output_c += prefix + "NULL";
				if (i < (current_call_node->parameters.size()-1))
					output_c += ")";
				else
					output_c += ", ";
			}
			break;
		}

		case ASTNode::variable:
			output_c += prefix + current_node->name + " ";
			break;

		case ASTNode::statement:
		{
			ASTNode_Statement* statement_node = dynamic_cast<ASTNode_Statement*> (current_node);
				switch (statement_node->statement_type)
				{
					case ASTNode_Statement::if_statement:
					{
						output_c += prefix + "if ( ";
						do_node(statement_node->condition, std::string(""));
						output_c += " )\n" + prefix + "{\n";
						do_node(statement_node->first_option, prefix+"\t");
						output_c += prefix + "}\n";
						break;
					}

					case ASTNode_Statement::while_statement:
						output_c += prefix + "while ( ";
						do_node(statement_node->condition, std::string(""));
						output_c += " )\n" + prefix + "{\n";
						do_node(statement_node->first_option, prefix+"\t");
						output_c += prefix + "}\n";
						break;

					default:
						break;
				}
			break;
		}

		case ASTNode::block:
			output_c += prefix + "{\n";
			for (unsigned int i = 0; i < current_node->children.size(); i++)
					do_node(current_node->children[i], prefix+"\t");
			output_c += prefix + "}\n";
			break;

		case ASTNode::prototype:
			output_c += prefix + "Plain prototype: Why are we here?\n";
			break;

		case ASTNode::prototype_function:
		{
			ASTNode_Prototype_Function* function_prototype = static_cast<ASTNode_Prototype_Function*> (current_node);
			output_c += prefix + toString(function_prototype->return_value->val_type) + " " + current_node->name + "()\n";

			unsigned int num_children = function_prototype->function_body->children.size();
			output_c += prefix + "{\n";
			for (unsigned int i = 0; i < num_children; i++)
			{
				do_node(function_prototype->function_body->children[i], prefix+"\t");
			}
			output_c += prefix + "\treturn 0;\n";
			output_c += prefix + "}\n";
			break;
		}

		case ASTNode::prototype_object:
			output_c += prefix + "Object prototype: Again, why are we here?\n";
			break;

		case ASTNode::value:
		{
			Value* current_value_node = dynamic_cast<Value*>(current_node);
			//output_c += prefix + "\tValue type: " + toString(current_value_node->val_type) +"\n";
			if (current_value_node->val_type == Value::typ_call)
			{
				output_c += prefix + current_value_node->data.dat_call->function->name + "(\n";
				do_node(current_value_node->data.dat_call, std::string(""));
				output_c += ")";

			}
			break;
		}

		case ASTNode::basic:
			output_c += prefix + "Basic node: Can we get here? I don't even know anymore...\n";
			break;
	}
}