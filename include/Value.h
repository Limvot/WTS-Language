#ifndef VALUE_H
#define VALUE_H

#include <ASTNode.h>
#include <ASTNode_Call.h>

class ASTNode_Call; //Fix the circular dependency, close the loop.

class Value : public ASTNode
{
    public:
        Value();
        Value(void* in_void_ptr);
        Value(int in_int);
        Value(unsigned int in_uint);
        Value(float in_float);
        Value(double in_double);
        Value(bool in_bool);
        Value(char in_char);
        Value(ASTNode_Call* in_call);

        virtual ~Value();
        enum value_type {   typ_void,
                            typ_int,
                            typ_uint,
                            typ_float,
                            typ_double,
                            typ_bool,
                            typ_char,
                            typ_object,
                            typ_function,
                            typ_call }  val_type;
        union  {void* dat_void_ptr;
                int   dat_int;
                unsigned int dat_uint;
                float dat_float;
                double dat_double;
                bool  dat_bool;
                char dat_char;
                //Figure out how to add support for object instances
                //Figure out how to add support for function pointers
                ASTNode_Call* dat_call;} data;
    protected:
    private:
};

#endif // VALUE_H
