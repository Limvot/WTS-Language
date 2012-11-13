#include "Value.h"

Value::Value()
{
    type = value;
    val_type = typ_void;
}

Value::Value(void* in_void_ptr)
{
    type = value;
    val_type = typ_void;
    data.dat_void_ptr = in_void_ptr;
}

Value::Value(int in_int)
{
    type = value;
    val_type = typ_int;
    data.dat_int = in_int;
}

Value::Value(unsigned int in_uint)
{
    type = value;
    val_type = typ_uint;
    data.dat_uint = in_uint;
}

Value::Value(float in_float)
{
    type = value;
    val_type = typ_float;
    data.dat_float = in_float;
}

Value::Value(double in_double)
{
    type = value;
    val_type = typ_double;
    data.dat_double = in_double;
}

Value::Value(bool in_bool)
{
    type = value;
    val_type = typ_bool;
    data.dat_bool = in_bool;
}

Value::Value(char in_char)
{
    type = value;
    val_type = typ_char;
    data.dat_char = in_char;
}

Value::Value(ASTNode_Variable* in_variable)
{
    type = value;
    val_type = typ_variable;
    data.dat_variable = in_variable;
}

Value::Value(ASTNode_Call* in_call)
{
    type = value;
    val_type = typ_call;
    data.dat_call = in_call;
}

Value::~Value()
{
	switch (val_type)
	{
		case typ_void:
			break;

        case typ_variable:
            if (data.dat_variable != NULL)
            {
                delete data.dat_variable;
                data.dat_variable = NULL;
            }
            break;
		
    	case typ_object:
			//if (data.dat_void_ptr != NULL)
				//BAD BAD BAD;
			break;

		case typ_function:
			//if (data.dat_void_ptr != NULL)
				//BAD BAD BAD;
			break;

		case typ_call:
			if (data.dat_call != NULL)
            {
				delete data.dat_call;
                data.dat_variable = NULL;
            }
			break;

	}
    //dtor
}

Value::value_type Value::isNumber(std::string input_string)
{
    if (atoi(input_string.c_str()))
    {
        return typ_int;
    } else {
        if (atof(input_string.c_str()))
        {
            return typ_double;
        }
    }
    return typ_void;
}

bool Value::makeNumber(std::string input_string)
{
    if (atoi(input_string.c_str()))
    {
        val_type = typ_int;
        data.dat_int = atoi(input_string.c_str());
        return true;
    } else {
        if (atof(input_string.c_str()))
        {
            val_type = typ_double;
            data.dat_double = atof(input_string.c_str());
            return true;
        } else {
            if (input_string == "0")                        //atoi returns 0 if not valid conversion, but what if the number is 0? Here we check for it
            {
                val_type = typ_int;
                data.dat_int = 0;
                return true;
            }
        }
    }
    return false;
}
