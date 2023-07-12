#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <string>

#include "Type.h"
#include "TypeFactory.h"
#include "User.h"

namespace ir
{
  class Constant : public User
  {
  public:
    explicit Constant(uint32_t val) : User(MakePrimitiveDataType(PrimitiveDataType::TypeID::Int32), "Constant"), val_(val) {}
    std::string dump() const override
    {
      return std::to_string(val_);
    }
    // [[nodiscard]] bool is_const() const override;

  protected:
    uint32_t val_ = 0;
  };

  class Allocate : public User
  {
  public:
    Allocate(std::unique_ptr<Type> type, std::string name) : User(MakePointerType(copyType(type)), std::move(name)), type_(std::move(type)) {}
    std::string dump() const override
    {
      return "Allocate " + getName() + " " + getType()->dump();
    }

  protected:
    std::unique_ptr<Type> type_;
  };

}
