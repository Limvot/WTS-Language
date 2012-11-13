#include "WTS_Parser.h"

WTS_Parser::WTS_Parser()
{
    reached_end = false;
    initialize_map();                                               //Setup out map of keywords
    initialize_builtin_functions();                                 //Add inbuilt functions to the AST (+-*/^print, etc)
}

WTS_Parser::~WTS_Parser()
{
    //dtor
}

int WTS_Parser::set_string(std::string input_string)
{
    reached_end = false;
    reader.set_string(input_string);
    return 0;
}

int WTS_Parser::parse()
{
    //Do the parse work!
    //The standard opening
    cpp_output = "#include <iostream>\n";

    std::string current_word;
    while (!reached_end)
    {
        current_word = reader.word(false);                          //Do not truncate ending char
        std::cout << "Current WORD/LINE =||" << current_word << "||end of word\n";
        if (current_word == "")                                     //Reached end-of-file
        {
            reached_end = true;
        } else if (current_word[0] == '#')                          //If comment
        {
            if (current_word[current_word.length()-1] == '\n')      //If a one word comment (line ending right after word), just make a comment with the one word
                cpp_output.append("//" + current_word);
            else                                                    //If multiple word comment, add the rest of the line (which has newline in it, if we don't truncate)
                cpp_output.append("//" + current_word + reader.line(false));

        } else {
            Value* return_value = do_token(reader.truncate_end(current_word));            //Evaluate the token, which is the word without the ending char
            if (return_value)
            {
                tree.current_node->addChild(return_value);
                std::cout << "Value did exist, adding value to current_node!\n";
            }
            std::cout << "Value in top loop did not exist.\n";
        }
    }

    std::cout << "Done parsing!\n";

    return 0;
}

std::string WTS_Parser::getCPP()
{
    return cpp_output;
}

AbstractSyntaxTree* WTS_Parser::getTree()
{
    return &tree;
}


Value* WTS_Parser::do_token(std::string token)
{
    //Look up the word in the keywordmap, call assoicated function pointer with argument token
    Value*(WTS_Parser::* function_pointer)(std::string) = wts_KeyWordsMap[token];
    if (function_pointer != 0)
    {
        Value* dotok_return_value = (this->*function_pointer)(token);
        std::cout << "This is what is returned from the function_pointer: " << dotok_return_value << "\n";
        return dotok_return_value;
    }
    else
    {
        ASTNode_Variable* variable = tree.variables[token];        //Look up variables in the AST's map, if exists, get the pointer, put it in a Value, and return its pointer
        if (variable)
        {
            cpp_output += token;
            return new Value(variable); // 
        }
        Value* number_value = new Value;
        if (number_value->makeNumber(token))                        //If the conversion to a number works, return the value with the number.
        {
            cpp_output += token;
            return number_value;
        }
        delete number_value;                                        //If not, delete and clean up
        std::cout << "Unknown token: " << token << "\n";
        //throw new SyntaxErrorException;
        return NULL;
    }
}


Value* WTS_Parser::wts_begin_function(std::string token)
{
    std::string return_type = reader.word();
    std::string function_name = reader.word();

    cpp_output += "int " + function_name + "() {\n";

    ASTNode_Prototype_Function* new_function = new ASTNode_Prototype_Function;      //Create a new function
    new_function->return_value->val_type = getValueType(return_type);
    new_function->name = function_name;                                             //Name the function the supplied name
    tree.functions[function_name] = new_function;                                   //Add the function to our function map, with the name as the key
    tree.current_node->addChild(new_function);                                      //Add the new function node to the current node
    new_function->parent = tree.current_node;                                       //Set the parent of our node
    tree.current_node = new_function->function_body;                                //Set current node to the new function's block

    return NULL;
}
Value* WTS_Parser::wts_end_function(std::string token)
{
    cpp_output += "return 0;\n}\n";

    tree.current_node = tree.current_node->parent->parent;                                    //Get the parent of our parent, that is the node the function was defigned in
                                                                                    //current node is the block->function->function's parent
    return NULL;
}
Value* WTS_Parser::wts_go_function(std::string token)
{
    std::string call_name = reader.word();
    cpp_output += call_name + "()";

    ASTNode_Call* new_call = new ASTNode_Call;
    new_call->name = call_name;
    new_call->function = tree.functions[call_name];                                 //Look up the pointer to the function prototype in the tree's functions map, and assign it to the call's function prototype pointer
    if (new_call->function == 0)                                                    //Throw an exception if we don't find it. This should be expanded later.
        throw new SyntaxErrorException;
    tree.current_node->addChild(new_call);

    return NULL;
}

