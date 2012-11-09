#include "Interpreter.h"


Interpreter::Interpreter()
{
    //ctor
}

Interpreter::~Interpreter()
{
    //dtor
}

void Interpreter::generate(AbstractSyntaxTree* tree)
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

std::string Interpreter::toString(int in)
{
	std::stringstream out_ss;
	out_ss << in;
	return out_ss.str();
}

std::string Interpreter::getOutput()
{
	return output_c;
}

std::string Interpreter::doOperatorSymbol(ASTNode_Prototype_Function_Builtin::builtin_type type_in)
{
    switch (type_in)
    {
    	case ASTNode_Prototype_Function_Builtin:: none:
    		return std::string("none");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: print:
    		return std::string("printf(\"%i\", ");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: assignment:
    		return std::string("=");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: addition:
    		return std::string("+");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: subtraction:
    		return std::string("-");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: multiplication:
    		return std::string("*");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: division:
    		return std::string("/");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: exponent:
    		return std::string("^");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: equal:
    		return std::string("==");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: not_equal:
    		return std::string("!=");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: greater_than:
    		return std::string(">");
    		break;

       	case ASTNode_Prototype_Function_Builtin:: less_than:
    		return std::string("<");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: greater_than_or_equal:
    		return std::string(">=");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: less_than_or_equal:
    		return std::string("<=");
    		break;

    	default:
    		return std::string("COULD NOT FIND BUILTIN SYMBOL: THIS SHOUDN'T EVER HAPPEN!");
    }
}

std::string Interpreter::doValueType(Value::value_type type_in)
{
	    switch (type_in)
    {
    	case Value:: typ_void:
    		return std::string("void");
    		break;

    	case Value:: typ_int:
    		return std::string("int");
    		break;

    	case Value:: typ_uint:
    		return std::string("unsigned int");
    		break;

    	case Value:: typ_float:
    		return std::string("float");
    		break;

    	case Value:: typ_double:
    		return std::string("double");
    		break;

    	case Value:: typ_bool:
    		return std::string("bool");
    		break;

    	case Value:: typ_char:
    		return std::string("char");
    		break;

    	case Value:: typ_variable:
    		return std::string("WAIT, WHAT? VARIABLE VARIABLE?");
    		break;

    	case Value:: typ_object:
    		return std::string("OBJECT TYPE NOT IMPLEMENTED");
    		break;

    	case Value:: typ_function:
    		return std::string("FUNCTION OBJECTS NOT IMPLEMENTED");
    		break;

    	case Value:: typ_call:
    		return std::string("WAIT, WHAT? TYP_CALL VARIABLE?");
    		break;

    	default:
    		return std::string("COULD NOT FIND VARIABLE SYMBOL: THIS SHOUDN'T EVER HAPPEN!");
    }
}

