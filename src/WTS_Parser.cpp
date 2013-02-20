#include "WTS_Parser.h"

WTS_Parser::WTS_Parser()
{
    reached_end = false;
    initializeMap();                                              //Setup out map of keywords
    initializeBuiltinFunctions();                                 //Add inbuilt functions to the AST (+-*/^print, etc)
}

WTS_Parser::~WTS_Parser()
{
    //dtor
}

int WTS_Parser::setString(std::string input_string)
{
    reached_end = false;
    reader.setString(input_string);
    return 0;
}

int WTS_Parser::parse()
{
    std::string current_word;
    while (!reached_end)
    {
        current_word = reader.word(false);                          //Do not truncate ending char
        std::cout << "Current WORD/LINE =||" << current_word << "||end of word\n";
        if (current_word == "")                                     //Reached end-of-file
        {
            reached_end = true;
        } else if (current_word[0] == '#' || current_word[0] == '/' && current_word[1] == '/')                          //If comment
        {
            //reader.line(); //Get rid of rest of line. Doesn't work b/c eats next line 
            //Ignore comments

        } else {
            Value* returnType = doToken(reader.truncateEnd(current_word));            //Evaluate the token, which is the word without the ending char
            if (returnType)
            {
                tree.currentNode->addChild(returnType);
                std::cout << "Value did exist, adding value to currentNode!\n";
            } else {
                std::cout << "Value in top loop did not exist. (Token probally did not transfer into a value)\n";
            }
        }
    }

    std::cout << "Done parsing!\n";

    return 0;
}

AbstractSyntaxTree* WTS_Parser::getTree()
{
    return &tree;
}


Value* WTS_Parser::doToken(std::string token)
{
    //Look up the word in the keywordmap, call assoicated function pointer with argument token
    Value*(WTS_Parser::* function_pointer)(std::string) = wts_KeyWordsMap[token];
    if (function_pointer != 0)
    {
        return (this->*function_pointer)(token);
    } else {
        ASTNode_Variable* variable = tree.variables[token];         //Look up variables in the AST's map, if exists, get the pointer, put it in a Value, and return its pointer
        if (variable)
        {
            return new Value(variable); // 
        } else if (tree.functions[token]) {
            return wts_go_function(token);
        }

        Value* number_value = new Value;
        if (number_value->makeNumber(token))                        //If the conversion to a number works, return the value with the number.
        {
            return number_value;
        }
        delete number_value;                                        //If not, delete and clean up
        std::cout << "Unknown token: " << token << "\n";
        //throw new SyntaxErrorException;
        return NULL;
    }
}


Value* WTS_Parser::wts_function_begin(std::string token)
{
    std::string return_type = reader.word();
    std::string function_name = reader.word();

    ASTNode_Prototype_Function* new_function = new ASTNode_Prototype_Function;      //Create a new function
    new_function->returnType->val_type = getValueType(return_type);
    new_function->name = function_name;                                             //Name the function the supplied name
    tree.functions[function_name] = new_function;                                   //Add the function to our function map, with the name as the key
    tree.currentNode->addChild(new_function);                                       //Add the new function node to the current node
    //std::cout << new_function << std::endl;
    new_function->setBody(doToken(reader.word()));

    return(NULL);
}

//This language assumes that return is always the last statement in the function.
Value* WTS_Parser::wts_function_return(std::string token)
{
    ASTNode_Prototype_Function* parentFunction = static_cast<ASTNode_Prototype_Function*>(tree.findAbove(ASTNode::prototype_function));
    if (parentFunction)
        parentFunction->setReturnValue(doToken(reader.word()));
    else
        throw new SyntaxErrorException;

    return(NULL);
}

Value* WTS_Parser::wts_block_begin(std::string token)
{
    Block* newBlock = new Block();
    Value* valueParentBlock = new Value(newBlock);
    newBlock->setParent(valueParentBlock);
    tree.setCurrentNode(static_cast<ASTNode*>(newBlock));

    return(valueParentBlock);
}

Value* WTS_Parser::wts_block_end(std::string token)
{
    std::cout << tree.currentNode->getLineage()<<std::endl;
    std::cout << ((tree.ascendToUpperBlock()) ? "ascension worked" : "ascension failed\n") << std::endl;
    std::cout << "Block ended, going to block named " + tree.currentNode->name + "\n";
    std::cout << tree.currentNode->getLineage()<<std::endl;
    return(NULL);
}

//This one is special, it is not called by the wts_KeyWordsMap but by doToken directly if there is a match in the function map
Value* WTS_Parser::wts_go_function(std::string call_name)
{
    ASTNode_Call* new_call = new ASTNode_Call;
    new_call->name = call_name;
    new_call->function = tree.functions[call_name];                                 //Look up the pointer to the function prototype in the tree's functions map, and assign it to the call's function prototype pointer
    if (new_call->function == 0)                                                    //Throw an exception if we don't find it. This should be expanded later.
        throw new SyntaxErrorException;
    tree.currentNode->addChild(new_call);

    return(new Value(new_call));
}

Value* WTS_Parser::wts_variable(std::string token)
{
    std::string variable_name = reader.word();

    ASTNode_Variable* new_variable = new ASTNode_Variable;
    new_variable->name = variable_name;
    new_variable->value->val_type = getValueType(token);
    tree.variables[variable_name] = new_variable;
    tree.currentNode->addChild(new_variable);

    return(NULL);

}

