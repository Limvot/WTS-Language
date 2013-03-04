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
	if (tree->root->num_children)
	{
		for (unsigned int i = 0; i < tree->root->num_children; i++)
		{
			doNode(tree->root->children[i]);							//Go through each child of the root node
		}
	}
	std::cout << "Calling main!\n";
	doNode(new ASTNode_Call(tree->functions["main"]));	//Get the main function pointer, create a call to it, cast to node, then interpret it
	std::cout << "Done with main!\n";
}

std::string Interpreter::toString(int in)
{
	std::stringstream out_ss;
	out_ss << in;
	return out_ss.str();
}

Value* Interpreter::doNode(ASTNode* currentNode)				//The prefix is added to each line added to the output, so that we can have multiple indents through recursive functions
{
	switch (currentNode->type)
	{
		case ASTNode::call:
			return(doCallNode(currentNode)); 
			break;

		case ASTNode::variable:
			if (currentNode)
				return (static_cast<ASTNode_Variable*>(currentNode)->value);
			else
				//std::cout <<  "Why the heck is an empty node passed through with a type of variable?\n";
			break;

		case ASTNode::statement:
		{

			ASTNode_Statement* statementNode = dynamic_cast<ASTNode_Statement*> (currentNode);
				switch (statementNode->statementType)
				{
					case ASTNode_Statement::if_statement:
					{
						if (doNode(statementNode->condition)->data.datInt)
							doNode(statementNode->firstOption);
						break;
					}

					case ASTNode_Statement::while_statement:
						while(doNode(statementNode->condition)->data.datInt)
						{
							doNode(statementNode->firstOption);
						}
						break;

					default:
						break;
				}
			break;
		}

		case ASTNode::block:
			for (unsigned int i = 0; i < currentNode->children.size(); i++)
			{
				doNode(currentNode->children[i]);
			}
			break;

		case ASTNode::prototype:
			break;

		case ASTNode::prototype_function:
		{
			//std::cout <<  "Would interpret this prototype_function if we didn't just ignore them...\n";
			//Do nothing, is a prototype. Evaluate when called
			//ASTNode_Prototype_Function* function_prototype = static_cast<ASTNode_Prototype_Function*> (currentNode);
			//Do nothing, is a prototype. Evaluate when called
			break;
		}

		case ASTNode::prototype_object:
			break;

		case ASTNode::value:
		{
			Value* currentValueNode = dynamic_cast<Value*>(currentNode);
			switch (currentValueNode->valType)
			{
				case Value::typCall:
					return(doNode(currentValueNode->data.datCall));				//If a call, evaluate
					break;

				case Value::typStatement:
					return(doNode(currentValueNode->data.datStatement));
					break;

				case Value::typBlock:
					return(doNode(currentValueNode->data.datBlock));
					break;

				case Value::typInt:
					return(currentValueNode);									//If a int, return this one
					break;

				case Value::typVariable:
					return(doNode(currentValueNode->data.datVariable));			//If a variable, do it
					break;
			}
			break;
		}

		case ASTNode::basic:
			break;
	}
}

