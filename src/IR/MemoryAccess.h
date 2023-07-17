#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <string>

#include "TypeFactory.h"
#include "Instruction.h"

namespace ir
{
  class Constant : public User
  {
  public:
    explicit Constant(std::shared_ptr<Type> type, uint32_t val) : User(type, "Constant"), val_(val) {}
    std::string dump(DumpHelper &helper) const override
    {
      return std::to_string(val_);
    }

  protected:
    uint32_t val_ = 0;
  };

  class Allocate : public User
  {
  public:
    Allocate(std::shared_ptr<Type> type, std::string name) : User(MakePointerType(type->copy()), std::move(name)), type_(std::move(type)){};
    std::string dump(DumpHelper &helper) const override
    {
      std::string output = "Allocate " + getName() + ": " + getType()->dump();
      helper.add(output);
      return output;
    }

    void setConst(bool is_const)
    {
      is_const_ = is_const;
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
      std::string output="Store " + val_.getValue()->dump(helper) + " " + ptr_.getValue()->dump(helper);
      helper.add(output);
      return output;
    }

  protected:
    Use val_;
    Use ptr_;
  };

  class GetElementPtr : public Instruction
  {
  };

  class ExtractValue : public Instruction
  {
  };

  class InsertValue : public Instruction
  {
  };

}
