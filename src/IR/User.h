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
        enum class UserType
        {
            User,
            Instruction,

//ControlFlow
            CompUnit,

            BasicBlock,
            Function,
            Icmp,
            Return,
            Br,
            Phi,
            Call,
            JPLabel,
            Label,

//Operands
            StaticValue,

//MemoryAccess
            Allocate,
            Store,
            Load,
            GetElementPtr,

//Conversion
            ZExt,
            SExt,
            ConversionOp,
            Fptosi,
            Sitofp,

//Operaters
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
        explicit User(std::shared_ptr<Type> type, std::string name = "") : Value(type, name){};

        UserType getUserType() const
        {
            return user_type_;
        }
        
        void setUserType(UserType user_type)
        {
            user_type_ = user_type;
        }

        bool isStaticValue() const
        {
            return is_staticvalue_||user_type_==UserType::StaticValue;
        }
        bool isAllocate() const
        {
            return is_allocate_||user_type_==UserType::Allocate;
        }
        bool isFunction() const
        {
            return is_function_||user_type_==UserType::Function;
        }

    protected:
        UserType user_type_ = UserType::User;
        bool is_staticvalue_=false;
        bool is_allocate_=false;
        bool is_function_=false;
    };
}