Value* WTS_Parser::wts_variable(std::string token, Value::value_type incoming_type) //This function creates a variable of the appropriet type. It is called by the below functions.
{
    std::string variable_name = reader.word();
    cpp_output += "variable " + variable_name;

    ASTNode_Variable* new_variable = new ASTNode_Variable;
    new_variable->name = variable_name;
    new_variable->value->val_type = incoming_type;
    tree.variables[variable_name] = new_variable;
    tree.current_node->addChild(new_variable);

    return NULL;

}

Value* WTS_Parser::wts_integer(std::string token)
{
    return wts_variable(token, Value::typ_int);
}
Value* WTS_Parser::wts_unsigned_integer(std::string token)
{
    return wts_variable(token, Value::typ_uint);
}
Value* WTS_Parser::wts_floating_point(std::string token)
{
    return wts_variable(token, Value::typ_float);
}
Value* WTS_Parser::wts_boolean(std::string token)
{
    return wts_variable(token, Value::typ_bool);
}


Value* WTS_Parser::wts_end_statement(std::string token)
{
    cpp_output += token + "\n";
    return NULL;
    //No representation for an end of statement in the AST. If the final language uses end_statements, it will be in the upper parts of the parser
}
Value* WTS_Parser::wts_binary_operator(std::string token)
{
    cpp_output += "( ";
    Value* param1 = do_token(reader.word());
    cpp_output += " " + token + " ";
    Value* param2 = do_token(reader.word());
    cpp_output += " )";

    std::cout << "wts_binary_operator: " << token << "\n";

    std::string call_name = token;
    ASTNode_Call* new_call = new ASTNode_Call;
    new_call->name = call_name;
    new_call->function = tree.functions[call_name];                                 //Look up the pointer to the function prototype in the tree's functions map, and assign it to the call's function prototype pointer
    if (new_call->function == 0)                                                    //Throw an exception if we don't find it. This should be expanded later.
        throw -1;
    new_call->parameters.push_back(param1);
    new_call->parameters.push_back(param2);
    Value* return_value = new Value();
    return_value->val_type = Value::typ_call;
    return_value->data.dat_call = new_call;
    std::cout << "This is the value of the wts_binary_operator: " << return_value << "\n";
    return return_value;    
}

Value* WTS_Parser::wts_unary_operator(std::string token)
{
    cpp_output += "( ";
    Value* param1 = do_token(reader.word());
    cpp_output += " " + token + " )";

    std::cout << "wts_unary_operator: " << token << "\n";

    std::string call_name = token;
    ASTNode_Call* new_call = new ASTNode_Call;
    new_call->name = call_name;
    new_call->function = tree.functions[call_name];                                 //Look up the pointer to the function prototype in the tree's functions map, and assign it to the call's function prototype pointer
    if (new_call->function == 0)                                                    //Throw an exception if we don't find it. This should be expanded later.
        throw -1;
    new_call->parameters.push_back(param1);
    Value* return_value = new Value();
    return_value->val_type = Value::typ_call;
    return_value->data.dat_call = new_call;
    std::cout << "This is the value of the wts_unary_operator: " << return_value << "\n";
    return return_value;    
}

