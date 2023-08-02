#pragma once

#include <set>
#include <memory>
#include <algorithm>
#include <string>

#include "Use.h"
#include "Type.h"
#include "DumpHelper.h"

namespace ir
{
    class Value
    {

    public:
        enum class ValueType
        {
            Value,
            User,
            Instruction,

            // ControlFlow
            CompUnit,

            BasicBlock,
            Function,
            Icmp,
            Fcmp,
            Return,
            Br,
            Phi,
            Call,
            JPLabel,
            Label,

            // Operands
            StaticValue,

            // MemoryAccess
            Allocate,
            Store,
            Load,
            GetElementPtr,

            // Conversion
            ZExt,
            SExt,
            ConversionOp,
            Fptosi,
            Sitofp,

            // Operaters
            BinaryOperator,
            Add,
            FAdd,
            Sub,
            FSub,
            Mul,
            FMul,
            SDiv,
            FDiv,
            SRem,

        };

        ValueType getValueType() const
        {
            return value_type_;
        }

        void setValueType(ValueType Value_type)
        {
            value_type_ = Value_type;
        }
        Value(std::shared_ptr<Type> type, std::string name) : type_(type), name_(std::move(name)) {}
        virtual std::string dump(DumpHelper &helper) const = 0;
        bool bindUse(Use *use)
        {
            if (uses_.count(use))
                return false;

            uses_.insert(use);
            return true;
        }
        bool unbindUse(Use *use)
        {

            if (!uses_.count(use))
                return false;

            uses_.erase(use);
            return true;
        }

        Value &setName(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }
        std::string getName() const { return name_; }

        void setType(std::shared_ptr<Type> type) { type_ = std::move(type); }
        std::shared_ptr<Type> getType() const { return type_->copy(); }

        // virtual bool isConstant() const = 0;
    protected:
        ValueType value_type_ = ValueType::Value;
        std::set<Use *> uses_;
        std::shared_ptr<Type> type_;
        std::string name_;
    };
    typedef Value::ValueType ValueType;
}
