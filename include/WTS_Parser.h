#ifndef WTS_PARSER_H
#define WTS_PARSER_H

#include <map>
#include <string>
#include <iostream>

#include "StringReader.h"
#include "SyntaxErrorException.h"
#include "AbstractSyntaxTree.h"
#include "ASTNode.h"
#include "ASTNode_Prototype_Variable.h"
#include "ASTNode_Prototype_Function.h"
#include "ASTNode_Prototype_Function_Builtin.h"
#include "ASTNode_Call.h"
#include "ASTNode_Variable.h"
#include "Value.h"
#include "ASTNode_Statement.h"

class WTS_Parser
{
    public:
        WTS_Parser();
        virtual ~WTS_Parser();
        int setString(std::string input_string);
        int parse();
        AbstractSyntaxTree* getTree();
        Value* doToken(std::string token);

        std::map<std::string, Value*(WTS_Parser::*)(std::string)> wts_KeyWordsMap;
        void initializeMap();
        void initializeBuiltinFunctions();
        Value::value_type getValueType(std::string string_in);

        AbstractSyntaxTree tree;

    protected:
        bool reached_end;
        StringReader reader;
        std::string next_token;

        Value* wts_function_begin(std::string token);
        Value* wts_function_return(std::string token);
        Value* wts_block_begin(std::string token);
        Value* wts_block_end(std::string token);
        Value* wts_go_function(std::string token);
        Value* wts_variable(std::string token);
        Value* wts_end_statement(std::string token);
        Value* wts_operator_binary(std::string token);
        Value* wts_operator_unary(std::string token);
        Value* wts_begin_if(std::string token);
        Value* wts_begin_while(std::string token);

    private:
};

#endif // WTS_PARSER_H
