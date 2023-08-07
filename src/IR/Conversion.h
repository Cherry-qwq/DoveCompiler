#pragma once

#include "Instruction.h"

namespace ir
{
    class ZExt : public Instruction
    {
    public:
        ZExt(std::shared_ptr<Value> src, std::shared_ptr<Value> dst, std::shared_ptr<Type> targetType)
            : Instruction(targetType, src->getName(), 1), src_(src, this), targetType_(targetType){value_type_=ValueType::ZExt;};

    protected:
        Use src_;
        std::shared_ptr<Type> targetType_;
    };
    
    class SExt : public Instruction
    {
        // TODO
    };

    class ConversionOp : public Instruction
    {
    public:
        explicit ConversionOp(std::shared_ptr<Value> src, std::shared_ptr<Value> dst, std::shared_ptr<Type> targetType)
            : Instruction(targetType, src->getName(), 1), src_(src, this), targetType_(targetType){value_type_=ValueType::ConversionOp;};

    protected:
        Use src_;
        std::shared_ptr<Type> targetType_;
    };

    class Fptosi : public ConversionOp
    {
        // TODO
    };
    class Sitofp : public ConversionOp
    {
        // TODO
    };
}