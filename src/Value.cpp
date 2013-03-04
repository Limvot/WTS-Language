#include "Value.h"

Value::Value()
{
    type = value;
    name = "value";
    valType = typVoid;
}

Value::Value(ASTNode* incomingParent)
{
    type = value;
    name = "value_node";
    valType = typVoid;
    parent = incomingParent;
}

Value::Value(void* inVoidPtr)
{
    type = value;
    name = "value_void_ptr";
    valType = typVoid;
    data.datVoidPtr = inVoidPtr;
}

Value::Value(int inInt)
{
    type = value;
    name = "value_int";
    valType = typInt;
    data.datInt = inInt;
}

Value::Value(unsigned int inUInt)
{
    type = value;
    name = "value_uint";
    valType = typUInt;
    data.datUInt = inUInt;
}

Value::Value(float inFloat)
{
    type = value;
    name = "value_float";
    valType = typFloat;
    data.datFloat = inFloat;
}

Value::Value(double inDouble)
{
    type = value;
    name = "value_double";
    valType = typDouble;
    data.datDouble = inDouble;
}

Value::Value(bool inBool)
{
    type = value;
    name = "value_bool";
    valType = typBool;
    data.datBool = inBool;
}

Value::Value(char inChar)
{
    type = value;
    name = "value_char";
    valType = typChar;
    data.datChar = inChar;
}

Value::Value(ASTNode_Variable* inVariable)
{
    type = value;
    name = "value_variable";
    valType = typVariable;
    data.datVariable = inVariable;
}

Value::Value(ASTNode_Prototype* inPrototype) {
    type = value;
    name = "value_prototype";
    valType = typPrototype;
    data.datPrototype = inPrototype;
}

Value::Value(ASTNode_Statement* inStatement) {
    type = value;
    name = "value_statement";
    valType = typStatement;
    data.datStatement = inStatement;
}

Value::Value(ASTNode_Call* inCall)
{
    type = value;
    name = "value_call";
    valType = typCall;
    data.datCall = inCall;
}

Value::Value(Block* inBlock)
{
    type = value;
    name = "value_block";
    valType = typBlock;
    data.datBlock = inBlock;
}

Value::~Value()
{
	switch (valType)
	{
		case typVoid:
			break;

        case typVariable:
            if (data.datVariable != NULL)
            {
                delete data.datVariable;
                data.datVariable = NULL;
            }
            break;
		
    	case typObject:
			//if (data.datVoidPtr != NULL)
				//BAD BAD BAD;
			break;

		case typFunction:
			//if (data.datVoidPtr != NULL)
				//BAD BAD BAD;
			break;

        case typPrototype:
            if (data.datPrototype != NULL)
            {
                delete data.datPrototype;
                data.datPrototype = NULL;
            }
            break;

		case typCall:
			if (data.datCall != NULL)
            {
				delete data.datCall;
                data.datCall = NULL;
            }
			break;

        case typBlock:
            if (data.datBlock != NULL)
            {
                delete data.datBlock;
                data.datBlock = NULL;
            }
            break;


	}
    //dtor
}

Value::value_type Value::isNumber(std::string inputString)
{
    if (atoi(inputString.c_str()))
    {
        return typInt;
    } else {
        if (atof(inputString.c_str()))
        {
            return typDouble;
        }
    }
    return typVoid;
}

bool Value::makeNumber(std::string inputString)
{
    if (atoi(inputString.c_str()))
    {
        valType = typInt;
        data.datInt = atoi(inputString.c_str());
        return true;
    } else {
        if (atof(inputString.c_str()))
        {
            valType = typDouble;
            data.datDouble = atof(inputString.c_str());
            return true;
        } else {
            if (inputString == "0")                        //atoi returns 0 if not valid conversion, but what if the number is 0? Here we check for it
            {
                valType = typInt;
                data.datInt = 0;
                return true;
            }
        }
    }
    return false;
}
