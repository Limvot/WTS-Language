#include "Interpreter.h"


Interpreter::Interpreter()
{
    //ctor
}

Interpreter::~Interpreter()
{
    //dtor
}

void Interpreter::interpret(AbstractSyntaxTree* tree)
{
	if (tree->root.num_children)
	{
		for (unsigned int i = 0; i < tree->root.num_children; i++)
		{
			do_node(tree->root.children[i]);							//Go through each child of the root node
		}
	}
	std::cout << "Calling main!\n";
	do_node(new ASTNode_Call(tree->functions["main"]));	//Get the main function pointer, create a call to it, cast to node, then interpret it
	std::cout << "Done with main!\n";
}

std::string Interpreter::toString(int in)
{
	std::stringstream out_ss;
	out_ss << in;
	return out_ss.str();
}

Value* Interpreter::do_node(ASTNode* current_node)				//The prefix is added to each line added to the output, so that we can have multiple indents through recursive functions
{
	switch (current_node->type)
	{
		case ASTNode::call:
		{
			std::cout << "Interpreting a call\n";

			ASTNode_Call* current_call_node = static_cast<ASTNode_Call*>(current_node);
			if (current_call_node->function->func_type == ASTNode_Prototype_Function::func_builtin)
			{
				std::cout << "Interpreting a builtin function\n";

				ASTNode_Prototype_Function_Builtin* current_builtin_function = static_cast<ASTNode_Prototype_Function_Builtin*>(current_call_node->function);
				if (current_builtin_function->is_binary)																//Binary operator
				{
					std::cout << "Interpreting a binary operator\n";
					Value* current_value_node = NULL;
					Value* first_param_value = NULL;
					Value* second_param_value = NULL;

					if (current_builtin_function->operator_type != ASTNode_Prototype_Function_Builtin::assignment)	//If not an assignment, do evaluate. If it is an assignment, we assign to the first param not evaluate it
					{
						//First Param
						current_value_node = current_call_node->parameters[0];
						if (current_value_node)
						{
							first_param_value = do_node(current_value_node);
						}
						else
							std::cout << "NO FIRST PARAMETER!\n";
					}

					//Second Param
					current_value_node = current_call_node->parameters[1];
					if (current_value_node)
					{
						second_param_value = do_node(current_value_node);
					}
					else
						std::cout << "NO SECOND PARAMETER!\n";

					//DO BUILTIN FUNCTIONS!!!!!
					switch(current_builtin_function->operator_type)
					{
						case ASTNode_Prototype_Function_Builtin::assignment:
							current_call_node->parameters[0]->data.dat_variable->value = second_param_value;
							break;

						case ASTNode_Prototype_Function_Builtin::addition:
							return new Value(first_param_value->data.dat_int + second_param_value->data.dat_int);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::subtraction:
							return new Value(first_param_value->data.dat_int - second_param_value->data.dat_int);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::multiplication:
							return new Value(first_param_value->data.dat_int * second_param_value->data.dat_int);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::division:
							return new Value(first_param_value->data.dat_int / second_param_value->data.dat_int);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::exponent:
							return new Value(first_param_value->data.dat_int ^ second_param_value->data.dat_int);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::equal:
							return new Value(int(first_param_value->data.dat_int == second_param_value->data.dat_int));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::not_equal:
							return new Value(int(first_param_value->data.dat_int != second_param_value->data.dat_int));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::greater_than:
							return new Value(int(first_param_value->data.dat_int > second_param_value->data.dat_int));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::less_than:
							return new Value(int(first_param_value->data.dat_int < second_param_value->data.dat_int));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::greater_than_or_equal:
							return new Value(int(first_param_value->data.dat_int >= second_param_value->data.dat_int));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						case ASTNode_Prototype_Function_Builtin::less_than_or_equal:
							return new Value(int(first_param_value->data.dat_int <=second_param_value->data.dat_int));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
						default:
							std::cout << "Hmm?! No builtin operator that matches!\n";
					}

				} else {																								//Unary operator!

					std::cout << "Interpreting a unary operator\n";

					//Only param
					Value* current_value_node = current_call_node->parameters[0];
					Value* first_param_value;
					if (current_value_node)
					{
						first_param_value = do_node(current_value_node);
					}
					else
						std::cout << "NO FIRST PARAMETER FOR UNARY\n";

					//DO BUILTIN FUNCTIONS!!!!!!
					switch(current_builtin_function->operator_type)
					{
						case ASTNode_Prototype_Function_Builtin::print:
							std::cout << first_param_value->data.dat_int << "\n";
							break;

						default:
							std::cout << "Hmm?! No builtin operator that matches!\n";
					}
				}

			} else {																									//A user function!

				std::cout << "Interpretign a custom function\n";

				for (unsigned int i = 0; i < current_call_node->parameters.size(); i++)
				{
					Value* current_value_node = current_call_node->parameters[i];
					if (current_value_node)
					{
						//CUSTOM FUNCTIONS DON'T ALLOW PARAMETERS YET
						do_node(current_value_node);
						//CUSTOM FUNCTIONS DON'T ALLOW PARAMETERS YET
					}
					else
						std::cout << "BAD PARAMETER!!!!!\n";
				}
				//Do regular functions!
				std::cout << "Interpreting the custom function's body\n";
				do_node(current_call_node->function->function_body);
			}
			break;
		}

		case ASTNode::variable:
			std::cout << "Interpreting a variable\n";
			if (current_node)
				return (static_cast<ASTNode_Variable*>(current_node)->value);
			else
				std::cout << "Why the heck is an empty node passed through with a type of variable?\n";
			break;

		case ASTNode::statement:
		{
			std::cout << "Interpreting a statement:";

			ASTNode_Statement* statement_node = dynamic_cast<ASTNode_Statement*> (current_node);
				switch (statement_node->statement_type)
				{
					case ASTNode_Statement::if_statement:
					{
						std::cout << "If statement\n";
						if (do_node(statement_node->condition)->data.dat_int)
							do_node(statement_node->first_option);
						break;
					}

					case ASTNode_Statement::while_statement:
						std::cout << "While statement\n";
						while(do_node(statement_node->condition)->data.dat_int)
						{
							std::cout << "About to do the value\n";
							std::cout << "The value for the while loop is:" << do_node(statement_node->condition)->data.dat_int << "\n";
							do_node(statement_node->first_option);
						}
						break;

					default:
							std::cout << "WHY THE HECK IS THIS STATEMENT NOT AN IF OR WHILE?\n";
						break;
				}
			break;
		}

		case ASTNode::block:
			std::cout << "Interpreting a block\n";
			for (unsigned int i = 0; i < current_node->children.size(); i++)
			{
				do_node(current_node->children[i]);
			}
			break;

		case ASTNode::prototype:
			std::cout << "Plain prototype: Why are we here?\n";
			break;

		case ASTNode::prototype_function:
		{
			std::cout << "Would interpret this prototype_function if we didn't just ignore them...\n";
			//Do nothing, is a prototype. Evaluate when called
			//ASTNode_Prototype_Function* function_prototype = static_cast<ASTNode_Prototype_Function*> (current_node);
			//Do nothing, is a prototype. Evaluate when called
			break;
		}

		case ASTNode::prototype_object:
			std::cout << "Object prototype: Again, why are we here?\n";
			break;

		case ASTNode::value:
		{
			std::cout << "Interpreting a value:";
			Value* current_value_node = dynamic_cast<Value*>(current_node);
			switch (current_value_node->val_type)
			{
				case Value::typ_call:
					std::cout << "is a call\n";
					return(do_node(current_value_node->data.dat_call));	//If a call, evaluate
					break;

				case Value::typ_int:
					std::cout << "is a int\n";
					return(current_value_node);									//If a int, return this one
					break;

				case Value::typ_variable:
					std::cout << "is a variable\n";
					return(do_node(current_value_node->data.dat_variable));	//If a variable, do it
					break;
			}
			break;
		}

		case ASTNode::basic:
			std::cout << "Basic node: Can we get here? I don't even know anymore...\n";
			break;
	}
}