Value* WTS_Parser::wts_end_statement(std::string token)
{
    return(NULL);
    //No representation for an end of statement in the AST. If the final language uses end_statements, it will be in the upper parts of the parser
}

Value* WTS_Parser::wts_operator_binary(std::string token)
{
    Value* param1 = doToken(reader.word());
    Value* param2 = doToken(reader.word());    //Will be the correct token because the doToken for param1 will keep getting tokens until done

    std::cout << "wts_operator_binary: " << token << "\n";

    std::string call_name = token;
    ASTNode_Call* new_call = new ASTNode_Call;
    new_call->name = call_name;
    new_call->function = tree.functions[call_name];                                 //Look up the pointer to the function prototype in the tree's functions map, and assign it to the call's function prototype pointer
    if (new_call->function == 0)                                                    //Throw an exception if we don't find it. This should be expanded later.
        throw new SyntaxErrorException;

    new_call->parameters.push_back(param1);
    new_call->parameters.push_back(param2);
    Value* returnType = new Value();
    returnType->val_type = Value::typ_call;
    returnType->data.dat_call = new_call;
    std::cout << "This is the value of the wts_operator_binary: " << returnType << "\n";
    return(returnType);    
}

Value* WTS_Parser::wts_operator_unary(std::string token)
{
    Value* param1 = doToken(reader.word());

    std::cout << "wts_operator_unary: " << token << "\n";

    std::string call_name = token;
    ASTNode_Call* new_call = new ASTNode_Call;
    new_call->name = call_name;
    new_call->function = tree.functions[call_name];                                 //Look up the pointer to the function prototype in the tree's functions map, and assign it to the call's function prototype pointer
    if (new_call->function == 0)                                                    //Throw an exception if we don't find it. This should be expanded later.
        throw new SyntaxErrorException;

    new_call->parameters.push_back(param1);
    Value* returnType = new Value();
    returnType->val_type = Value::typ_call;
    returnType->data.dat_call = new_call;
    std::cout << "This is the value of the wts_operator_unary: " << returnType << "\n";
    return(returnType);    
}

Value* WTS_Parser::wts_begin_if(std::string token)
{

    ASTNode_Statement* new_if_statement = new ASTNode_Statement;                        //Create a new statement
    tree.currentNode->addChild(new_if_statement);                                      //Add the new statement node to the current node
    new_if_statement->statement_type = ASTNode_Statement::if_statement;
    new_if_statement->condition = doToken(reader.word());


    if (new_if_statement->condition)
    {
        std::cout << "if condition exists!\n";
        new_if_statement->condition->parent = new_if_statement;                             //Make sure we set parents correctly
    } else {
        std::cout << "if statement with no value!!\n\n\n";
        throw new SyntaxErrorException;
    }
    new_if_statement->parent = tree.currentNode; 
    new_if_statement->first_option = doToken(reader.word());
    new_if_statement->first_option->parent = new_if_statement;
    return(NULL);
}

Value* WTS_Parser::wts_begin_while(std::string token)
{

    ASTNode_Statement* new_while_statement = new ASTNode_Statement;      //Create a new statement
    tree.currentNode->addChild(new_while_statement);                                      //Add the new statement node to the current node
    new_while_statement->statement_type = ASTNode_Statement::while_statement;
    new_while_statement->condition = doToken(reader.word());


    if (new_while_statement->condition)
    {
        std::cout << "while condition exists!\n";
        new_while_statement->condition->parent = new_while_statement;                             //Make sure we set parents correctly
    } else {
        std::cout << "while statement with no value!!\n\n\n";
        throw new SyntaxErrorException;
    }
    new_while_statement->parent = tree.currentNode;
    new_while_statement->first_option = doToken(reader.word());
    std::cout << new_while_statement->first_option;
    new_while_statement->first_option->parent = new_while_statement;                         //Set current node to the new statement's first option's block
    return(NULL);

}


void WTS_Parser::initializeMap()
{
    wts_KeyWordsMap["def"] = &WTS_Parser::wts_function_begin;
    wts_KeyWordsMap["return"] = &WTS_Parser::wts_function_return;
    wts_KeyWordsMap["{"] = &WTS_Parser::wts_block_begin;
    wts_KeyWordsMap["}"] = &WTS_Parser::wts_block_end;
    wts_KeyWordsMap["print"] = &WTS_Parser::wts_operator_unary;
    wts_KeyWordsMap["int"] = &WTS_Parser::wts_variable;
    wts_KeyWordsMap["uint"] = &WTS_Parser::wts_variable;
    wts_KeyWordsMap["float"] = &WTS_Parser::wts_variable;
    wts_KeyWordsMap["bool"] = &WTS_Parser::wts_variable;
    wts_KeyWordsMap[";"] = &WTS_Parser::wts_end_statement;
    wts_KeyWordsMap["="] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["+"] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["-"] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["*"] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["/"] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["%"] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["^"] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["if"] = &WTS_Parser::wts_begin_if;
    wts_KeyWordsMap["while"] = &WTS_Parser::wts_begin_while;
    wts_KeyWordsMap["=="] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["!="] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap[">"] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["<"] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap[">="] = &WTS_Parser::wts_operator_binary;
    wts_KeyWordsMap["<="] = &WTS_Parser::wts_operator_binary;

    std::cout << "wts_KeyWordMap initilized, now contains " << wts_KeyWordsMap.size() << " entries.\n";
}

void WTS_Parser::initializeBuiltinFunctions()
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

