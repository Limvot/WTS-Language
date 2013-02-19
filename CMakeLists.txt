cmake_minimum_required (VERSION 2.6)

project(WTS-Language-Compiler)

set( MY_INCLUDES ${PROJECT_SOURCE_DIR}/include)

set( MY_SOURCES main.cpp src/Program.cpp src/AbstractSyntaxTree.cpp src/ASTNode.cpp src/ASTNode_Call.cpp src/ASTNode_Prototype.cpp src/ASTNode_Prototype_Function.cpp src/ASTNode_Prototype_Function_Builtin.cpp src/ASTNode_Prototype_Object.cpp src/ASTNode_Statement.cpp src/ASTNode_Variable.cpp src/Block.cpp src/StringReader.cpp src/Value.cpp src/WTS_Parser.cpp src/CodeGenerator.cpp src/CCodeGenerator.cpp src/GenInfoCodeGenerator.cpp src/Interpreter.cpp)

include_directories( ${MY_INCLUDES} )

add_executable(WTSCompiler ${MY_SOURCES})

