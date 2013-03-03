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
	std::cout << "Root has " << tree->root->num_children << " children." << std::endl;
	if (tree->root->num_children)
	{
		std::cout << "Root has " << tree->root->num_children << " children." << std::endl;
		for (unsigned int i = 0; i < tree->root->num_children; i++)
		{
			doNode(tree->root->children[i]);							//Go through each child of the root node
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

std::string CCodeGenerator::doOperatorSymbol(ASTNode_Prototype_Function_Builtin::builtin_type type_in)
{
    switch (type_in)
    {
    	case ASTNode_Prototype_Function_Builtin:: none:
    		return std::string("none");
    		break;

    	case ASTNode_Prototype_Function_Builtin:: print:
    		return std::string("printf(\"%i\\n\", ");
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

    	case ASTNode_Prototype_Function_Builtin:: modulus:
    		return std::string("%");
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

std::string CCodeGenerator::doValueType(Value::value_type type_in)
{
	    switch (type_in)
    {
    	case Value:: typVoid:
    		return std::string("void");
    		break;

    	case Value:: typInt:
    		return std::string("int");
    		break;

    	case Value:: typUInt:
    		return std::string("unsigned int");
    		break;

    	case Value:: typFloat:
    		return std::string("float");
    		break;

    	case Value:: typDouble:
    		return std::string("double");
    		break;

    	case Value:: typBool:
    		return std::string("bool");
    		break;

    	case Value:: typChar:
    		return std::string("char");
    		break;

    	case Value:: typVariable:
    		return std::string("WAIT, WHAT? VARIABLE VARIABLE?");
    		break;

    	case Value:: typObject:
    		return std::string("OBJECT TYPE NOT IMPLEMENTED");
    		break;

    	case Value:: typFunction:
    		return std::string("FUNCTION OBJECTS NOT IMPLEMENTED");
    		break;

    	case Value:: typPrototype:
    		return std::string("WAIT, WHAT? PROTOTYPE VARIABLE?");
    		break;

    	case Value:: typCall:
    		return std::string("WAIT, WHAT? typCall VARIABLE?");
    		break;

    	default:
    		return std::string("COULD NOT FIND VARIABLE SYMBOL: THIS SHOUDN'T EVER HAPPEN!");
    }
}

void CCodeGenerator::doNode(ASTNode* currentNode, std::string prefix, Value* returnStatement)				//The prefix is added to each line added to the output, so that we can have multiple indents through recursive functions
{
	switch (currentNode->type)
	{
		case ASTNode::call:
			doCallNode(currentNode, prefix);
			break;

		case ASTNode::prototype_variable:
			doVariablePrototypeNode(currentNode, prefix);
			break;

		case ASTNode::statement:
			doStatementNode(currentNode, prefix);
			break;

		case ASTNode::block:
			doBlockNode(currentNode, prefix, returnStatement);
			break;

		case ASTNode::prototype:
			output_c += prefix + "Plain prototype: Why are we here?\n";
			break;

		case ASTNode::prototype_function:
			doPrototypeFunctionNode(currentNode, prefix);
			break;

		case ASTNode::prototype_object:
			output_c += prefix + "Object prototype: Again, why are we here?\n";
			break;

		case ASTNode::value:
			doValueNode(currentNode, prefix, returnStatement);
			break;

		case ASTNode::basic:
			output_c += prefix + "Basic node: Can we get here? I don't even know anymore...\n";
			break;
	}
}

void CCodeGenerator::doCallNode(ASTNode* currentNode, std::string prefix)
{
	ASTNode_Call* currentCallNode = static_cast<ASTNode_Call*>(currentNode);
	if (currentCallNode->function->funcType == ASTNode_Prototype_Function::func_builtin)
	{
		ASTNode_Prototype_Function_Builtin* currentBuiltinFunction = static_cast<ASTNode_Prototype_Function_Builtin*>(currentCallNode->function);
		if (currentBuiltinFunction->isBinary)																//Binary operator
		{
			if (currentBuiltinFunction->operatorType != ASTNode_Prototype_Function_Builtin::assignment)
				output_c += "( ";
			else
				output_c += prefix;																				//Assignment, so use the prefix
			//First Param
			Value* currentValueNode = currentCallNode->parameters[0];
			if (currentValueNode)
				doNode(currentValueNode, "");
			else
				output_c += prefix + "NULL";

			output_c += " " + doOperatorSymbol(currentBuiltinFunction->operatorType) + " ";

			//Second Param
			currentValueNode = currentCallNode->parameters[1];
			if (currentValueNode)
				doNode(currentValueNode, "");
			else
				output_c += prefix + "NULL";

			if (currentBuiltinFunction->operatorType != ASTNode_Prototype_Function_Builtin::assignment)
				output_c += ")";

		} else {																								//Unary operator!

			output_c += prefix + doOperatorSymbol(currentBuiltinFunction->operatorType) + "( ";

			//Only param
			Value* currentValueNode = currentCallNode->parameters[0];
			if (currentValueNode)
				doNode(currentValueNode, "");
			else
				output_c += prefix + "NULL";

			output_c += "))";//BAD HACK BAD HACK TAKE OFF THE LAST PARENTHESE SO PRINT COMPILES WITH MORE HACKS TAKE IT OFF!
		}

	} else {																									//If not a built in function, i.e. a regular one

		output_c += prefix + currentCallNode->function->name + "(";
		for (unsigned int i = 0; i < currentCallNode->parameters.size(); i++)
		{
			Value* currentValueNode = currentCallNode->parameters[i];

			if (currentValueNode)
				doNode(currentValueNode, "");
			else
				output_c += prefix + "NULL";

			if (i < currentCallNode->parameters.size() - 1)					//If this is not the last parameter, add a comma space
			{
				output_c += ", ";
			}
		}
		output_c += ")";
	}
}

void CCodeGenerator::doVariablePrototypeNode(ASTNode* currentNode, std::string prefix) {
	ASTNode_Variable* variable = static_cast<ASTNode_Prototype_Variable*>(currentNode)->variable;
	output_c += prefix + doValueType(variable->value->valType) + " " + variable->name;
}

void CCodeGenerator::doStatementNode(ASTNode* currentNode, std::string prefix) {

	ASTNode_Statement* statementNode = dynamic_cast<ASTNode_Statement*>(currentNode);
	switch (statementNode->statementType)
	{
		case ASTNode_Statement::if_statement:
		{
			output_c += prefix + "if ( ";
			doNode(statementNode->condition, std::string(""));
			output_c += " )\n";
			doNode(statementNode->firstOption, prefix+"\t");
			break;
		}

		case ASTNode_Statement::while_statement:
			output_c += prefix + "while ( ";
			doNode(statementNode->condition, std::string(""));
			output_c += " )\n";
			doNode(statementNode->firstOption, prefix+"\t");
			break;

		default:
			break;
	}
}

void CCodeGenerator::doBlockNode(ASTNode* currentNode, std::string prefix, Value* returnStatement) {

	output_c += prefix + "{\n";
	for (unsigned int i = 0; i < currentNode->children.size(); i++)
	{
		doNode(currentNode->children[i], prefix+"\t");
		output_c += ";\n";
	}
	if (returnStatement) {
		output_c += prefix + "\t" + "return ";
		doNode(returnStatement);
		output_c += ";\n" + prefix + "}\n";
	}
	else
		output_c += prefix + "}\n";
}

void CCodeGenerator::doPrototypeFunctionNode(ASTNode* currentNode, std::string prefix) {

	ASTNode_Prototype_Function* function_prototype = static_cast<ASTNode_Prototype_Function*> (currentNode);
	output_c += prefix + doValueType(function_prototype->returnType->valType) + " " + currentNode->name + "(";
	for (int i = 0; i < function_prototype->parameters.size(); i++) {
		doNode(function_prototype->parameters[i]);
		if (i < function_prototype->parameters.size() - 1)					//If this is not the last parameter, add a comma space
		{
			output_c += ", ";
		}
	}
	output_c += ")\n";
	doNode(function_prototype->function_body, prefix, function_prototype->returnValue);
}

void CCodeGenerator::doValueNode(ASTNode* currentNode, std::string prefix, Value* returnStatement) {

	Value* currentValueNode = dynamic_cast<Value*>(currentNode);
	//output_c += prefix + "\tValue type: " + toString(currentValueNode->valType) +"\n";
	switch (currentValueNode->valType)
	{
		case Value::typPrototype:
			doNode(currentValueNode->data.datPrototype, prefix);
			break;

		case Value::typCall:
			doNode(currentValueNode->data.datCall, prefix);
			break;

		case Value::typBlock:
			doNode(currentValueNode->data.datBlock, prefix, returnStatement);
			break;

		case Value::typInt:
			output_c += toString(currentValueNode->data.datInt);
			break;

		case Value::typVariable:
			output_c += currentValueNode->data.datVariable->name;
			break;
	}
}
