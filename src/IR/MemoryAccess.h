#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <string>

#include "TypeFactory.h"
#include "Instruction.h"
#include "GlobalObject.h"

namespace ir
{
  class Constant : public User
  {
  public:
    explicit Constant(uint32_t val) : User(MakePrimitiveDataType(PrimitiveDataType::TypeID::Int32), "Constant"), val_(val) {}
    std::string dump(DumpHelper &helper) const override
    {
      return std::to_string(val_);
    }
    // [[nodiscard]] bool is_const() const override;

  protected:
    uint32_t val_ = 0;
  };


  class GlobalAllocate : public GlobalObject
  {
  public:
    GlobalAllocate(std::shared_ptr<Type> type, std::string name) : GlobalObject(MakePointerType(type->copy()), std::move(name)), type_(std::move(type)){};
    std::string dump(DumpHelper &helper) const override
    {
      return "GlobalVariable " + getName() + " " + getType()->dump();
    }

  protected:
    std::shared_ptr<Type> type_;
  };

  class Allocate : public User
  {
  public:
    Allocate(std::shared_ptr<Type> type, std::string name) : User(MakePointerType(type->copy()), std::move(name)), type_(std::move(type)){};
    std::string dump(DumpHelper &helper) const override
    {
      return "Allocate " + getName() + " " + getType()->dump();
    }

  protected:
    std::shared_ptr<Type> type_;
  };

  class Load : public Instruction
  {
    // TODO
  };

  class Store : public Instruction
  {
    // TODO
  };

}