void Interpreter::do_node(ASTNode* current_node, std::string prefix, std::string ending_statement)				//The prefix is added to each line added to the output, so that we can have multiple indents through recursive functions
{
	switch (current_node->type)
	{
		case ASTNode::call:
		{
			ASTNode_Call* current_call_node = static_cast<ASTNode_Call*>(current_node);
			if (current_call_node->function->func_type == ASTNode_Prototype_Function::func_builtin)
			{
				std::cout << "Builtin operator prefix:" << prefix <<"||\n";

				ASTNode_Prototype_Function_Builtin* current_builtin_function = static_cast<ASTNode_Prototype_Function_Builtin*>(current_call_node->function);
				if (current_builtin_function->is_binary)																//Binary operator
				{
					if (current_builtin_function->operator_type != ASTNode_Prototype_Function_Builtin::assignment)
						output_c += "( ";
					else
						output_c += prefix;																				//Assignment, so use the prefix
					//First Param
					Value* current_value_node = current_call_node->parameters[0];
					if (current_value_node)
					{
						do_node(current_value_node, "");
					}
					else
						output_c += prefix + "NULL";

					output_c += " " + doOperatorSymbol(current_builtin_function->operator_type) + " ";

					//Second Param
					current_value_node = current_call_node->parameters[1];
					if (current_value_node)
					{
						do_node(current_value_node, "");
					}
					else
						output_c += prefix + "NULL";

					if (current_builtin_function->operator_type != ASTNode_Prototype_Function_Builtin::assignment)
						output_c += ")";
				} else {																								//Unary operator!
					output_c += prefix + doOperatorSymbol(current_builtin_function->operator_type) + "( ";

					//Only param
					Value* current_value_node = current_call_node->parameters[0];
					if (current_value_node)
					{
						do_node(current_value_node, "");
					}
					else
						output_c += prefix + "NULL";
					output_c += "))";//BAD HACK BAD HACK TAKE OFF THE LAST PARENTHESE SO PRINT COMPILES WITH MORE HACKS TAKE IT OFF!
				}

			} else {

				output_c += prefix + current_call_node->function->name + "(";
				for (unsigned int i = 0; i < current_call_node->parameters.size(); i++)
				{
					Value* current_value_node = current_call_node->parameters[i];
					if (current_value_node)
					{
						//output_c += prefix + "\tParameter " + toString(i) + ": type = " + toString(current_value_node->val_type) + "\n";
						do_node(current_value_node, "");
					}
					else
						//output_c += prefix + "\tParameter " + toString(i) + " is NULL.\n";
						output_c += prefix + "NULL";
					output_c += ", ";
				}
				output_c += ")";
			}
			break;
		}

		case ASTNode::variable:
			output_c += prefix + doValueType(static_cast<ASTNode_Variable*>(current_node)->value->val_type) + " " + current_node->name;
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
						output_c += " )\n";
						do_node(statement_node->first_option, prefix+"\t");
						break;
					}

					case ASTNode_Statement::while_statement:
						output_c += prefix + "while ( ";
						do_node(statement_node->condition, std::string(""));
						output_c += " )\n";
						do_node(statement_node->first_option, prefix+"\t");
						break;

					default:
						break;
				}
			break;
		}

		case ASTNode::block:
			output_c += prefix + "{\n";
			for (unsigned int i = 0; i < current_node->children.size(); i++)
			{
				do_node(current_node->children[i], prefix+"\t");
				output_c += ";\n";
			}
			if (ending_statement == "")
				output_c += prefix + "\t" + ending_statement + "\n" + prefix + "}\n";
			else
				output_c += prefix + "}\n";
			break;

		case ASTNode::prototype:
			output_c += prefix + "Plain prototype: Why are we here?\n";
			break;

		case ASTNode::prototype_function:
		{
			ASTNode_Prototype_Function* function_prototype = static_cast<ASTNode_Prototype_Function*> (current_node);
			//output_c += prefix + toString(function_prototype->return_value->val_type) + " " + current_node->name + "()\n";
			output_c += prefix + doValueType(function_prototype->return_value->val_type) + " " + current_node->name + "()\n";
			do_node(function_prototype->function_body, prefix, "return 0;");
			break;
		}

		case ASTNode::prototype_object:
			output_c += prefix + "Object prototype: Again, why are we here?\n";
			break;

		case ASTNode::value:
		{
			Value* current_value_node = dynamic_cast<Value*>(current_node);
			//output_c += prefix + "\tValue type: " + toString(current_value_node->val_type) +"\n";
			switch (current_value_node->val_type)
			{
				case Value::typ_call:

					do_node(current_value_node->data.dat_call, prefix);
					break;

				case Value::typ_int:
					output_c += toString(current_value_node->data.dat_int);
					break;

				case Value::typ_variable:
					output_c += current_value_node->data.dat_variable->name;
					break;
			}
			break;
		}

		case ASTNode::basic:
			output_c += prefix + "Basic node: Can we get here? I don't even know anymore...\n";
			break;
	}
}