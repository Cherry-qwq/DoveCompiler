#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Value.h"

namespace ir
{
    class User : public Value
    {
        public:
        explicit User(std::shared_ptr<Type> type, std::string name = "") : Value(type, name){
            value_type_=ValueType::User;
        };
        bool isStaticValue() const
        {
            return is_staticvalue_||value_type_==ValueType::StaticValue;
        }
        bool isAllocate() const
        {
            return is_allocate_||value_type_==ValueType::Allocate;
        }
        bool isFunction() const
        {
            return is_function_||value_type_==ValueType::Function;
        }

    protected:
        bool is_staticvalue_=false;
        bool is_allocate_=false;
        bool is_function_=false;
    };
}
