#pragma once

#include <memory>

#include "Type.h"
#include "User.h"

namespace ir
{
    class StaticValue : public User
    {
    public:
        explicit StaticValue(std::shared_ptr<Type> type, std::string name, int32_t val) : User(type, std::move(name)), is_int_(true), int_val_(val) { value_type_=ValueType::StaticValue; }
        explicit StaticValue(std::shared_ptr<Type> type, std::string name, float val) : User(type, std::move(name)), is_float_(true), float_val_(val) { value_type_=ValueType::StaticValue; }
        explicit StaticValue(std::shared_ptr<Type> type, std::string name, bool val) : User(type, std::move(name)), is_bool_(true), bool_val_(val) { value_type_=ValueType::StaticValue; }
        explicit StaticValue(std::shared_ptr<Type> type, std::string name, std::vector<std::shared_ptr<StaticValue>> vals) : User(type, std::move(name)), is_array_(true), array_vals_(vals) { value_type_=ValueType::StaticValue; }
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "StaticValue ";
            if (is_int_)
                output += std::to_string(int_val_);
            else if (is_float_)
                output += std::to_string(float_val_);
            else if (is_bool_)
                output += std::to_string(bool_val_);
            else if(is_array_)
                for(auto val: array_vals_)
                    output += val->dump(helper) + ", ";
            return output;
        }

        bool isInt() const
        {
            return is_int_;
        }

        bool isFloat() const
        {
            return is_float_;
        }

        bool isBool() const
        {
            return is_bool_;
        }
        bool isArray() const
        {
            return is_array_;
        }

        int32_t getIntVal() const
        {
            return int_val_;
        }
        float getFloatVal() const
        {
            return float_val_;
        }
        bool getBoolVal() const
        {
            return bool_val_;
        }
        std::shared_ptr<StaticValue> arrAt(size_t idx) const
        {
            return array_vals_[idx];
        }
        std::shared_ptr<StaticValue> arrAt(std::vector<size_t> idxs) const
        {
            auto val = array_vals_[idxs[0]];
            if (val->isArray())
            {
                return val->arrAt(std::vector<size_t>(idxs.begin() + 1, idxs.end()));
            }
            else
            {
                return val;
            }
        }
        std::vector<std::shared_ptr<StaticValue>> getArrayVal() const
        {
            return array_vals_;
        }

    protected:
        bool is_int_ = false;
        int32_t int_val_;
        bool is_float_ = false;
        float float_val_;
        bool is_bool_ = false;
        bool bool_val_;
        bool is_array_ = false;
        std::vector<std::shared_ptr<StaticValue>> array_vals_;
    };
}