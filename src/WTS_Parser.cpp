#include "WTS_Parser.h"

WTS_Parser::WTS_Parser()
{
    reachedEnd = false;
    initializeMap();                                              //Setup out map of keywords
    initializeBuiltinFunctions();                                 //Add inbuilt functions to the AST (+-*/^print, etc)
}

WTS_Parser::~WTS_Parser()
{
    //dtor
}

int WTS_Parser::setString(std::string inputString)
{
    reachedEnd = false;
    reader.setString(inputString);
    return 0;
}

int WTS_Parser::parse()
{
    std::string currentWord;
    while (!reachedEnd)
    {
        currentWord = reader.word(false);                          //Do not truncate ending char
        std::cout << "Current WORD/LINE =||" << currentWord << "||end of word\n";
        if (currentWord == "")                                     //Reached end-of-file
        {
            reachedEnd = true;
        } else if (currentWord[0] == '#' || currentWord[0] == '/' && currentWord[1] == '/')                          //If comment
        {
            //reader.line(); //Get rid of rest of line. Doesn't work b/c eats next line if one word comment
            //Ignore comments

        } else {
            Value* returnType = doToken(reader.truncateEnd(currentWord));            //Evaluate the token, which is the word without the ending char
            if (returnType)
            {
                tree.root->addChild(returnType);
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
        if (tree.variables.find(token) != tree.variables.end())
        {
            return new Value(tree.variables[token]); // 
        } else if (tree.functions.find(token) != tree.functions.end()) {
            return wts_go_function(token);
        }

        Value* numberValue = new Value;
        if (numberValue->makeNumber(token))                        //If the conversion to a number works, return the value with the number.
        {
            return numberValue;
        }
        delete numberValue;                                        //If not, delete and clean up
        std::cout << "Unknown token: ||" << token << "||\n";
        std::cout << tree.currentNode->getLineage() << std::endl;
        //throw new SyntaxErrorException;
        return NULL;
    }
}


Value* WTS_Parser::wts_function_begin(std::string token)
{
    std::string returnType = reader.word();
    std::string functionName = reader.word();

    ASTNode_Prototype_Function* newFunction = new ASTNode_Prototype_Function;      //Create a new function
    newFunction->returnType->valType = getValueType(returnType);
    newFunction->name = functionName;                                             //Name the function the supplied name
    tree.functions[functionName] = newFunction;                                   //Add the function to our function map, with the name as the key
    tree.setCurrentNode(newFunction);                                             //Add the new function node to the current node

    std::string paramString = reader.word();
    while(paramString != "{") {
        std::cout << "First param is " << paramString << std::endl;
        newFunction->parameters.push_back(doToken(paramString));
        paramString = reader.word();
    }    
    newFunction->setBody(wts_block_begin(paramString));

    return(new Value(static_cast<ASTNode_Prototype*>(newFunction)));
}

//This language assumes that return is always the last statement in the function.
Value* WTS_Parser::wts_function_return(std::string token)
{
    ASTNode_Prototype_Function* parentFunction = static_cast<ASTNode_Prototype_Function*>(tree.findAbove(ASTNode::prototype_function));
    if (parentFunction) {
        std::string returnString = reader.word();
        std::cout << "returns " << returnString << std::endl;
        parentFunction->setReturnValue(doToken(returnString));
    }
    else
        throw new SyntaxErrorException;

    return(NULL);
}

Value* WTS_Parser::wts_block_begin(std::string token)
{
    Block* newBlock = new Block();
    Value* valueParentBlock = new Value(newBlock);
    newBlock->setParent(valueParentBlock);
    valueParentBlock->setParent(tree.currentNode);
    ASTNode* toRestoreCurrentNode = tree.currentNode;
    tree.setCurrentNode(static_cast<ASTNode*>(newBlock));

    bool reachedEnd = false;
    while (!reachedEnd)
    {
        std::string currentWord = reader.word(false);                          //Do not truncate ending char
        std::cout << "Current WORD/LINE =||" << currentWord << "||end of word\n";
        if (currentWord == "" || currentWord[0] == '}')                                     //Reached end-of-file
        {
            reachedEnd = true;
        } else if (currentWord[0] == '#' || currentWord[0] == '/' && currentWord[1] == '/')                          //If comment
        {
            //reader.line(); //Get rid of rest of line. Doesn't work b/c eats next line if one word comment
            //Ignore comments

        } else {
            Value* returnType = doToken(reader.truncateEnd(currentWord));            //Evaluate the token, which is the word without the ending char
            if (returnType)
                newBlock->addChild(returnType);
        }
    }
    reachedEnd = false;
    tree.setCurrentNode(toRestoreCurrentNode);
    return(valueParentBlock);
}

//This one is special, it is not called by the wts_KeyWordsMap but by doToken directly if there is a match in the function map
Value* WTS_Parser::wts_go_function(std::string callName)
{
    ASTNode_Call* newCall = new ASTNode_Call;
    newCall->name = callName;
    newCall->function = tree.functions[callName];                                 //Look up the pointer to the function prototype in the tree's functions map, and assign it to the call's function prototype pointer
    if (newCall->function == 0)                                                    //Throw an exception if we don't find it. This should be expanded later.
        throw new SyntaxErrorException;

    for (int i = 0; i < newCall->function->parameters.size(); i++) {
        newCall->parameters.push_back(doToken(reader.word()));
    }

    return(new Value(newCall));
}

Value* WTS_Parser::wts_variable(std::string token)
{
    std::string variable_name = reader.word();

    ASTNode_Variable* newVariable = new ASTNode_Variable;
    newVariable->name = variable_name;
    newVariable->value->valType = getValueType(token);
    tree.variables[variable_name] = newVariable;

    return(new Value( new ASTNode_Prototype_Variable(newVariable)));

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

    std::string callName = token;
    ASTNode_Call* newCall = new ASTNode_Call;
    newCall->name = callName;
    newCall->function = tree.functions[callName];                                 //Look up the pointer to the function prototype in the tree's functions map, and assign it to the call's function prototype pointer
    if (newCall->function == 0)                                                    //Throw an exception if we don't find it. This should be expanded later.
        throw new SyntaxErrorException;

    newCall->parameters.push_back(param1);
    newCall->parameters.push_back(param2);
    Value* returnType = new Value();
    returnType->valType = Value::typCall;
    returnType->data.datCall = newCall;
    std::cout << "This is the value of the wts_operator_binary: " << returnType << "\n";
    return(returnType);    
}

Value* WTS_Parser::wts_operator_unary(std::string token)
{
    Value* param1 = doToken(reader.word());

    std::cout << "wts_operator_unary: " << token << "\n";

    std::string callName = token;
    ASTNode_Call* newCall = new ASTNode_Call;
    newCall->name = callName;
    newCall->function = tree.functions[callName];                                 //Look up the pointer to the function prototype in the tree's functions map, and assign it to the call's function prototype pointer
    if (newCall->function == 0)                                                    //Throw an exception if we don't find it. This should be expanded later.
        throw new SyntaxErrorException;

    newCall->parameters.push_back(param1);
    Value* returnType = new Value();
    returnType->valType = Value::typCall;
    returnType->data.datCall = newCall;
    std::cout << "This is the value of the wts_operator_unary: " << returnType << "\n";
    return(returnType);    
}

Value* WTS_Parser::wts_begin_if(std::string token)
{

    ASTNode_Statement* new_if_statement = new ASTNode_Statement;                        //Create a new statement
    new_if_statement->statementType = ASTNode_Statement::if_statement;
    new_if_statement->condition = doToken(reader.word());


    if (new_if_statement->condition)
    {
        std::cout << "if condition exists!\n";
        new_if_statement->condition->setParent(new_if_statement);
    } else {
        std::cout << "if statement with no value!!\n\n\n";
        throw new SyntaxErrorException;
    }
    new_if_statement->firstOption = doToken(reader.word());
    new_if_statement->firstOption->setParent(new_if_statement);
    return(new Value(new_if_statement));
}

Value* WTS_Parser::wts_begin_while(std::string token)
{

    ASTNode_Statement* new_while_statement = new ASTNode_Statement;
    new_while_statement->statementType = ASTNode_Statement::while_statement;
    new_while_statement->condition = doToken(reader.word());

    if (new_while_statement->condition)
    {
        std::cout << "while condition exists!\n";
        new_while_statement->condition->setParent(new_while_statement);
    } else {
        std::cout << "while statement with no value!!\n\n\n";
        throw new SyntaxErrorException;
    }
    new_while_statement->firstOption = doToken(reader.word());
    new_while_statement->firstOption->setParent(new_while_statement);

    return(new Value(new_while_statement));
}


void WTS_Parser::initializeMap()
{
    wts_KeyWordsMap["def"] = &WTS_Parser::wts_function_begin;
    wts_KeyWordsMap["return"] = &WTS_Parser::wts_function_return;
    wts_KeyWordsMap["{"] = &WTS_Parser::wts_block_begin;
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
        return Value::typInt;

    if (string_in == "uint")
        return Value::typUInt;

    if (string_in == "float")
        return Value::typFloat;

    if (string_in == "double")
        return Value::typDouble;

    if (string_in == "bool")
        return Value::typBool;

    if (string_in == "char")
        return Value::typChar;
}

