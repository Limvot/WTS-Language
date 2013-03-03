#ifndef VALUE_H
#define VALUE_H

#include <stdlib.h>

#include <ASTNode.h>
#include <ASTNode_Call.h>
#include <ASTNode_Variable.h>
#include <Block.h>


class ASTNode_Call; //Fix the circular dependency, close the loop.
class ASTNode_Variable;

class Value : public ASTNode
{
    public:
        Value();
        Value(ASTNode*);
        Value(void* inVoidPtr);
        Value(int inInt);
        Value(unsigned int inUInt);
        Value(float inFloat);
        Value(double inDouble);
        Value(bool inBool);
        Value(char inChar);
        Value(ASTNode_Prototype* inPrototype);
        Value(ASTNode_Call* inCall);
        Value(ASTNode_Variable* inVariable);
        Value(Block* inBlock);

        virtual ~Value();
        
        enum value_type {   typVoid,
                            typInt,
                            typUInt,
                            typFloat,
                            typDouble,
                            typBool,
                            typChar,
                            typVariable,
                            typObject,
                            typFunction,
                            typPrototype,
                            typCall,
                            typBlock }  valType;
        union  {void* datVoidPtr;
                int   datInt;
                unsigned int datUInt;
                float datFloat;
                double datDouble;
                bool  datBool;
                char datChar;
                ASTNode_Variable* datVariable;
                //Figure out how to add support for object instances
                //Figure out how to add support for function pointers
                ASTNode_Prototype* datPrototype;
                ASTNode_Call* datCall;
                Block* datBlock;} data;

        value_type isNumber(std::string inputString);
        bool makeNumber(std::string inputString);

    protected:
    private:
};

#endif // VALUE_H
