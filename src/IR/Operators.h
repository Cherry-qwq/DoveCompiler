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
      {
        if (std::dynamic_pointer_cast<PrimitiveDataType>(opleft->getType())->isInt() && std::dynamic_pointer_cast<PrimitiveDataType>(opright->getType())->isInt())
        {
          operands_.push_back(Use(opleft, this));
          operands_.push_back(Use(opright, this));
        }
      }
      else
      {
        throw std::runtime_error("Add: Invalid operand types");
      }
    };
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Add " + getName() + " = " + operands_[0].getValue()->getName() + " + " + operands_[1].getValue()->getName();
      helper.add(output);
      return output;
    }
  };

  class FAdd : public Instruction
  {
  public:
    explicit FAdd(std::shared_ptr<User> opleft, std::shared_ptr<User> opright, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2)
    {
      setType(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Float32));
      if (opleft->getType()->isPrimitive() && opright->getType()->isPrimitive())
      {
        if (std::dynamic_pointer_cast<PrimitiveDataType>(opleft->getType())->isFloat() && std::dynamic_pointer_cast<PrimitiveDataType>(opright->getType())->isFloat())
        {
          operands_.push_back(Use(opleft, this));
          operands_.push_back(Use(opright, this));
        }
      }
      else
      {
        throw std::runtime_error("FAdd: Invalid operand types");
      }
    };
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "FAdd " + getName() + " = " + operands_[0].getValue()->getName() + " + " + operands_[1].getValue()->getName();
      helper.add(output);
      return output;
    }
  };

  class Sub : public Instruction
  {
  public:
    explicit Sub(std::shared_ptr<User> opleft, std::shared_ptr<User> opright, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2)
    {
      setType(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32));
      if (opleft->getType()->isPrimitive() && opright->getType()->isPrimitive())
        if (std::dynamic_pointer_cast<PrimitiveDataType>(opleft->getType())->isInt() && std::dynamic_pointer_cast<PrimitiveDataType>(opright->getType())->isInt())
        {
          operands_.push_back(Use(opleft, this));
          operands_.push_back(Use(opright, this));
        }
        else
          throw std::runtime_error("Sub: Invalid operand types");
    };

  };

  class FSub : public Instruction
  {
  public:
    explicit FSub(std::shared_ptr<User> opleft, std::shared_ptr<User> opright, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2)
    {
      setType(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Float32));
      if (opleft->getType()->isPrimitive() && opright->getType()->isPrimitive())
        if (std::dynamic_pointer_cast<PrimitiveDataType>(opleft->getType())->isFloat() || std::dynamic_pointer_cast<PrimitiveDataType>(opright->getType())->isFloat())
        {
          operands_.push_back(Use(opleft, this));
          operands_.push_back(Use(opright, this));
        }
        else
          throw std::runtime_error("FSub: Invalid operand types");
    };
   // TODO
  };

  class Mul : public Instruction
  {
  public:
    explicit Mul(std::shared_ptr<User> opleft, std::shared_ptr<User> opright, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2)
    {
      setType(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Int32));
      if (opleft->getType()->isPrimitive() && opright->getType()->isPrimitive())
        if (std::dynamic_pointer_cast<PrimitiveDataType>(opleft->getType())->isInt() && std::dynamic_pointer_cast<PrimitiveDataType>(opright->getType())->isInt())
        {
          operands_.push_back(Use(opleft, this));
          operands_.push_back(Use(opright, this));
        }
        else
          throw std::runtime_error("Mul: Invalid operand types");
    };

  };

  class FMul : public Instruction
  {
  public:
    explicit FMul(std::shared_ptr<User> opleft, std::shared_ptr<User> opright, std::string name) : Instruction(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Void), std::move(name), 2)
    {
      setType(MakePrimitiveDataType(ir::PrimitiveDataType::TypeID::Float32);
      if (opleft->getType()->isPrimitive() && opright->getType()->isPrimitive())
        if (std::dynamic_pointer_cast<PrimitiveDataType>(opleft->getType())->isFloat() || std::dynamic_pointer_cast<PrimitiveDataType>(opright->getType())->isFloat())
        {
          operands_.push_back(Use(opleft, this));
          operands_.push_back(Use(opright, this));
        }
        else
          throw std::runtime_error("FMul: Invalid operand types");
    };
    
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