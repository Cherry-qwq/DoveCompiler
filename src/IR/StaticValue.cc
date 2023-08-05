#include "StaticValue.h"


namespace ir{
    std::shared_ptr<StaticValue> MakeEmptyStaticValue(std::shared_ptr<Type> type, std::string name)
    {
        if (type->isArray())
        {
            auto arr_type = std::dynamic_pointer_cast<ArrayType>(type);
            auto arr_count = arr_type->count();
            auto vals = std::vector<std::shared_ptr<StaticValue>>(arr_count);
            for (size_t i = 0; i < arr_count; i++)
            {
                vals[i]=MakeEmptyStaticValue(arr_type->getInternalType(), name);
            }
            return std::make_shared<StaticValue>(type, name, vals);
        }
        else if (type->isPointer())
        {
            throw std::runtime_error("StaticValue can't be pointer");
        }
        else if (type->isPrimitive())
        {
            auto prim_type = std::dynamic_pointer_cast<PrimitiveDataType>(type);
            if (prim_type->isInt())
            {
                return std::make_shared<StaticValue>(name, 0);
            }
            else if (prim_type->isFloat())
            {
                return std::make_shared<StaticValue>(name, 0.0f);
            }
            else if (prim_type->isBool())
            {
                return std::make_shared<StaticValue>(name, false);
            }
            else
            {
                throw std::runtime_error("Unknown primitive type");
            }
        }
        else
        {
            throw std::runtime_error("Unknown type");
        }
    }
}