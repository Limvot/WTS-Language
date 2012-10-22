cmake_minimum_required (VERSION 2.6)

project(WTS-Language-Compiler)

set( MY_INCLUDES ${PROJECT_SOURCE_DIR}/include)

set( MY_SOURCES main.cpp src/Program.cpp src/ASTNode.cpp src/ASTNode_Call.cpp src/ATSNode_Prototype.cpp src/ATSNode_Prototype_Function.cpp src/ATSNode_Prototype_Object.cpp src/ATSNode_Statement.cpp src/ATSNode_Variable.cpp src/Block.cpp src/Node.cpp src/string_reader.cpp src/Value.cpp src/WTS_Parser.cpp)

include_directories( ${MY_INCLUDES} )

add_executable(WTSCompiler ${MY_SOURCES})


