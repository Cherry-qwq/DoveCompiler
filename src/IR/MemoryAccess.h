#pragma once

#include <cstdarg>
#include <cstdint>
#include <memory>
#include <utility>
#include <string>

#include "Type.h"
#include "Instruction.h"

namespace ir
{
    class Constant : public Instruction
    {
    public:
        explicit Constant(std::shared_ptr<Type> type, std::string name, int32_t val) : Instruction(type, std::move(name), 2), is_int_(true), int_val_(val) { is_constant_ = true; }
        explicit Constant(std::shared_ptr<Type> type, std::string name, float val) : Instruction(type, std::move(name), 2), is_float_(true), float_val_(val) { is_constant_ = true; }
        explicit Constant(std::shared_ptr<Type> type, std::string name, bool val) : Instruction(type, std::move(name), 2), is_bool_(true), bool_val_(val) { is_constant_ = true; }
        explicit Constant(std::shared_ptr<Type> type, std::string name, std::vector<std::shared_ptr<Constant>> vals) : Instruction(type, std::move(name), 2), is_array_(true), array_vals_(vals) { is_constant_ = true; }
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "Constant ";
            if (is_int_)
                output += std::to_string(int_val_);
            else if (is_float_)
                output += std::to_string(float_val_);
            else if (is_bool_)
                output += std::to_string(bool_val_);
            else
                output += "null";
            helper.add(output);
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
        std::shared_ptr<Constant> arrAt(size_t idx) const
        {
            return array_vals_[idx];
        }
        std::shared_ptr<Constant> arrAt(std::vector<size_t> idxs) const
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
        std::vector<std::shared_ptr<Constant>> getArrayVal() const
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
        std::vector<std::shared_ptr<Constant>> array_vals_;
    };

    class Allocate : public Instruction
    {
    public:
        Allocate(std::shared_ptr<Type> type, std::string name) : Instruction(MakePointerType(type->copy()), std::move(name), 1), type_(std::move(type)) { is_allocate_ = true; };
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = (is_const_ ? "Constant " : "Allocate ") + getName() + ": " + getType()->dump();
            helper.add(output);
            return output;
        }

        void setConst(bool is_const)
        {
            is_const_ = is_const;
        }
        bool getConst() const
        {
            return is_const_;
        }

    protected:
        std::shared_ptr<Type> type_;
        bool is_const_ = false;
    };

    class Load : public Instruction
    {
    public:
        Load(std::shared_ptr<Value> ptr, std::string name) : Instruction(ptr->getType(), std::move(name), 1), ptr_(ptr, this){};
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "Load " + ptr_.getValue()->dump(helper);
            helper.add(output);
            return output;
        }

    protected:
        Use ptr_;
    };

    class Store : public Instruction
    {
    public:
        Store(std::shared_ptr<Value> val, std::shared_ptr<Value> ptr) : Instruction(MakePrimitiveDataType(PrimitiveDataType::TypeID::Void), "Store", 2), val_(val, this), ptr_(ptr, this){};

        std::string dump(DumpHelper &helper) const override
        {
            std::string output = "Store " + val_.getValue()->dump(helper) + " " + ptr_.getValue()->dump(helper);
            helper.add(output);
            return output;
        }

    protected:
        Use val_;
        Use ptr_;
    };

    class GetElementPtr : public Instruction
    {
    public:
        explicit GetElementPtr(std::shared_ptr<User> ptr, std::shared_ptr<Type> type, std::vector<std::shared_ptr<User>> idx, const std::string &name) : Instruction(MakePointerType(ptr->getType()->copy()), std::move(name), 2), ptr_(ptr, this)
        {
            for (auto &i : idx)
            {
                idx_.push_back(Use(i, this));
            }
        };

    protected:
        Use ptr_;
        std::vector<Use> idx_;
    };

    class ExtractValue : public Instruction
    {
    };

    class InsertValue : public Instruction
    {
    };
}
