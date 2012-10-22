#ifndef ATSNODE_PROTOTYPE_FUNCTION_H
#define ATSNODE_PROTOTYPE_FUNCTION_H

#include <vector>

#include <ATSNode_Prototype.h>
#include <Value.h>


class ATSNode_Prototype_Function : public ATSNode_Prototype
{
    public:
        ATSNode_Prototype_Function();
        virtual ~ATSNode_Prototype_Function();
        std::vector<Value*> parameters;
        unsigned int num_parameters;
        Value::value_type return_type;
    protected:
    private:
};

#endif // ATSNODE_PROTOTYPE_FUNCTION_H
