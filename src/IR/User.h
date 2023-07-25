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
    explicit User(std::shared_ptr<Type> type, std::string name = "") : Value(type, name){};
     bool isConstant() const
    {
      return is_constant_;
    }
    bool isAllocate() const
    {
      return is_allocate_;
    }
    bool isFunction() const
    {
      return is_function_;
    }
  protected:
    bool is_constant_;
    bool is_allocate_;
    bool is_function_;
  };
}
