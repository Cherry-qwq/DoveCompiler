#pragma once

#include <memory>

#include "Instruction.h"
#include "Type.h"
namespace ir
{
  class Add : public Instruction
  {
  public:
    explicit Add(std::shared_ptr<User> opleft, std::shared_ptr<User> opright, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2)
    {
      setType(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32));
      if (opleft->getType()->isPrimitive() && opright->getType()->isPrimitive())
        if (std::dynamic_pointer_cast<PrimitiveDataType>(opleft->getType())->isInt() && std::dynamic_pointer_cast<PrimitiveDataType>(opright->getType())->isInt())
        {
          operands_.push_back(Use(opleft, this));
          operands_.push_back(Use(opright, this));
        }
        else
          throw std::runtime_error("Add: Invalid operand types");
    };

  };

  class FAdd : public Instruction
  {
  };

  class Sub : public Instruction
  {
    // TODO
  };

  class FSub : public Instruction
  {
    // TODO
  };

  class Mul : public Instruction
  {
    // TODO
  };

  class FMul : public Instruction
  {
    // TODO
  };

  class UDiv : public Instruction
  {
    // TODO
  };

  class SDiv : public Instruction
  {
    // TODO
  };

  class FDiv : public Instruction
  {
    // TODO
  };

  class URem : public Instruction
  {
    // TODO
  };

  class SRem : public Instruction
  {
    // TODO
  };

  class FRem : public Instruction
  {
    // TODO
  };
}