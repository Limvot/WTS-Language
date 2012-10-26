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
            do_token(reader.truncate_end(current_word));            //Evaluate the token, which is the word without the ending char
        }
    }

    std::cout << "Done parsing!\n";

    return 0;
}

std::string WTS_Parser::getCPP()
{
    return cpp_output;
}


Value* WTS_Parser::do_token(std::string token)
{
    //Look up the word in the keywordmap, call assoicated function pointer with argument token
    Value*(WTS_Parser::* function_pointer)(std::string) = wts_KeyWordsMap[token];
    if (function_pointer != 0)
    {
        return (this->*function_pointer)(token);
    }
    else
    {
        cpp_output += token;    //CHANGE THIS! If we don't know what it is, we output it. This is for varyables, but should be replaced with lookup table
    }
}


Value* WTS_Parser::wts_begin_function(std::string token)
{
    std::string function_name = reader.word();
    cpp_output += "int " + function_name + "() {\n";

    ASTNode_Prototype_Function* new_function = new ASTNode_Prototype_Function;      //Create a new function
    new_function->name = function_name;                                             //Name the function the supplied name
    tree.functions[function_name] = new_function;                                   //Add the function to our function map, with the name as the key
    tree.current_node->addChild(new_function);                                      //Add the new function node to the current node
    new_function->parent = tree.current_node;                                       //Set the parent of our node
    tree.current_node = new_function->function_body;                                //Set current node to the new function's block
}
Value* WTS_Parser::wts_end_function(std::string token)
{
    cpp_output += "return 0;\n}\n";

    tree.current_node = tree.current_node->parent->parent;                                    //Get the parent of our parent, that is the node the function was defigned in
                                                                                    //current node is the block->function->function's parent
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
}
Value* WTS_Parser::wts_print(std::string token)
{
    cpp_output += "std::cout << " + reader.line();

    //ADD SUPPORT FOR EXTERNAL FUNCTIONS!!!! (Standerd Library)
}
Value* WTS_Parser::wts_integer(std::string token)
{
    std::string variable_name = reader.word();
    cpp_output += "int " + variable_name;

    ASTNode_Variable* new_variable = new ASTNode_Variable;
    new_variable->name = variable_name;
    new_variable->value->val_type = Value::typ_int;
    tree.variables[variable_name] = new_variable;
    tree.current_node->addChild(new_variable);

}
Value* WTS_Parser::wts_unsigned_integer(std::string token)
{
    std::string variable_name = reader.word();
    cpp_output += "unsigned int " + variable_name;

    ASTNode_Variable* new_variable = new ASTNode_Variable;
    new_variable->name = variable_name;
    new_variable->value->val_type = Value::typ_uint;
    tree.variables[variable_name] = new_variable;
    tree.current_node->addChild(new_variable);
}
Value* WTS_Parser::wts_floating_point(std::string token)
{
    std::string variable_name = reader.word();
    cpp_output += "float " + variable_name;

    ASTNode_Variable* new_variable = new ASTNode_Variable;
    new_variable->name = variable_name;
    new_variable->value->val_type = Value::typ_float;
    tree.variables[variable_name] = new_variable;
    tree.current_node->addChild(new_variable);
}
Value* WTS_Parser::wts_boolean(std::string token)
{
    std::string variable_name = reader.word();
    cpp_output += "bool " + variable_name;

    ASTNode_Variable* new_variable = new ASTNode_Variable;
    new_variable->name = variable_name;
    new_variable->value->val_type = Value::typ_bool;
    tree.variables[variable_name] = new_variable;
    tree.current_node->addChild(new_variable);
}
Value* WTS_Parser::wts_end_statement(std::string token)
{
    cpp_output += token + "\n";

    //No representation for an end of statement in the AST. If the final language uses end_statements, it will be in the upper parts of the parser
}
Value* WTS_Parser::wts_simple_token_replacement(std::string token)
{
    cpp_output += "( ";
    Value* param1 = do_token(reader.word());
    cpp_output += " " + token + " ";
    Value* param2 = do_token(reader.word());
    cpp_output += " )";


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
    return return_value;

    
}
Value* WTS_Parser::wts_equals(std::string token)
{
    std::string variable_name = reader.word();
    cpp_output += variable_name + " = ";
    tree.variables[variable_name]->value = do_token(reader.word());
}
Value* WTS_Parser::wts_begin_if(std::string token)
{
    cpp_output += "if ( ";
    do_token(reader.word());
    cpp_output += ")\n{\n";
}
Value* WTS_Parser::wts_end_if(std::string token)
{
    cpp_output += "}\n";
}
Value* WTS_Parser::wts_begin_while(std::string token)
{
    cpp_output += "while ( ";
    do_token(reader.word());
    cpp_output += ")\n{\n";
}
Value* WTS_Parser::wts_end_while(std::string token)
{
    cpp_output += "}\n";
}

void WTS_Parser::initialize_map()
{
    wts_KeyWordsMap["begin"] = &WTS_Parser::wts_begin_function;
    wts_KeyWordsMap["end"] = &WTS_Parser::wts_end_function;
    wts_KeyWordsMap["go"] = &WTS_Parser::wts_go_function;
    wts_KeyWordsMap["print"] = &WTS_Parser::wts_print;
    wts_KeyWordsMap["int"] = &WTS_Parser::wts_integer;
    wts_KeyWordsMap["uint"] = &WTS_Parser::wts_unsigned_integer;
    wts_KeyWordsMap["float"] = &WTS_Parser::wts_floating_point;
    wts_KeyWordsMap["bool"] = &WTS_Parser::wts_boolean;
    wts_KeyWordsMap[";"] = &WTS_Parser::wts_end_statement;
    wts_KeyWordsMap["="] = &WTS_Parser::wts_equals;
    wts_KeyWordsMap["+"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["-"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["*"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["/"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["^"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["if"] = &WTS_Parser::wts_begin_if;
    wts_KeyWordsMap["endif"] = &WTS_Parser::wts_end_if;
    wts_KeyWordsMap["while"] = &WTS_Parser::wts_begin_while;
    wts_KeyWordsMap["endwhile"] = &WTS_Parser::wts_end_while;
    wts_KeyWordsMap["=="] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["!="] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap[">"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["<"] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap[">="] = &WTS_Parser::wts_simple_token_replacement;
    wts_KeyWordsMap["<="] = &WTS_Parser::wts_simple_token_replacement;

    std::cout << "wts_KeyWordMap initilized, now contains " << wts_KeyWordsMap.size() << " entries.\n";
}

void WTS_Parser::initialize_builtin_functions()
{
    tree.functions["+"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::addition);
    tree.functions["-"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::subtraction);
    tree.functions["*"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::multiplication);
    tree.functions["/"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::division);
    tree.functions["^"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::exponent);
    tree.functions["=="] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::equal);
    tree.functions["!="] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::not_equal);
    tree.functions[">"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::greater_than);
    tree.functions["<"] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::less_than);
    tree.functions[">="] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::greater_than_or_equal);
    tree.functions["<="] = new ASTNode_Prototype_Function_Builtin(ASTNode_Prototype_Function_Builtin::less_than_or_equal);
}

