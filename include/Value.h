#ifndef VALUE_H
#define VALUE_H

#include <ASTNode.h>


class Value : public ASTNode
{
    public:
        Value();
        virtual ~Value();
        enum value_type {   typ_void,
                            typ_int,
                            type_uint,
                            typ_float,
                            typ_double,
                            typ_bool,
                            typ_char,
                            typ_object,
                            typ_function }  val_type;
    protected:
    private:
};

#endif // VALUE_H
