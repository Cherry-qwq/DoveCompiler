#pragma once

#include <cstdarg>
#include <cstdint>
#include <memory>
#include <utility>
#include <string>

#include "Type.h"
#include "Instruction.h"
#include "StaticValue.h"

namespace ir
{

    class Allocate : public Instruction
    {
    public:
        Allocate(std::shared_ptr<Type> type, std::string name) : Allocate(type, name, false, false, std::make_shared<StaticValue>(StaticValue(type, name, 0))) {}
        Allocate(std::string name, bool is_const, std::shared_ptr<StaticValue> val) : Allocate(val->getType(), name, is_const, false, val) {}
        Allocate(std::shared_ptr<Type> type, std::string name, bool is_global) : Allocate(type, name, false, is_global, std::make_shared<StaticValue>(StaticValue(type, name, 0))) {}
        Allocate(std::string name, bool is_const, bool is_global, std::shared_ptr<StaticValue> val) : Allocate(val->getType(), name, is_const, is_global, val) {}
        Allocate(std::shared_ptr<Type> type, std::string name, bool is_const, bool is_global, std::shared_ptr<StaticValue> val) : Instruction(MakePointerType(type->copy()), std::move(name), 1), type_(std::move(type)), is_const_(is_const), is_global_(is_global), staticvalue_(val) { value_type_ = ValueType::Allocate; }
        std::string dump(DumpHelper &helper) const override
        {
            std::string output = (is_const_ ? "ConstAlc " : "Allocate ") + getName() + ": " + getType()->dump() + " = " + staticvalue_->dump(helper);
            helper.add(output);
            return output;
        }

        void setConst(bool is_const)
        {
            is_const_ = is_const;
        }
        bool isConst() const
        {
            return is_const_;
        }

        void setInitValue(std::shared_ptr<StaticValue> val)
        {
            staticvalue_ = val;
        }
        std::shared_ptr<StaticValue> getInitValue() const
        {
            return staticvalue_;
        }

        void setGlobal(bool is_global)
        {
            is_global_ = is_global;
        }
        bool isGlobal() const
        {
            return is_global_;
        }

    protected:
        std::shared_ptr<Type> type_;
        bool is_const_ = false;
        bool is_global_ = false;
        std::shared_ptr<StaticValue> staticvalue_;
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