Value* Interpreter::doCallNode(ASTNode* currentNode)
{
	//std::cout <<  "Interpreting a call\n";

	ASTNode_Call* currentCallNode = static_cast<ASTNode_Call*>(currentNode);
	if (currentCallNode->function->funcType == ASTNode_Prototype_Function::func_builtin)
	{
		//std::cout <<  "Interpreting a builtin function\n";

		ASTNode_Prototype_Function_Builtin* currentBuiltinFunction = static_cast<ASTNode_Prototype_Function_Builtin*>(currentCallNode->function);
		if (currentBuiltinFunction->isBinary)																//Binary operator
		{
			//std::cout <<  "Interpreting a binary operator\n";
			Value* currentValueNode = NULL;
			Value* firstParamValue = NULL;
			Value* secondParamValue = NULL;

			if (currentBuiltinFunction->operatorType != ASTNode_Prototype_Function_Builtin::assignment)	//If not an assignment, do evaluate. If it is an assignment, we assign to the first param not evaluate it
			{
				//First Param
				currentValueNode = currentCallNode->parameters[0];
				if (currentValueNode)
				{
					firstParamValue = doNode(currentValueNode);
				}
				else
					std::cout <<  "NO FIRST PARAMETER!\n";
			}

			//Second Param
			currentValueNode = currentCallNode->parameters[1];
			if (currentValueNode)
			{
				secondParamValue = doNode(currentValueNode);
			}
			else
				std::cout <<  "NO SECOND PARAMETER!\n";

			//DO BUILTIN FUNCTIONS!!!!!
			switch(currentBuiltinFunction->operatorType)
			{
				case ASTNode_Prototype_Function_Builtin::assignment:
					//if (currentCallNode->parameters[0]->data.datVariable->value != NULL)
					//	delete currentCallNode->parameters[0]->data.datVariable->value;
					currentCallNode->parameters[0]->data.datVariable->value = secondParamValue;
					break;

				case ASTNode_Prototype_Function_Builtin::addition:
					return new Value(firstParamValue->data.datInt + secondParamValue->data.datInt);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::subtraction:
					return new Value(firstParamValue->data.datInt - secondParamValue->data.datInt);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::multiplication:
					return new Value(firstParamValue->data.datInt * secondParamValue->data.datInt);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::division:
					return new Value(firstParamValue->data.datInt / secondParamValue->data.datInt);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::exponent:
					return new Value(firstParamValue->data.datInt ^ secondParamValue->data.datInt);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::modulus:
					return new Value(firstParamValue->data.datInt % secondParamValue->data.datInt);	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::equal:
					return new Value(int(firstParamValue->data.datInt == secondParamValue->data.datInt));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::not_equal:
					return new Value(int(firstParamValue->data.datInt != secondParamValue->data.datInt));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::greater_than:
					return new Value(int(firstParamValue->data.datInt > secondParamValue->data.datInt));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::less_than:
					return new Value(int(firstParamValue->data.datInt < secondParamValue->data.datInt));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::greater_than_or_equal:
					return new Value(int(firstParamValue->data.datInt >= secondParamValue->data.datInt));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
				
				case ASTNode_Prototype_Function_Builtin::less_than_or_equal:
					return new Value(int(firstParamValue->data.datInt <=secondParamValue->data.datInt));	//THIS IS BAD! Not only do we not account for other data types, we just assume it is that data type! Could be wierd with the fact that data is a union....
						
				default:
					std::cout <<  "Hmm?! No builtin operator that matches!\n";
					break;
			}

		} else {																								//Unary operator!

			//std::cout <<  "Interpreting a unary operator\n";

			//Only param
			Value* currentValueNode = currentCallNode->parameters[0];
			Value* firstParamValue;
			if (currentValueNode)
			{
				firstParamValue = doNode(currentValueNode);
			}
			else
				std::cout <<  "NO FIRST PARAMETER FOR UNARY\n";

			//DO BUILTIN FUNCTIONS!!!!!!
			switch(currentBuiltinFunction->operatorType)
			{
				case ASTNode_Prototype_Function_Builtin::print:
				{
					std::string toPrint = toString(firstParamValue->data.datInt) + "\n";
					std::cout << toPrint;
					break;
				}

				default:
					std::cout <<  "Hmm?! No builtin operator that matches!\n";
					break;
			}
		}

	} else {																									//A user function!

		//std::cout <<  "Interperting a custom function named " << currentCallNode->function->name << std::endl;

		for (unsigned int i = 0; i < currentCallNode->parameters.size(); i++)
		{
			Value* currentValueNode = currentCallNode->parameters[i];
			if (currentValueNode)
			{
				//function's parameters are values that are variable prototypes, get the actual variable and set it's value to be the call's parameter's value.
				static_cast<ASTNode_Prototype_Variable*>(currentCallNode->function->parameters[i]->data.datPrototype)->variable->value = doNode(currentValueNode);
			}
			else
				std::cout <<  "BAD PARAMETER!!!!!\n";
		}
		//Do regular functions!
		//std::cout <<  "Interpreting the custom function's body\n";
		doNode(currentCallNode->function->function_body);
		if (currentCallNode->function->returnValue)
			return doNode(currentCallNode->function->returnValue);
	}
}
