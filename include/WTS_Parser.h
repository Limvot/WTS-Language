#ifndef WTS_PARSER_H
#define WTS_PARSER_H

#include <map>
#include <string>
#include <iostream>

#include "string_reader.h"
#include "SyntaxErrorException.h"
#include "AbstractSyntaxTree.h"
#include "ASTNode.h"
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
        int set_string(std::string input_string);
        int parse();
        AbstractSyntaxTree* getTree();
        Value* do_token(std::string token);

        std::map<std::string, Value*(WTS_Parser::*)(std::string)> wts_KeyWordsMap;
        void initialize_map();
        void initialize_builtin_functions();
        Value::value_type getValueType(std::string string_in);

        AbstractSyntaxTree tree;

    protected:
        bool reached_end;
        string_reader reader;
        std::string next_token;

        Value* wts_begin_function(std::string token);
        Value* wts_end_function(std::string token);
        Value* wts_go_function(std::string token);
        Value* wts_variable(std::string token, Value::value_type);    //This one is special. It is not called from or included in the map, but is called by the other specilized variable functions
        Value* wts_integer(std::string token);
        Value* wts_unsigned_integer(std::string token);
        Value* wts_floating_point(std::string token);
        Value* wts_boolean(std::string token);
        Value* wts_end_statement(std::string token);
        Value* wts_binary_operator(std::string token);
        Value* wts_unary_operator(std::string token);
        Value* wts_begin_if(std::string token);
        Value* wts_end_if(std::string token);
        Value* wts_begin_while(std::string token);
        Value* wts_end_while(std::string token);

    private:
};

#endif // WTS_PARSER_H
