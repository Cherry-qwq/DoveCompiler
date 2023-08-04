#pragma once

#include <memory>

#include "Type.h"
#include "User.h"

namespace ir
{
    class StaticValue : public User
    {
    public:
        explicit StaticValue(std::shared_ptr<Type> type, std::string name, int32_t val) : User(type, std::to_string(val)), is_int_(true), int_val_(val) { value_type_ = ValueType::StaticValue; }
        explicit StaticValue(std::shared_ptr<Type> type, std::string name, float val) : User(type, std::to_string(val)), is_float_(true), float_val_(val) { value_type_ = ValueType::StaticValue; }
        explicit StaticValue(std::shared_ptr<Type> type, std::string name, bool val) : User(type, std::to_string(val)), is_bool_(true), bool_val_(val) { value_type_ = ValueType::StaticValue; }
        explicit StaticValue(std::shared_ptr<Type> type, std::string name, std::vector<std::shared_ptr<StaticValue>> vals) : User(type, std::move(name)), is_array_(true), array_vals_(vals) { value_type_ = ValueType::StaticValue; }
        explicit StaticValue(std::string name, int32_t val) : User(MakePrimitiveDataType(PrimitiveDataType::TypeID::Int32), std::to_string(val)), is_int_(true), int_val_(val) { value_type_ = ValueType::StaticValue; }
        explicit StaticValue(std::string name, float val) : User(MakePrimitiveDataType(PrimitiveDataType::TypeID::Float32), std::to_string(val)), is_float_(true), float_val_(val) { value_type_ = ValueType::StaticValue; }
        explicit StaticValue(std::string name, bool val) : User(MakePrimitiveDataType(PrimitiveDataType::TypeID::Boolean), std::to_string(val)), is_bool_(true), bool_val_(val) { value_type_ = ValueType::StaticValue; }
        explicit StaticValue(std::string name, std::vector<std::shared_ptr<StaticValue>> vals) : User(MakeArrayType(vals[0]->getType(), vals.size()), std::move(name)), is_array_(true), array_vals_(vals) { value_type_ = ValueType::StaticValue; }
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "";
            if (is_int_)
            {
                output += std::to_string(int_val_);
            }
            else if (is_float_)
            {
                output += std::to_string(float_val_);
            }
            else if (is_bool_)
            {
                output += std::to_string(bool_val_);
            }
            else if (is_array_)
            {
                output += "[";
                for (size_t i = 0; i < array_vals_.size(); i++)
                {
                    output += array_vals_[i]->dump(helper) + ", ";
                }

                output += "]";
            }

            return output;
        }

        void setStaticValue(int32_t val)
        {
            setType(MakePrimitiveDataType(PrimitiveDataType::TypeID::Int32));
            int_val_ = val;
        }

        void setStaticValue(float val)
        {
            setType(MakePrimitiveDataType(PrimitiveDataType::TypeID::Float32));
            float_val_ = val;
        }

        void setStaticValue(bool val)
        {
            setType(MakePrimitiveDataType(PrimitiveDataType::TypeID::Boolean));
            bool_val_ = val;
        }

        void setStaticValue(std::vector<std::shared_ptr<StaticValue>> vals)
        {
            array_vals_.clear();
            for (auto val : vals)
            {
                array_vals_.push_back(val);
            }
        }

        bool isInt() const
        {
            return type_->isPrimitive() && std::dynamic_pointer_cast<PrimitiveDataType>(type_)->isInt();
        }

        bool isFloat() const
        {
            return type_->isPrimitive() && std::dynamic_pointer_cast<PrimitiveDataType>(type_)->isFloat();
        }

        bool isBool() const
        {
            return type_->isPrimitive() && std::dynamic_pointer_cast<PrimitiveDataType>(type_)->isBool();
        }
        bool isArray() const
        {
            return type_->isArray();
        }

        int32_t getInt() const
        {
            return int_val_;
        }
        float getFloat() const
        {
            return float_val_;
        }
        bool getBool() const
        {
            return bool_val_;
        }
        std::shared_ptr<StaticValue> at(size_t idx) const
        {
            return array_vals_[idx];
        }
        std::shared_ptr<StaticValue> at(std::vector<size_t> idxs) const
        {
            auto val = array_vals_[idxs[0]];
            if (val->isArray())
            {
                return val->at(std::vector<size_t>(idxs.begin() + 1, idxs.end()));
            }
            else
            {
                return val;
            }
        }
        std::vector<std::shared_ptr<StaticValue>> getArray() const
        {
            return array_vals_;
        }

    protected:
        bool is_int_ = false;
        int32_t int_val_ = 0;
        bool is_float_ = false;
        float float_val_ = .0f;
        bool is_bool_ = false;
        bool bool_val_ = false;
        bool is_array_ = false;
        std::vector<std::shared_ptr<StaticValue>> array_vals_ = std::vector<std::shared_ptr<StaticValue>>(0);
    };

    std::shared_ptr<StaticValue> MakeEmptyStaticValue(std::shared_ptr<Type> type, std::string name);
}