Value* WTS_Parser::wts_begin_if(std::string token)
{
    cpp_output += "if ( ";


    ASTNode_Statement* new_if_statement = new ASTNode_Statement;      //Create a new statement
    tree.current_node->addChild(new_if_statement);                                      //Add the new statement node to the current node
    new_if_statement->statement_type = ASTNode_Statement::if_statement;
    new_if_statement->condition = do_token(reader.word());


    cpp_output += ")\n{\n";


    if (new_if_statement->condition)
    {
        std::cout << "if condition exists!\n";
        new_if_statement->condition->parent = new_if_statement;                             //Make sure we set parents correctly
    } else {
        std::cout << "if statement with no value!!\n\n\n";
    }
    new_if_statement->first_option = new Block;
    new_if_statement->first_option->parent = new_if_statement;
    new_if_statement->parent = tree.current_node;                                       //Set the parent of our node
    tree.current_node = new_if_statement->first_option;                                //Set current node to the new statement's first option's block
    return NULL;
}
Value* WTS_Parser::wts_end_if(std::string token)
{
    cpp_output += "}\n";
    tree.current_node = tree.current_node->parent->parent;
    return NULL;
}
Value* WTS_Parser::wts_begin_while(std::string token)
{
    cpp_output += "while ( ";


    ASTNode_Statement* new_while_statement = new ASTNode_Statement;      //Create a new statement
    tree.current_node->addChild(new_while_statement);                                      //Add the new statement node to the current node
    new_while_statement->statement_type = ASTNode_Statement::while_statement;
    new_while_statement->condition = do_token(reader.word());


    cpp_output += ")\n{\n";


    if (new_while_statement->condition)
    {
        std::cout << "while condition exists!\n";
        new_while_statement->condition->parent = new_while_statement;                             //Make sure we set parents correctly
    } else {
        std::cout << "while statement with no value!!\n\n\n";
    }
    new_while_statement->first_option = new Block;
    new_while_statement->first_option->parent = new_while_statement;
    new_while_statement->parent = tree.current_node;                                       //Set the parent of our node
    tree.current_node = new_while_statement->first_option;                                //Set current node to the new statement's first option's block

    return NULL;

}
Value* WTS_Parser::wts_end_while(std::string token)
{
    cpp_output += "}\n";
    tree.current_node = tree.current_node->parent->parent;
    return NULL;
}

void WTS_Parser::initialize_map()
{
    wts_KeyWordsMap["begin"] = &WTS_Parser::wts_begin_function;
    wts_KeyWordsMap["end"] = &WTS_Parser::wts_end_function;
    wts_KeyWordsMap["go"] = &WTS_Parser::wts_go_function;
    wts_KeyWordsMap["print"] = &WTS_Parser::wts_unary_operator;
    wts_KeyWordsMap["int"] = &WTS_Parser::wts_integer;
    wts_KeyWordsMap["uint"] = &WTS_Parser::wts_unsigned_integer;
    wts_KeyWordsMap["float"] = &WTS_Parser::wts_floating_point;
    wts_KeyWordsMap["bool"] = &WTS_Parser::wts_boolean;
    wts_KeyWordsMap[";"] = &WTS_Parser::wts_end_statement;
    wts_KeyWordsMap["="] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["+"] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["-"] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["*"] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["/"] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["%"] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["^"] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["if"] = &WTS_Parser::wts_begin_if;
    wts_KeyWordsMap["endif"] = &WTS_Parser::wts_end_if;
    wts_KeyWordsMap["while"] = &WTS_Parser::wts_begin_while;
    wts_KeyWordsMap["endwhile"] = &WTS_Parser::wts_end_while;
    wts_KeyWordsMap["=="] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["!="] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap[">"] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["<"] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap[">="] = &WTS_Parser::wts_binary_operator;
    wts_KeyWordsMap["<="] = &WTS_Parser::wts_binary_operator;

    std::cout << "wts_KeyWordMap initilized, now contains " << wts_KeyWordsMap.size() << " entries.\n";
}

void WTS_Parser::initialize_builtin_functions()
{                                                                                                   //Notice that we also set the name of the function
    tree.functions["print"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::print, "print");
    tree.functions["="] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::assignment, "assignment");
    tree.functions["+"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::addition, "addition");
    tree.functions["-"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::subtraction, "subtraction");
    tree.functions["*"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::multiplication, "multiplication");
    tree.functions["/"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::division, "division");
    tree.functions["%"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::modulus, "modulus");
    tree.functions["^"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::exponent, "exponent");
    tree.functions["=="] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::equal, "equal");
    tree.functions["!="] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::not_equal, "not_equal");
    tree.functions[">"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::greater_than, "greater_than");
    tree.functions["<"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::less_than, "less_than");
    tree.functions[">="] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::greater_than_or_equal, "greater_than_or_equal");
    tree.functions["<="] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::less_than_or_equal, "less_than_or_equal");
}

Value::value_type WTS_Parser::getValueType(std::string string_in)
{
    if (string_in == "int")
        return Value::typ_int;

    if (string_in == "uint")
        return Value::typ_uint;

    if (string_in == "float")
        return Value::typ_float;

    if (string_in == "double")
        return Value::typ_double;

    if (string_in == "bool")
        return Value::typ_bool;

    if (string_in == "char")
        return Value::typ_char;
}

